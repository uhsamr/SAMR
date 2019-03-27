//When writing to HEX, LED etc. look at them as bits:
/* HEX# = bit-6 bit-5 bit-4 bit-3 bit-2 bit-1 bit-0 = 0000000 to turn them all on or 1111111 to turn them all off, 
 but that is for binary, so we must write a hex number or decimal equivalent to the binary bits we want */
/* LED = bit-9 bit-8 bit-7 bit-6 bit-5 bit-4 bit-3 bit-2 bit-1 bit-0 */
#include "samr_driver.h"
#include "functions.h"

/****************************************************************************************************************************************
*********************************RECENT DISCOVERY*********************************
1. TO GET SENSORS TO WORK MUST DO ALL WIRING BEFORE TURN FPGA ON!!!!!!!!!!!!!!!!!!!!
2. TURN ON FPGA AND MUST DISCONNECT FEEDBACK THROUGH 1K RESISTOR BEFORE RUNNING EXECUTABLE (DON'T THINK HAVE TO DO THIS)!!!!!!!!!!!!!!!!!!
3. DOING CHAINING THE BW PIN ON EVERY SONAR SENSOR MUST BE CONNECTED TO +5[V]!!!!!!!!!!!!!!!!!!
************************************************************************************************************************************/
/********************************GLOBAL VARIABLE DECLARATIONS********************************/
 int numberinms = 0, pushed = 0, i = 0, j = 0, k = 0;
 int flags[6];
 unsigned int switches, buttons;
 //these variables save the current value of each sensor
 unsigned int ss1, ss2, ss3, ss4, ss5, ss6, ss7;
 //trigger vatiable if sensor detected a wall (active low)
 unsigned int ss1wall = 1, ss2wall = 1, ss3wall = 1, ss4wall = 1, ss5wall = 1, ss6wall = 1, ss7wall = 1;
 //trigger variable if sensor detected a person (active low)
 unsigned int ss1person = 1, ss2person = 1, ss3person = 1, ss4person = 1, ss5person = 1, ss6person = 1, ss7person = 1;
 //these variables are used to save the temp sensor reading to check if it was triggered or not
 unsigned int sonar1t, sonar2t, sonar3t, sonar4t, sonar5t, sonar6t, sonar7t;
 //these are arrays used to take average of readings of sensors for more accurate reading.
 unsigned int sonar1[100], sonar2[100], sonar3[100], sonar4[100], sonar5[100], sonar6[100], sonar7[100];
 //these save the triggered value of each sensor to compare to the average for human detection.
 unsigned int ss1t, ss2t, ss3t, ss4t, ss5t, ss6t, ss7t;
 //these are the average variables for each sensor
 unsigned int ss1ave, ss2ave, ss3ave, ss4ave, ss5ave, ss6ave, ss7ave;
 //these are flag variables for each sensor. flags are active low meaning if it equal 0 then it was triggered. safe state is 1.
 int one = 1, two = 1, three = 1, four = 1, five = 1, six = 1, seven = 1;
 //these are arrays of 100 readings to take average and see if person or wall detected.
 unsigned int ss1temp[100], ss2temp[100];
 unsigned int dutyl = 3800, dutyr = 3800;//start both duty cycles at off.
 bool triggered = false;
/********************************END GLOBAL VARIABLE DECLARATIONS********************************/

/*************************************FUNCTION DECLARATIONS*************************************/
void Test_All_Sensors(void);

void Test_All_Sensors_Arrays(void);

void button_L_R_PWM(void);

void switch_controlled_duty_cycle(void);

void read_all_sensors(void);

void read_all_sensors_triggered(void);

void check_if_clear_of_wall(void);

void check_if_anything_detected(void);

void determine_wall_or_human(void);
/***********************************END FUNCTION DECLARATIONS***********************************/
 
/**************************************START MAIN FUNCTION**************************************/
int main() {

	 init();
	 WriteLed(0x0);//turn all LEDs off.		
	 printroscoe();
	 Stop_Motors();
	
	for(;;){
		read_all_sensors();
		check_if_anything_detected();

			
			if(triggered == true){
				read_all_sensors_triggered();
				determine_wall_or_human();
			}
			else{}
	

		
	}//end for(;;)

return(0);}
/***************************************END MAIN FUNCTION***************************************/

/*********************************************************************************************************************/
void check_if_clear_of_wall(void){
	
	
	
	
}
/*********************************************************************************************************************/
void read_all_sensors(void){
		 //The loop delays 2ms and runs 100 times so take 200ms to get readings.
		for(j = 0; j < 100; j++){
			ReadPW(PW0,&ss1);
			ReadPW(PW1,&ss2);
			ReadPW(PW2,&ss3);
			ReadPW(PW3,&ss4);
			ReadPW(PW4,&ss5);
			ReadPW(PW5,&ss6);
			ReadPW(PW6,&ss7);
			sonar1[j] = ss1;
			sonar2[j] = ss2;
			sonar3[j] = ss3;
			sonar4[j] = ss4;
			sonar5[j] = ss5;
			sonar6[j] = ss6;
			// wait function where numberinms is how many ms to wait.
			numberinms = 2;
			usleep( numberinms*1000 );
		}
			sonar1t = Average_Reading(sonar1);//get average of 100 readings	
			sonar2t = Average_Reading(sonar2);//get average of 100 readings	
			sonar3t = Average_Reading(sonar3);//get average of 100 readings	
			sonar4t = Average_Reading(sonar4);//get average of 100 readings	
			sonar5t = Average_Reading(sonar5);//get average of 100 readings	
			sonar6t = Average_Reading(sonar6);//get average of 100 readings	
			sonar7t = Average_Reading(sonar7);//get average of 100 readings	
	
}
/*********************************************************************************************************************/
void check_if_anything_detected(void){
	
/*If a sensor reads something less than 24 inches away it will set the corresponding
flag by setting the number sensor it is equal to 1. If a sensor doesnt detect anything
it will set flag to 0. Flag is active high.*/
			
//sensor 1 (left corner)
		if((sonar1t < 16) && (sonar1t > 0)){//if first sonar sensor reads less than 2 feet
			printf("SS 1 triggered: %d [in]\r\n\n",sonar1t);
			WriteLed(0x1);
			ss1t = sonar1t;//save current value in temp to compare to.
			one = 0;
			flags[0] = 0;}
		else{
			one = 1;
			flags[0] = 0;}
//sensor 2 (left middle)
		if((sonar2t < 24) && (sonar2t > 0)){//if second sonar sensor reads less than 2 feet
			printf("SS 2 triggered: %d [in]\r\n\n",sonar2t);
			WriteLed(0x2);
			ss2t = sonar2t;//save current value in temp to compare to.
			two = 0;
			flags[1] = 0;}
		else{
			two = 1;
			flags[1] = 1;}
//sensor 3 (left floor)
		if((sonar3t < 24) && (sonar3t > 0)){
			printf("SS 3 triggered: %d [in]\r\n\n",sonar3t);
			WriteLed(0x4);
			ss3t = sonar3t;//save current value in temp to compare to.
			three = 0;
			flags[2] = 0;}
		else{three = 1;
			flags[2] = 1;}
//sensor 4 (middle middle)
		if((sonar4t < 24) && (sonar4t > 0)){
			printf("SS 4 triggered: %d [in]\r\n\n",sonar4t);
			WriteLed(0x10);
			ss4t = sonar4t;//save current value in temp to compare to.
			four = 0;
			flags[3] = 0;}
		else{four = 1;
			flags[3] = 1;}
//sensor 5 (right floor)
		if((sonar5t < 24) && (sonar5t > 0)){
			printf("SS 5 triggered: %d [in]\r\n\n",sonar5t);
			WriteLed(0x20);
			ss5t = sonar5t;//save current value in temp to compare to.
			five = 0;
			flags[4] = 0;}
		else{five = 1;
			flags[4] = 1;}
//sensor 6 (right middle)
		if((sonar6t < 24) && (sonar6t > 0)){
			printf("SS 6 triggered: %d [in]\r\n\n",sonar6t);
			WriteLed(0x40);
			ss6t = sonar6t;//save current value in temp to compare to.
			six = 0;
			flags[5] = 0;}
		else{six = 1;
			flags[5] = 1;}
//sensor 7 (right corner)
		if((sonar7t < 16) && (sonar7t > 0)){
			printf("SS 7 triggered: %d [in]\r\n\n",sonar7t);
			WriteLed(0x80);
			ss7t = sonar7t;//save current value in temp to compare to.
			seven = 0;
			flags[6] = 0;}
		else{seven = 1;
			flags[6] = 1;}
		
//if all trigger flags == 1 (nothing was detected) then turn off all LEDs and do not print anything.
		if((one == 0) || (two == 0) || (three == 0) || (four == 0) || (five == 0) || (six == 0) || (seven == 0)){
			Stop_Motors();
			triggered = true;}
		else{
			Forward_Motors();	
			WriteLed(0x0);
			triggered = false;}
}
/*********************************************************************************************************************/
void read_all_sensors_triggered(void){
		 //The loop delays 2ms and runs 100 times so take 200ms to get readings.
		for(j = 0; j < 100; j++){
			if(one == 0){
			ReadPW(PW0,&ss1);
			sonar1[j] = ss1;
			}
			if(two == 0){
			ReadPW(PW1,&ss2);
			sonar2[j] = ss2;
			}
			if(three == 0){
			ReadPW(PW2,&ss3);
			sonar3[j] = ss3;
			}
			if(four == 0){
			ReadPW(PW3,&ss4);
			sonar4[j] = ss4;
			}
			if(five == 0){
			ReadPW(PW4,&ss5);
			sonar5[j] = ss5;
			}
			if(six == 0){
			ReadPW(PW5,&ss6);
			sonar6[j] = ss6;
			}
			if(seven == 0){
			ReadPW(PW6,&ss7);
			sonar7[j] = ss7;
			}
			
			// wait function where numberinms is how many ms to wait.
			numberinms = 2;
			usleep( numberinms*1000 );
		}
			sonar1t = Average_Reading(sonar1);//get average of 100 readings	
			sonar2t = Average_Reading(sonar2);//get average of 100 readings	
			sonar3t = Average_Reading(sonar3);//get average of 100 readings	
			sonar4t = Average_Reading(sonar4);//get average of 100 readings	
			sonar5t = Average_Reading(sonar5);//get average of 100 readings	
			sonar6t = Average_Reading(sonar6);//get average of 100 readings	
			sonar7t = Average_Reading(sonar7);//get average of 100 readings	
	
}
/*********************************************************************************************************************/
void determine_wall_or_human(void){
/*ss1t = value that triggered. sonar1t = newest average sensor reading.
if new average (sonar#t) is equal to the triggered value (ss#t) then wall.
if they are not it is a human.

						trigger variable if sensor detected a wall
unsigned int ss1wall = 1, ss2wall = 1, ss3wall = 1, ss4wall = 1, ss5wall = 1, ss6wall = 1, ss7wall = 1;
						trigger variable if sensor detected a person.
unsigned int ss1person = 1, ss2person = 1, ss3person = 1, ss4person = 1, ss5person = 1, ss6person = 1, ss7person = 1;
*/
//sensor 1
				if(ss1t == sonar1t){//wall detected
					ss1wall = 0;//active low
					ss1person = 1;
				}
				else{//object detected
					ss1wall = 1;//active low
					ss1person = 0;
				}
//sensor 2
				if(ss2t == sonar2t){
					ss2wall = 0;//active low
					ss2person = 1;
				}
				else{
					ss2wall = 1;//active low
					ss2person = 0;
				}
//sensor 3
				if(ss3t == sonar3t){
					ss3wall = 0;//active low
					ss3person = 1;
				}
				else{
					ss3wall = 1;//active low
					ss3person = 0;
				}
//sensor 4
				if(ss4t == sonar4t){
					ss4wall = 0;//active low
					ss4person = 1;
				}
				else{
					ss4wall = 1;//active low
					ss4person = 0;
				}
//sensor 5
				if(ss5t == sonar5t){
					ss5wall = 0;//active low
					ss5person = 1;
				}
				else{
					ss5wall = 1;//active low
					ss5person = 0;
				}
//sensor 6
				if(ss6t == sonar6t){
					ss6wall = 0;//active low
					ss6person = 1;
				}
				else{
					ss6wall = 1;//active low
					ss6person = 0;
				}
//sensor 7
				if(ss7t == sonar7t){
					ss7wall = 0;//active low
					ss7person = 1;
				}
				else{
					ss7wall = 1;//active low
					ss7person = 0;
				}
}

/********************************************************************************************************************
Calling this function will take all 7 sensor readings and light up the corresponding LED and print out the 
sensor triggered and the value it read. Sensors triggered if they read withint 24[in].
*/

void Test_All_Sensors(void){
/*call this in a for(;;) loops to constantly check if any of the sensors
read something within 24 inches, if they do it will print which sensor,
the reading and light up the corresponding LED.*/
		ReadPW(PW0,&ss1);
		ReadPW(PW1,&ss2);
		ReadPW(PW2,&ss3);
		ReadPW(PW3,&ss4);
		ReadPW(PW4,&ss5);
		ReadPW(PW5,&ss6);
		ReadPW(PW6,&ss7);
//sensor 1
		if((ss1 < 24) && (ss1 > 0)){//if first sonar sensor reads less than 2 feet
			printf("SS 1: %d [in]\r\n\n",ss1);
			//WritePWM(PWMl,LOW_DUTY);
			WriteLed(0x1);
			one = 0;}//max speed forward
		else{
			//WritePWM(PWMl,STOP_DUTY);
			one = 1;}
//sensor 2
		if((ss2 < 24) && (ss2 > 0)){//if second sonar sensor reads less than 2 feet
			//WritePWM(PWMr,LOW_DUTY);
			printf("SS 2: %d [in]\r\n\n",ss2);
			WriteLed(0x2);
			two = 0;}//max speed forward
		else{
			//WritePWM(PWMr,STOP_DUTY);
			two = 1;}
//sensor 3
		if((ss3 < 24) && (ss3 > 0)){
			printf("SS 3: %d [in]\r\n\n",ss3);
			WriteLed(0x4);
			three = 0;}
		else{three = 1;}
//sensor 4
		if((ss4 < 24) && (ss4 > 0)){
			printf("SS 4: %d [in]\r\n\n",ss4);
			WriteLed(0x10);
			four = 0;}
		else{four = 1;}
//sensor 5
		if((ss5 < 24) && (ss5 > 0)){
			printf("SS 5: %d [in]\r\n\n",ss5);
			WriteLed(0x20);
			five = 0;}
		else{five = 1;}
//sensor 6
		if((ss6 < 24) && (ss6 > 0)){
			printf("SS 6: %d [in]\r\n\n",ss6);
			WriteLed(0x40);
			six = 0;}
		else{six = 1;}
//sensor 7
		if((ss7 < 24) && (ss7 > 0)){
			printf("SS 7: %d [in]\r\n\n",ss7);
			WriteLed(0x80);
			seven = 0;}
		else{seven = 1;}
		//if all trigger flags == 1 then turn off all LEDs and do not print anything.
		if((one == 0) || (two == 0) || (three == 0) || (four == 0) || (five == 0) || (six == 0) || (seven == 0)){}
		else{WriteLed(0x0);}
}

/********************************************************************************************************************
Calling this function will take all 7 sensor readings and light up the corresponding LED and print out the 
sensor triggered and the value it read. Sensors triggered if they read withint 24[in].
*/

void Test_All_Sensors_Arrays(void){
/*call this in a for(;;) loops to constantly check if any of the sensors
read something within 24 inches, if they do it will print which sensor,
the reading and light up the corresponding LED.*/
		
		if(j < 100){
			ReadPW(PW0,&ss1);
			ReadPW(PW1,&ss2);
			ReadPW(PW2,&ss3);
			ReadPW(PW3,&ss4);
			ReadPW(PW4,&ss5);
			ReadPW(PW5,&ss6);
			ReadPW(PW6,&ss7);
			sonar1[j] = ss1;
			sonar2[j] = ss2;
			sonar3[j] = ss3;
			sonar4[j] = ss4;
			sonar5[j] = ss5;
			sonar6[j] = ss6;
			sonar7[j] = ss7;
			++j;
		}
		else{
			sonar1t = Average_Reading(sonar1);//get average of 100 readings	
			sonar2t = Average_Reading(sonar2);//get average of 100 readings	
			sonar3t = Average_Reading(sonar3);//get average of 100 readings	
			sonar4t = Average_Reading(sonar4);//get average of 100 readings	
			sonar5t = Average_Reading(sonar5);//get average of 100 readings	
			sonar6t = Average_Reading(sonar6);//get average of 100 readings	
			sonar7t = Average_Reading(sonar7);//get average of 100 readings	
			j = 0;
		}

/*If a sensor reads something less than 24 inches away it will set the corresponding
flag by setting the number sensor it is equal to 1. If a sensor doesnt detect anything
it will set flag to 0. Flag is active high.*/
			
//sensor 1 (left corner)
		if((sonar1t < 24) && (sonar1t > 0)){//if first sonar sensor reads less than 2 feet
			printf("SS 1 triggered: %d [in]\r\n\n",sonar1t);
			WriteLed(0x1);
			ss1t = sonar1t;//save current value in temp to compare to.
			one = 0;
			flags[0] = 0;}
		else{
			one = 1;
			flags[0] = 0;}
//sensor 2 (left middle)
		if((sonar2t < 24) && (sonar2t > 0)){//if second sonar sensor reads less than 2 feet
			printf("SS 2 triggered: %d [in]\r\n\n",sonar2t);
			WriteLed(0x2);
			ss2t = sonar2t;//save current value in temp to compare to.
			two = 0;
			flags[1] = 0;}
		else{
			two = 1;
			flags[1] = 1;}
//sensor 3 (left floor)
		if((sonar3t < 24) && (sonar3t > 0)){
			printf("SS 3 triggered: %d [in]\r\n\n",sonar3t);
			WriteLed(0x4);
			ss3t = sonar3t;//save current value in temp to compare to.
			three = 0;
			flags[2] = 0;}
		else{three = 1;
			flags[2] = 1;}
//sensor 4 (middle middle)
		if((sonar4t < 24) && (sonar4t > 0)){
			printf("SS 4 triggered: %d [in]\r\n\n",sonar4t);
			WriteLed(0x10);
			ss4t = sonar4t;//save current value in temp to compare to.
			four = 0;
			flags[3] = 0;}
		else{four = 1;
			flags[3] = 1;}
//sensor 5 (right floor)
		if((sonar5t < 24) && (sonar5t > 0)){
			printf("SS 5 triggered: %d [in]\r\n\n",sonar5t);
			WriteLed(0x20);
			ss5t = sonar5t;//save current value in temp to compare to.
			five = 0;
			flags[4] = 0;}
		else{five = 1;
			flags[4] = 1;}
//sensor 6 (right middle)
		if((sonar6t < 24) && (sonar6t > 0)){
			printf("SS 6 triggered: %d [in]\r\n\n",sonar6t);
			WriteLed(0x40);
			ss6t = sonar6t;//save current value in temp to compare to.
			six = 0;
			flags[5] = 0;}
		else{six = 1;
			flags[5] = 1;}
//sensor 7 (right corner)
		if((sonar7t < 24) && (sonar7t > 0)){
			printf("SS 7 triggered: %d [in]\r\n\n",sonar7t);
			WriteLed(0x80);
			ss7t = sonar7t;//save current value in temp to compare to.
			seven = 0;
			flags[6] = 0;}
		else{seven = 1;
			flags[6] = 1;}
		

		
//if all trigger flags == 1 (nothing was detected) then turn off all LEDs and do not print anything.
		if((one == 0) || (two == 0) || (three == 0) || (four == 0) || (five == 0) || (six == 0) || (seven == 0)){
			Stop_Motors();
		}
		else{
			Forward_Motors();	
			WriteLed(0x0);}

}

//********************************************************************************************************************

void button_L_R_PWM(void){
/*flip SW9 up to control left motor, flip SW0 up to control right motor,
flip SW1 up to stop both motors. Press KEY3 to decrease speed, press 
KEY2 to increase speed*/
		buttons = ReadKeys();
		switches = ReadSwitches();
		if(switches == 0x200){//SW9 up to control left motor
			if((buttons == 0x7) && (pushed == 0)){//KEY3 increases duty cycle
				dutyl = dutyl + 100;
				pushed = 1;
			}
			else if((buttons == 0xB) && (pushed == 0)){//KEY2 decreases duty cycle
				dutyl = dutyl - 100;
				pushed = 1;
			}
			else if(pushed == 1){//if either button was pressed check to see if it was released.
				if(buttons == 0xF){pushed =0;}
				else{pushed = 1;}
			}
			else{pushed = 0;}//do nothing if no buttons pressed
			if(dutyl > 5000){dutyl = 5000;}
			else if(dutyl < 0){dutyl = 0;}
			else{}
			WritePWM(PWMl,ACTIVATE_PWM + dutyl);
		}
		else if(switches == 0x1){//SW0 up to control right motor
			if((buttons == 0x7) && (pushed == 0)){//KEY3 increases duty cycle
				dutyr = dutyr + 100;
				pushed = 1;
			}
			else if((buttons == 0xB) && (pushed == 0)){//KEY2 decreases duty cycle
				dutyr = dutyr - 100;
				pushed = 1;
			}
			else if(pushed == 1){//if either button was pressed check to see if it was released.
				if(buttons == 0xF){pushed =0;}
				else{pushed = 1;}
			}
			else{pushed = 0;}//do nothing if no buttons pressed
			if(dutyr > 5000){dutyr = 5000;}
			else if(dutyr < 0){dutyr = 0;}
			else{}
			WritePWM(PWMr,ACTIVATE_PWM + dutyr);
		}
		else if(switches == 0x2){//SW1 turns both motors off
			WritePWM(PWMl,STOP_DUTY);
			WritePWM(PWMr,STOP_DUTY);
		}
		else{//no switches up leave them as is
			WritePWM(PWMl,ACTIVATE_PWM + dutyl);
			WritePWM(PWMr,ACTIVATE_PWM + dutyr);
		}	
}

//********************************************************************************************************************

void switch_controlled_duty_cycle(void){
	unsigned int duty;
	unsigned int switches;
	switches = ReadSwitches();
	WriteLed(ReadSwitches());
	switch(switches){
		case 0x0:
		duty = 1;
		break;
		case 0x1:
		duty = 500;
		break;
		case 0x2:
		duty = 1000;
		break;
		case 0x4:
		duty = 1500;
		break;
		case 0x8:
		duty = 2000;
		break;
		case 0x10:
		duty = 2500;
		break;
		case 0x20:
		duty = 3000;
		break;
		case 0x40:
		duty = 3500;
		break;
		case 0x80:
		duty = 4000;
		break;
		case 0x100:
		duty = 4500;
		break;
		case 0x200:
		duty = 4999;//MAX FORWARD DUTY CYCLE 3.2[V] OUTPUT
		break;
		default:
		duty = 0;
		break;
	}//end switch statement
	WritePWM(PWM1,ACTIVATE_PWM + duty);	
}

//********************************************************************************************************************




/*

		if(triggered == false){
			
			
		for(i = 0; i < 100; ++i){//save 100 readings of sensor in array
			ReadPW(PW0,&ss1);
			sonar1[i] = ss1;
		}
		sonar1t = Average_Reading(sonar1);//get average of 100 readings	
			
		// ReadPW(PW0,&ss1);
		
//If a sensor reads something less than 24 inches away it will set the corresponding
//flag by setting the number sensor it is equal to 1. If a sensor doesnt detect anything
//it will set flag to 0. Flag is active high.


// sensor 1
		if((sonar1t < 24) && (sonar1t > 0)){//if first sensor less than 2 feet.
			// printf("SS 1: %d [in]\r\n\n",ss1);
			WriteLed(0x1);//turn on first LED
			one = 1;//set first sensor flag
			triggered = true;//turn on triggered loop
			printf("SS1 has been triggered\n\n");
			printf("SS 1: %d [in]\r\n\n",sonar1t);
			ss1t = ss1;//save current value in temp to compare to.
			Stop_Motors();
		}
		else{
			one = 0;
			WriteLed(0x0);
			Forward_Motors();
		}
		
		
		
		
		
		}//end triggered == false
		else{//if triggered == true meaning a sensor was triggered.
			
		// wait function where numberinms is how many ms to wait.
		numberinms = 5;
		usleep( numberinms*1000 );
			
	
		if(one == 1){
	
			if(j < 100){
				++j;
				ReadPW(PW0,&ss1);
				ss1temp[j] = ss1;
				//printf("SS 1: %d [in]\r\n\n",ss1);
			}
			else{
				ss1ave = Average_Reading(ss1temp);//get average of 100 readings
				printf("Trigger Value = %d [in] vs. Average Value: %d [in]\r\n\n",ss1t,ss1ave);
				if(ss1t == ss1ave){//if the average is equal to the trigger value, it is probably a wall
					printf("SS1 is detecting a wall, keep moving.\n\n");
					triggered = false;//this will take it out of this loop and flags will be set to current.
				}
				else{//if the average and trigger values are not same, it is probably a moving person or object
//moving around so stay put and read an array of values again.
					printf("SS1 is detecting a moving object, stay put.\n\n");
					j = 0;
				}
			}
			
			
		}
		else{//sensor 1 was not triggered within 24 inches.
		}
		
		
		
		
		
		}//end if triggered == true
		
		*/










