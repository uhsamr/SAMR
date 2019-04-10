/*
Team: Shasta Advertiser Mobile Robot (SAMR)
Sponsor: Roscoe Furgeson (Ferguson Control Systems)
Members: Brandon Kain, Matt Metoyer, Jaime Juarez & Matt Aguiano
Semesters: Fall 2018 - Spring 2019
Capstone Conference: 4/26/2019

FPGA DIP Switch Configuration: MSEL[5:0] = 001010
FPGA rbf file name: DE1_SoC_Computer.rbf

*/
#include "samr_driver.h"
#include "functions.h"

/********************************************GLOBAL VARIABLE DECLARATIONS********************************************/
int numberinms = 0, pushed = 0, i = 0, j = 0, k = 0;
int srint, numberinms, LED, rc, allsensorsread = 0, start_at_1 = 1, array_position;
char srchar;
unsigned int switches, buttons, number_readings;
int srss1 = 1, srss2 = 1, srss3 = 1, srss4 = 1, srss5 = 1, srss6 = 1, srss7 = 1;//active low (srss# = serial read sonar sensor #
//these variables save the current value of each sensor
unsigned int ss1, ss2, ss3, ss4, ss5, ss6, ss7;
//trigger vatiable if sensor detected a wall (active low)
unsigned int ss1wall = 1, ss2wall = 1, ss3wall = 1, ss4wall = 1, ss5wall = 1, ss6wall = 1, ss7wall = 1;
//trigger variable if sensor detected a person (active low)
unsigned int ss1person = 1, ss2person = 1, ss3person = 1, ss4person = 1, ss5person = 1, ss6person = 1, ss7person = 1;
//These variables save the current sensor average values and are used to check if anything was detected.
unsigned int sonar1t, sonar2t, sonar3t, sonar4t, sonar5t, sonar6t, sonar7t;
//these are arrays used to take average of readings of sensors for more accurate reading.
unsigned int sonar1[100], sonar2[100], sonar3[100], sonar4[100], sonar5[100], sonar6[100], sonar7[100];
//If a sensor detected something its triggered value will be saved in this temp variable for comparison.
unsigned int ss1t, ss2t, ss3t, ss4t, ss5t, ss6t, ss7t;
//these are the average variables for each sensor
unsigned int ss1ave, ss2ave, ss3ave, ss4ave, ss5ave, ss6ave, ss7ave;
//these are flag variables for each sensor. flags are active low meaning if it equal 0 then it was triggered. safe state is 1.
int one = 1, two = 1, three = 1, four = 1, five = 1, six = 1, seven = 1;
//these are arrays of 100 readings to take average and see if person or wall detected.
unsigned int ss1temp[100], ss2temp[100];
unsigned int dutyl = 3800, dutyr = 3800;//start both duty cycles at off.
bool triggered = false;
bool walldetected = false;
bool humancheck = false;
/********************************************END GLOBAL VARIABLE DECLARATIONS********************************************/

/*************************************************FUNCTION DECLARATIONS*************************************************/
void button_L_R_PWM(void);

void switch_controlled_duty_cycle(void);

void read_all_sensors_triggered(void);

void check_if_anything_detected(void);

void determine_wall_or_human(void);

void turn_right(void);

void turn_left(void);

void determine_if_wall_avoided(void);

void check_sensor_flags(void);

void avoid_objects(void);

void sensor_4_triggered(void);

void sensor_1_and_7_triggered(void);

void get_average_of_all_sensors(void);

void reset_srss_flags(void);

void serial_sensor_number(void);

void serial_sensor_number_triggered(void);

void serial_sensor_value_single(void);

void serial_sensor_value_array(void);

void serial_sensor_value_single_triggered(void);

void serial_sensor_value_array_triggered(void);

void all_sensors_read_check(void);

void read_all_sensors_single(void);
	
void read_all_sensors_array(void);

void read_all_sensors_single_triggered(void);

void read_all_sensors_array_triggered(void);
/***********************************************END FUNCTION DECLARATIONS***********************************************/
 
/**************************************************START MAIN FUNCTION**************************************************/
int main() {

	 init();
	 number_readings = 2;//how many times to read all sensors in array
	 WriteLed(0x0);//turn all LEDs off.		
	 printroscoe();//print rOSCOE on displays
	 Stop_Motors();//start motors at 3,800 (off)
	
	for(;;){
		printf("FOR LOOP RESTARTED\n\n");
		read_all_sensors_single();
		
		/*ONLY NEED THIS WHEN DOING ARRAY READING*/
		//read_all_sensors_array();	
		//get_average_of_all_sensors();

		/*This function takes all "sonar#t" values and checks to see if they detected anything within their
		range, if so it sets that sensors flag and saves the "sonar#t" value to a "sonar#t" variable. If sonar1t 
		detected something then ss1t saves sonar1t value and the flag "one" will be set to 0 becuase the flags 
		are active low. This will also set the triggered flag to true if ANY sensor flag is set.*/
		check_if_anything_detected();

			
		if(triggered == true){/*was a while statement, changing to if because code inside should fix triggered.*/
			Stop_Motors();//stop motors to avoid / human detect
			dutyl = 3800;//reset duty cycles to 3800, which is "0" point.
			dutyr = 3800;
			
/*Wait 1[s] after triggered to let sensors steady so if it is a wall they get accurate and similar readings.*/
			numberinms = 500;//NOT SURE IF THIS IS GOOD OR NOT
			usleep( numberinms*1000 );//NOT SURE IF THIS IS GOOD OR NOT

			/*If the correct combination of sensors were triggered then go on to determine if wall or human, if not
			then the code will simply rotate according to sensors that were triggered with avoid_objects();.*/
			check_sensor_flags();
			
			if(humancheck == true){//only if certain sensors were triggered
			/*This function will take the newest "sonar#t" value and compare it to the "ss#t" value ONLY for sensors
			whose trigger flags were set (one, two etc.) and set the wall flag "ss#wall" or the person flag "ss#person"
			depending on the values of the newest sensor reading (sonar#t) and the original triggered reading (ss#t).*/
			printf("HUMANCHECKING\n\n");
			
			/*This function will take an array of reading for only triggered sensors and save the average of the
			array into sonar#t for comparison to the temp value ss#t, which holds the initial triggered sensor reading.*/
			read_all_sensors_single_triggered();

/*
SHOULD I CALL determine_wall_or_human(); MORE THAN ONCE INCASE THE READINGS WERE NOT STEADY AND IT WAS A WALL
BUT THE SENSORS WERE NOT SETTLED ON THEIR READINGS? read_all_sensors_single_triggered(); and call determine_wall_or_human();
multiple times and take the most common result? Ex. call this 5 times and if determine_wall_or_human(); determined
wall 3/5 times then we will set walldetected = true. if determined human 3/5 we will go to human triggered.
*/
				determine_wall_or_human();//sets walldetected flag.
			}//end if humancheck == true
			else{//if those sensors were not triggered avoid objects.
				printf("AVOIDING OBJECTS\n\n");
				avoid_objects();
			}//end else humancheck == false
			
			if((walldetected == true) && (humancheck == true)){
				printf("WALL DETECTED\n\n");
				sensor_4_triggered();
				
				sensor_1_and_7_triggered();
	
			}//end if walldetected
			else if(humancheck == true){/*if no wall detected then a human was detected. Wait 5-10 seconds and restart checking all sensors again
			This is because if it was maybe someone passing or a glitch reading then the sensors will read and they will
			not detect the glitch again. If it was a person and the person is still there then the sensor reading the person
			will trigger again, it will determine if wall or person and if person still then it will wait even longer and repeat this.*/
				Stop_Motors();
				dutyl = 3800;
				dutyr = 3800;
				numberinms = 1000;
				usleep( numberinms*1000 );
				triggered = false;
			}//end else human detected
			else{/*This means human check was not true so the code did not "determine_wall_or_human" so the code will not
			have walldetected flag set and also should not go through the human detected wait state. This means the code
			went through the avoiding algorithm.*/}
			
		}//end if triggered == true;
		else{/*triggered == false*/}
		
	}//end for(;;)

return(0);}
/***************************************************END MAIN FUNCTION***************************************************/

/*********************************************************************************************************************
This function takes the sonar#t values and checks if any of the sensors readings are within the specified range for
detection. If any sensor reading is in detection range then the value will be saved in ss#t, this is a trigger saving
to be compared later. If sensor detect then their corresponding trigger flag will also be set. This variable corresonds
to the number of sensor. If sensor 1 detected something then flag "one" will be set. Flags are active low (0 = set).
If any of the flags were set then this function sets "triggered" = true.
*********************************************************************************************************************/
void check_if_anything_detected(void){
	
/*If a sensor reads something less than 24 inches away it will set the corresponding
flag by setting the number sensor it is equal to 1. If a sensor doesnt detect anything
it will set flag to 0. Flag is active high.*/
		LED = 0;
//sensor 1 (left corner)
		if((sonar1t < 24) && (sonar1t > 8)){//if first sonar sensor reads less than 2 feet
			printf("SS 1 triggered: %d [in]\r\n\n",sonar1t);
			LED = LED + 0x1;
			ss1t = sonar1t;//save current value in triggered value to compare
			one = 0;}//set sensor 1 trigger flag
		else{one = 1;}
		
//sensor 2 (left middle)
		if((sonar2t < 24) && (sonar2t > 8)){//if second sonar sensor reads less than 2 feet
			printf("SS 2 triggered: %d [in]\r\n\n",sonar2t);
			LED = LED + 0x2;
			ss2t = sonar2t;//save current value in temp to compare to.
			two = 0;}
		else{two = 1;}
		
//sensor 3 (left floor)
		if((sonar3t > 65)){
			printf("SS 3 triggered: %d [in]\r\n\n",sonar3t);
			LED = LED + 0x4;
			ss3t = sonar3t;//save current value in temp to compare to.
			three = 0;}
		else{three = 1;}
		
//sensor 4 (middle middle)
		if((sonar4t < 24) && (sonar4t > 8)){
			printf("SS 4 triggered: %d [in]\r\n\n",sonar4t);
			LED = LED + 0x8;
			ss4t = sonar4t;//save current value in temp to compare to.
			four = 0;}
		else{four = 1;}
		
//sensor 5 (right floor)
		if((sonar5t > 57)){
			printf("SS 5 triggered: %d [in]\r\n\n",sonar5t);
			LED = LED + 0x10;
			ss5t = sonar5t;//save current value in temp to compare to.
			five = 0;}
		else{five = 1;}
		
//sensor 6 (right middle)
		if((sonar6t < 24) && (sonar6t > 8)){
			printf("SS 6 triggered: %d [in]\r\n\n",sonar6t);
			LED = LED + 0x20;
			ss6t = sonar6t;//save current value in temp to compare to.
			six = 0;}
		else{six = 1;}
		
//sensor 7 (right corner)
		if((sonar7t < 24) && (sonar7t > 8)){
			printf("SS 7 triggered: %d [in]\r\n\n",sonar7t);
			LED = LED + 0x40;
			ss7t = sonar7t;//save current value in temp to compare to.
			seven = 0;}
		else{seven = 1;}
		WriteLed(LED);
		
//Check sensor trigger flags
		/*IF ONLY SENSOR 1 OR SENSOR 7 SET DO NOT STOP OR SET TRIGGERED FLAG*/
		if(( (one == 0) || (seven == 0) ) && ( (three == 1) && (four == 1) && (five == 1) && (six == 1) && (two == 1) )){
			Forward_Motors();	
			triggered = false;
		}
		else if((one == 0) || (two == 0) || (three == 0) || (four == 0) || (five == 0) || (six == 0) || (seven == 0)){
			triggered = true;
		}
		else{//if all trigger flags == 1 (nothing was detected) then turn off all LEDs and do not print anything.
			Forward_Motors();	
			printf("MOTORS FORWARD\n\n");
			triggered = false;
		}
}// END check_if_anything_detected


/*********************************************************************************************************************
Calling this function will take a reading from ONLY the sensors whose flags were set every 2[ms] and store it in an 
array. After 100 readings, which is 200[ms], the function takes the average of the array and stores that value in sonar#t.
*********************************************************************************************************************/
void read_all_sensors_triggered(void){
		 //The loop delays 2ms and runs 100 times so take 200ms to get readings.
		// for(j = 0; j < 100; j++){
			
// if(one == 0){
			// ReadPW(PW0,&ss1);
			// sonar1[j] = ss1;
			// }
// if(two == 0){
			// ReadPW(PW1,&ss2);
			// sonar2[j] = ss2;
			// }
// if(three == 0){
			// ReadPW(PW2,&ss3);
			// sonar3[j] = ss3;
			// }
// if(four == 0){
			// ReadPW(PW3,&ss4);
			// sonar4[j] = ss4;
			// }
// if(five == 0){
			// ReadPW(PW4,&ss5);
			// sonar5[j] = ss5;
			// }
// if(six == 0){
			// ReadPW(PW5,&ss6);
			// sonar6[j] = ss6;
			// }
// if(seven == 0){
			// ReadPW(PW6,&ss7);
			// sonar7[j] = ss7;
			// }
			
			//wait function where numberinms is how many ms to wait.
			// numberinms = 2;
			// usleep( numberinms*1000 );
								// }//end for loop
		
			// /*Can take average of all sonars again because if a sensors flag was not 
			// triggered then its array will not be over written, therefore the sonar#t
			// average will end up being the same still*/
			// get_average_of_all_sensors();
			
}// END read_all_sensors_triggered


/*********************************************************************************************************************
Calling this function checks ONLY triggered sensors to see if their most recent reading (sonar#t) is equal to, +1[in]
or -1[in] from the triggered values (ss#t). If it is then that means a wall was detected and the ss#wall flag is set.
If the most recent reading is more than 1[in] from the triggered reading it means the sensor detected a person or 
something moving so the ss#person flag is set for that sensor. If any of the ss#wall flags were set then the flag
"walldetected" = true is set.
*********************************************************************************************************************/
void determine_wall_or_human(void){
/*
ss1t = value that triggered. sonar1t = newest average sensor reading.
if new average (sonar#t) is equal to the triggered value (ss#t) then wall.
if they are not it is a human.

- trigger variable if sensor detected a wall (active low = 0)
unsigned int ss1wall = 1, ss2wall = 1, ss3wall = 1, ss4wall = 1, ss5wall = 1, ss6wall = 1, ss7wall = 1;
- trigger variable if sensor detected a person (active low = 0)
unsigned int ss1person = 1, ss2person = 1, ss3person = 1, ss4person = 1, ss5person = 1, ss6person = 1, ss7person = 1;
*/
/************************************************************************/	
//								SENSOR 1
if(one == 0){//if the first sensor was initially triggered
				/*If ss1t (the sonar reading value that triggered the detection is equal to, +2 or -2 the values
				of sonar1t (the most recent reading of sonar 1) then the sensor detected a wall */
				if((ss1t == sonar1t) || (ss1t == (sonar1t+2)) || (ss1t == (sonar1t-2))) {//wall detected
					ss1wall = 0;//set wall flag for sensor 1 (active low)
					ss1person = 1;//make sure person flag for sensor 1 is not triggered
					printf("SS 1 detected a wall! SS1 reading = %d[in]\r\n\n", sonar1t);
				}
				else{//object detected
					/*If the newest sensor reading is not within 2[in] of the triggered reading then chances
					are that it was a passing person or a person standing there looking giving inconsistent readings.*/
					ss1wall = 1;
					ss1person = 0;
					printf("SS 1 detected a object! SS1 reading = %d[in]\r\n\n", sonar1t);
				}
}
else{
	ss1wall = 1;
	ss1person = 1;
	}
/************************************************************************/	
//								SENSOR 2
if(two == 0){
				if((ss2t == sonar2t) || (ss2t == (sonar2t+2)) || (ss1t == (sonar2t-2))){
					ss2wall = 0;//active low
					ss2person = 1;
				}
				else{
					ss2wall = 1;
					ss2person = 0;
				}
}
else{
	ss2wall = 1;
	ss2person = 1;
	}
/************************************************************************/	
//								SENSOR 3
if(three == 0){
				if((ss3t == sonar3t) || (ss3t == (sonar3t+2)) || (ss1t == (sonar3t-2))){
					ss3wall = 0;
					ss3person = 1;
				}
				else{
					ss3wall = 1;
					ss3person = 0;
				}
}
else{
	ss3wall = 1;
	ss3person = 1;
	}
/************************************************************************/	
//								SENSOR 4
if(four == 0){
				if((ss4t == sonar4t) || (ss4t == (sonar4t+2)) || (ss4t == (sonar4t-2))){
					ss4wall = 0;
					ss4person = 1;
				}
				else{
					ss4wall = 1;
					ss4person = 0;
				}
}
else{
	ss4wall = 1;
	ss4person = 1;
	}
/************************************************************************/	
//								SENSOR 5
if(five == 0){
				if((ss5t == sonar5t) || (ss5t == (sonar5t+2)) || (ss5t == (sonar5t-2))){
					ss5wall = 0;
					ss5person = 1;
				}
				else{
					ss5wall = 1;
					ss5person = 0;
				}
}
else{
	ss5wall = 1;
	ss5person = 1;
	}
/************************************************************************/	
//								SENSOR 6
if(six == 0){
				if((ss6t == sonar6t) || (ss6t == (sonar6t+2)) || (ss6t == (sonar6t-2))){
					ss6wall = 0;
					ss6person = 1;
				}
				else{
					ss6wall = 1;
					ss6person = 0;
				}
}
else{
	ss6wall = 1;
	ss6person = 1;
	}
/************************************************************************/	
//								SENSOR 7
if(seven == 0){
				if((ss7t == sonar7t) || (ss7t == (sonar7t+2)) || (ss7t == (sonar7t-2))){
					ss7wall = 0;
					ss7person = 1;
				}
				else{
					ss7wall = 1;
					ss7person = 0;
				}
}
else{
	ss7wall = 1;
	ss7person = 1;
	}

		//if a wall was detected by any sensor then set the walldetected flag to true.
		if((ss1wall == 0) || (ss2wall == 0) || (ss3wall == 0) || (ss4wall == 0) || (ss5wall == 0) || (ss6wall == 0) || (ss7wall == 0)){
			walldetected = true;}
		else{
			walldetected = false;}
}// END determine_wall_or_human


/*********************************************************************************************************************
Calling this function will make the robot turn right. The right wheel must go backwards and the left wheel must go
forward. This checks "dutyl' to make sure it is less than 3,800 (because forward speed is less than this) and more than
2,600 (max speed forward). If it is in this range then decrease it by 50 to speed up forward. This checks "dutyr" 
to make sure it is more than 3,800 (because reverse speed is higher number), but less than 4,999 (max speed backwards).
If it is in this range it adds 50 to increase reverse speed. After sets both PWMl and PWMr to active + dutycycle.
*********************************************************************************************************************/
void turn_right(void){
	
	/*To turn right left motor goes forward. Anything less than 3,800 is forward speed. Max forward speed can be
	2,600. This means dutyl cannot be less than 2,600.*/
	if(dutyl < 2600){
		dutyl = 2600;
	}
	else if(dutyl > 3800){
		dutyl = 3800;
	}
	else{
		dutyl = dutyl - 50;
	}
	
	/*To turn right right motor goes backwards. Anything more than 3,800 is backwards, but max is 4,9999. dutyr must
	be at least 3,800+*/
	if(dutyr < 3800){
		dutyr = 3800;
	}
	else if(dutyr > 4999){
		dutyr = 4999;
	}
	else{
		dutyr = dutyr + 50;
	}
	
	printf("TURNING RIGHT, dutyl = %d & dutyr = %d\r\n\n",dutyl,dutyr);
	
	WritePWM(PWMl,ACTIVATE_PWM + dutyl);
	WritePWM(PWMr,ACTIVATE_PWM + dutyr);
	
}


/*********************************************************************************************************************
Calling this function will make the robot turn left. The right wheel must go forward and the left wheel must go
backward. This checks "dutyr' to make sure it is less than 3,800 (because forward speed is less than this) and more than
2,600 (max speed forward). If it is in this range then decrease it by 50 to speed up forward. This checks "dutyl" 
to make sure it is more than 3,800 (because reverse speed is higher number), but less than 4,999 (max speed backwards).
If it is in this range it adds 50 to increase reverse speed. After sets both PWMl and PWMr to active + dutycycle.
*********************************************************************************************************************/
void turn_left(void){
	
	printf("TURNING LEFT, dutyl = %d & dutyr = %d\r\n\n",dutyl,dutyr);
		
	/*To turn left right motor goes forward. Anything less than 3,800 is forward speed. Max forward speed can be
	2,600. This means dutyr cannot be less than 2,600.*/
	if(dutyr < 2600){
		dutyr = 2600;
	}
	else if(dutyr > 3800){
		dutyr = 3800;
	}
	else{
		dutyr = dutyr - 50;
	}
	
	/*To turn left left motor goes backwards. Anything more than 3,800 is backwards, but max is 4,9999. dutyl must
	be at least 3,800+*/
	if(dutyl < 3800){
		dutyl = 3800;
	}
	else if(dutyl > 4999){
		dutyl = 4999;
	}
	else{
		dutyl = dutyl + 50;
	}
	
	WritePWM(PWMl,ACTIVATE_PWM + dutyl);
	WritePWM(PWMr,ACTIVATE_PWM + dutyr);
	
}


/*********************************************************************************************************************
This function takes the new sonar#t number and checks to see if it is still within the triggering range of that sensor.
If it is then the ss#wall flag remains triggered, if it is not it will change the flag to 1 (off). If the sensor is no 
longer reading in range then the ss#wall going 1 will break the while loop and the code will restart and check sensor again.
If the sensor is still in range then ss#wall stays 0 and the motors turning speed increases and all sensors are read again
to determine if sensor reading something in range still.
*********************************************************************************************************************/
void determine_if_wall_avoided(void){
	
//								SENSOR 1
if(one == 0){//only check if sensor 1 was initially triggered
				if((sonar1t < 24) && (sonar1t > 0)){//if sensor 1 is reading within range keep wall trigger flag
					ss1wall = 0;//wall not avoided
					printf("Wall not avoided because SS1 reading = %d   <  24[in]\r\n\n",sonar1t);
				}//end if sonar reads in range
				else{//if sensor not in range change wall flag to 1 (not triggered anymore)
					ss1wall = 1;//wall avoided
					printf("SS 1 avoided a wall! SS1 reading = %d\r\n\n",sonar1t);
				}//end else sonar did not read in range.
}//end if one == 0
else{
	ss1wall = 1;
	}//end else one == 1
/************************************************************************/	
//								SENSOR 2
if(two == 0){
				if((sonar2t < 24) && (sonar2t > 0)){
					ss2wall = 0;//active low
				}
				else{
					ss2wall = 1;
				}
}
else{
	ss2wall = 1;
	}
/************************************************************************/	
//								SENSOR 3
if(three == 0){
				if(sonar3t > 26){
					ss3wall = 0;
				}
				else{
					ss3wall = 1;
				}
}
else{
	ss3wall = 1;
	}
/************************************************************************/	
//								SENSOR 4
if(four == 0){
				if((sonar4t < 24) && (sonar4t > 0)){
					ss4wall = 0;
				}
				else{
					ss4wall = 1;
				}
}
else{
	ss4wall = 1;
	}
/************************************************************************/	
//								SENSOR 5
if(five == 0){
				if(sonar5t > 26){
					ss5wall = 0;
				}
				else{
					ss5wall = 1;
				}
}
else{
	ss5wall = 1;
	}
/************************************************************************/	
//								SENSOR 6
if(six == 0){
				if((sonar6t < 24) && (sonar6t > 0)){
					ss6wall = 0;
				}
				else{
					ss6wall = 1;
				}
}
else{
	ss6wall = 1;
	}
/************************************************************************/	
//								SENSOR 7
if(seven == 0){
				if((sonar7t < 24) && (sonar7t > 0)){
					ss7wall = 0;
				}
				else{
					ss7wall = 1;
				}
}
else{
	ss7wall = 1;
	}

}// END determine_if_wall_avoided


/*********************************************************************************************************************
This function will check the triggered sensors and decide if we need to do a human check or not. This is to make sure
the robot is not constantly stopping for every sensor trigger. Unless sensor 4 is triggered or sensor 1 and 7
and 2 or 6 are triggered the robot will not check to see if it was a person it will simply turn away until the 
sensors are no longer triggered and continue moving.
*********************************************************************************************************************/
void check_sensor_flags(void){
	/*Sensor 4 is the middle sensor so if this one is triggered we definitely need to check for a person or wall.*/
	if(four == 0){
		humancheck = true;
	}
	/*Sensor 1 & 7 are the far left and right side of the robot. If both of these are triggered AND sensor 2 or 6
	this means the robot has encountered some type of door way or narrow path and must avoid it or go through it.*/
	else if( ((one == 0) && (seven == 0)) && ((two == 0) || (six == 0)) ){
		humancheck = true;
	}
	/*If any other sensors or combination of sensor are triggered then we do not need to check for a person or wall
	we will go directly to avoiding what was detected. This sets humancheck to false.*/
	else{
		humancheck = false;
	}
}


/********************************************************************************************************************
This function will be called if the sensor combinations that are being used to detect a human or wall were not set.
This means that the robot must take the sensors that were triggered and try avoiding the objects that triggered them.
********************************************************************************************************************/
void avoid_objects(void){
	/*We do not need to worry about the case with ONLY sensor 1 or 7 triggered because of their placement.
	If only these sensors are triggered the object is not directly in the path of SAMR, if the object comes
	too into the path of samr then sensors 2 or 6 will also trigger, which will trigger the humancheck flag.*/
	while(((three == 0) && (five == 0)) || (three == 0) || ((one == 0) && (two == 0)) ){//line 5, 6 & 9 Sensor Scenarios for SAMR
		turn_right();//starts turning motors
		read_all_sensors_single_triggered();//checks only sensors that were triggered.
		/*only sensors that were triggered were checked again so only those flags will be changed.*/
		check_if_anything_detected();
		//rotate right
		}//end while
				
	while((five == 0) || ((six == 0) && (seven == 0))){//line 7 & 8 Sensor Scenarios for SAMR
		turn_left();//starts turning motors
		read_all_sensors_single_triggered();//only reads sensors triggered and changes their sonar#t value
		check_if_anything_detected();//takes new sonar#t value of triggered sensors and updates triggered flags.
		//rotate left
	}//end while
}


/********************************************************************************************************************
This function is called if walldetected and humancheck are both true. It stays in a while loop as long as sensor 4's
wall trigger is set. It then checks other sensors wall trigger flags to move accordingly. Each run the function makes
sure sensor 4 wall trigger is still set. Checks other sensors wall trigger flags. Moves motors accordingly. Reads
all sensors triggered again to update sonar#t values. Determines if the wall is avoided by seeing if new sonar# reading
is still within the range that triggered it.
********************************************************************************************************************/
void sensor_4_triggered(void){
	
	while(ss4wall == 0){/*All of these sensor scenarios will only happen if ss4 detected a wall 
	also becuase it is the middle sensor and must be triggered in order to check for a person.*/
		if((ss2wall == 0) && (ss6wall == 0) && (ss7wall == 0)){//line 15
			turn_left();//starts turning motors left.
			/*only reads sensors whose flags were set and updates their sonar#t value to be checked again*/
			read_all_sensors_single_triggered();
			/*This takes new sonar#t value of triggered sensors and checks to see if it is within their
			initial trigger range still.*/
			determine_if_wall_avoided();
		}
		else if((ss1wall == 0) && (ss2wall == 0) && (ss6wall == 0)){//line 16 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else if((ss6wall == 0) && (ss7wall == 0)){//line 10 Sensor Scenarios for SAMR
			turn_left();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else if((ss1wall == 0) && (ss2wall == 0)){//line 11 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else if((ss2wall == 0) && (ss6wall == 0)){//line 14 Sensor Scenarios for SAMR
			turn_left();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else if((ss1wall == 0) || (ss2wall == 0)){//line 12 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else if((ss6wall == 0) || (ss7wall == 0)){//line 13 Sensor Scenarios for SAMR
			turn_left();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else{//else ONLY sensor 4 detected something?
		}
		
	}//end while ss4 detects a wall
	
}// end sensor_4_triggered


/********************************************************************************************************************


********************************************************************************************************************/
void sensor_1_and_7_triggered(void){
	/*These scenarios will only happen if ss1 and ss7 detected a wall. This is because
	if the robot detects something on both sides it needs to be careful going between 
	them or completely turn around and avoid it.*/
	while((ss1wall == 0) && (ss7wall == 0)){
		/*TO GET HERE BOTH SS1&SS7 HAD TO BE TRIGGERED, BUT TO AVOID THE 
	SITUATION I THINK THIS MUST BE OR BECAUSE WHAT IF SS1, SS6 & SS7 WERE TRIGGERED. ROBOT WILL TURN LEFT.
	THIS MEANS WHAT SS1 READ SS2 MIGHT READ NOW AND WHAT SS6 & SS7 READ ONLY SS7 MIGHT READ SO ONLY SS2 AND 
	SS7 ARE TRIGGERED, BUT THIS MIGHT NOT BE GOOD TO START GOING FORWARD WITH??????*/
/*
SHOULD THIS WHILE LOOP INCLUDE SS1 AND SS7 TRIGGERED, IF THE ROBOT TURNS AND EITHER OF THOSE ARE NO LONGER
TRIGGERED THE ROBOT WILL STOP AVOIDING. SHOULD I MAKE THE WHILE ABOVE AND IF AND THEN BELOW MAKE THE IF STATEMENT
WHILE LOOPS SO THE SS1 AND SS7 GIVE US THE SITUATION THE ROBOT IS IN AND THE BELOW STATEMENTS WILL TAKE
THOSE SENSORS INTO ACCOUNT FOR AVOIDING THE WALL??????
*/
		if((ss2wall == 0) && (ss6wall == 0)){//line 20 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else if((ss2wall == 0)){//line 19 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else if((ss6wall == 0)){//line 18 Sensor Scenarios for SAMR
			turn_left();
			read_all_sensors_single_triggered();
			determine_if_wall_avoided();
		}
		else{//only sensor 1 & 7 detected something?
		}
		
	}//end if ss1 & ss7 detected a wall	
	
}


/*********************************************************************************************************************
This function takes all sonar# arrays and calls Average_Reading for each sensor and stores the average of all
values in sonar#t.
*********************************************************************************************************************/
void get_average_of_all_sensors(void){
	//get average of 100 readings	
	sonar1t = Average_Reading(sonar1, number_readings);
	sonar2t = Average_Reading(sonar2, number_readings);
	sonar3t = Average_Reading(sonar3, number_readings);	
	sonar4t = Average_Reading(sonar4, number_readings);
	sonar5t = Average_Reading(sonar5, number_readings);
	sonar6t = Average_Reading(sonar6, number_readings);
	sonar7t = Average_Reading(sonar7, number_readings);
}


/*******************************************************************************************************************************************
This function is called after each sensors reading is read so that the next loop all flags will be cleared and the loop can determine
which sensor number is about to be read so it can set that flag and the previous flag will not still be set.
*******************************************************************************************************************************************/
void reset_srss_flags(void){
	//printf(" %d\n\n",srint);
	srss1 = 1;
	srss2 = 1;
	srss3 = 1;
	srss4 = 1;
	srss5 = 1;
	srss6 = 1;
	srss7 = 1;
}


/********************************************************************************************************************************************
This function will trigger every time the do while loop checks a sensors reading, this will only happen after the sensor number has been
determined and then the sensor reading for that sensor is checked. This means that allsensorsread will only increase every other run through
the loop. If the flag has not gone up to 6 it will increase +1. Once the flag hits 6 it will reset and increment array_position which
tells the arrays that sensor values are being stored in to increment by 1.
********************************************************************************************************************************************/
void all_sensors_read_check(void){
		if(allsensorsread == 6){//if all 7 read, reset number and increment array position for all.
			allsensorsread = 0;
			array_position = array_position + 1;
			//printf("Reading %d of all sensors.\n\n",(i/14) + 1);
		}
		else{
		allsensorsread = allsensorsread + 1;//add to number of sensors read for array position.
		}
}

/***************************************************FUNCTIONS FOR READING ALL SENSORS ONCE***************************************************/

/********************************************************************************************************************************************
This function takes the serial reading of sensors and saves each corresonding sensor reading to sonar#t. Readings from serial come in as:
sensor number
sensor reading
and repeates constantly.
********************************************************************************************************************************************/
void read_all_sensors_single(void){

	i = 0;
	do{
		rc = ReadSerial(SONAR,&srchar);//reads serial from SONAR and puts in srchar
		srint = (int)srchar;//convert char reading to integer
		if(rc){//only run if rc == 1, meaning it successfully read a new value.
			
			if(srint < 8){//if srint less than 8 it is a sensor
				serial_sensor_number();//call this to read single value of each sensor.0
			}//end if sring < 8
			else{//srint > 8 so is a sensor reading.
				serial_sensor_value_single();
			}//end else srint > 8
			i = i + 1;
		}//end if( (rc) && (start_at_1 == true) )	

	}while(i < 14);/*14 readings to get number and value for each sensor. So 14*# readings want
	+ 1 incase the readings started at a value and not the sensor number. */
}


/*******************************************************************************************************************************************
This function takes the new srint value that is = 8+ and checks to see which sensor flag was set. If a sensor flag was set this means that
the reading is the distance value for that sensor so the function will take the srint reading and set it equal to sonar#t if doing a single reading.
*******************************************************************************************************************************************/
void serial_sensor_value_single(void){
	
	if(srss1 == 0){/*if srss1 flag was set that meant the reading before was a 1 and this tells the loop
	that the next reading is the distance reading for sensor 1.*/
		sonar1t = srint;
		reset_srss_flags();//reset flag for sonar reading.
	}
	else if(srss2 == 0){
		sonar2t = srint;
		reset_srss_flags();
	}
	else if(srss3 == 0){
		sonar3t = srint;
		reset_srss_flags();
	}
	else if(srss4 == 0){
		sonar4t = srint;
		reset_srss_flags();
	}
	else if(srss5 == 0){
		sonar5t = srint;
		reset_srss_flags();
	}
	else if(srss6 == 0){
		sonar6t = srint;
		reset_srss_flags();
	}
	else if(srss7 == 0){
		sonar7t = srint;
		reset_srss_flags();
	}
	else{}
	
}

/********************************************************************************************************************************************
This function takes the serial reading of triggered sensors and saves each corresonding sensor reading to sonar#t. Readings from serial come in as:
sensor number
sensor reading
and repeates constantly.
********************************************************************************************************************************************/
void read_all_sensors_single_triggered(void){
	i = 0;
	do{
		rc = ReadSerial(SONAR,&srchar);//reads serial from SONAR and puts in srchar
		srint = (int)srchar;//convert char reading to integer
		if(rc){//only run if rc == 1, meaning it successfully read a new value.
			
			if(srint < 8){//if srint less than 8 it is a sensor
				serial_sensor_number_triggered();//call this to read single value of each sensor.0
			}//end if sring < 8
			else{//srint > 8 so is a sensor reading.
				serial_sensor_value_single_triggered();
			}//end else srint > 8
			i = i + 1;
		}//end if( (rc) && (start_at_1 == true) )	

	}while(i < 14);/*14 readings to get number and value for each sensor. So 14*# readings want
	+ 1 incase the readings started at a value and not the sensor number. */
	
}


/*******************************************************************************************************************************************
This function takes the new srint value that is = 8+ and checks to see which sensor flag was set. If a sensor flag was set and that sensors
trigger flag was set this means that the reading is the distance value for that sensor so the function will take the srint reading and 
set it equal to sonar#t if doing a single reading.
*******************************************************************************************************************************************/
void serial_sensor_value_single_triggered(void){
	
	if((srss1 == 0) && (one == 0)){/*if srss1 flag was set that meant the reading before was a 1 and this tells the loop
	that the next reading is the distance reading for sensor 1.*/
		sonar1t = srint;
		reset_srss_flags();//reset flag for sonar reading.
	}
	else if((srss2 == 0) && (two == 0)){
		sonar2t = srint;
		reset_srss_flags();
	}
	else if((srss3 == 0) && (three == 0)){
		sonar3t = srint;
		reset_srss_flags();
	}
	else if((srss4 == 0) && (four == 0)){
		sonar4t = srint;
		reset_srss_flags();
	}
	else if((srss5 == 0) && (five == 0)){
		sonar5t = srint;
		reset_srss_flags();
	}
	else if((srss6 == 0) && (six == 0)){
		sonar6t = srint;
		reset_srss_flags();
	}
	else if((srss7 == 0) && (seven == 0)){
		sonar7t = srint;
		reset_srss_flags();
	}
	else{}
	
}


/*******************************************************************************************************************************************
This function takes the srint reading that we know is a sensor number and checks which sensor number it is and sets the corresponding flag.
This allows us to know that the next serial reading will be the distance reading for that sensor.
*******************************************************************************************************************************************/
void serial_sensor_number(void){

	if(srint == 1){/*If srint = the number of the sensor then that sensors srss1 flag
	is set. srss# = serial read sonar sensor #. This will tell the next value, whose 
	value will be greater than 8, that the reading is for that specific sensor.*/
		srss1 = 0;
	}
	else if(srint == 2){
		srss2 = 0;
	}
	else if(srint == 3){
		srss3 = 0;
	}
	else if(srint == 4){
		srss4 = 0;
	}
	else if(srint == 5){
		srss5 = 0;
	}
	else if(srint == 6){
		srss6 = 0;
	}
	else if(srint == 7){
		srss7 = 0;
	}
	else{
		srss1 = 1;
		srss2 = 1;
		srss3 = 1;
		srss4 = 1;
		srss5 = 1;
		srss6 = 1;
		srss7 = 1;
	}

}


/*******************************************************************************************************************************************
This function takes the srint reading that we know is a sensor number and checks which sensor number it is and checks to make sure that
sensor was triggered and if so sets the corresponding flag. This allows us to know that the next serial reading will be the distance 
reading for that sensor.
*******************************************************************************************************************************************/
void serial_sensor_number_triggered(void){
	
	if((srint == 1) && (one == 0)){/*If srint = the number of the sensor then that sensors srss1 flag
	is set. srss# = serial read sonar sensor #. This will tell the next value, whose 
	value will be greater than 8, that the reading is for that specific sensor.*/
		srss1 = 0;
	}
	else if((srint == 2) && (two == 0)){
		srss2 = 0;
	}
	else if((srint == 3) && (three == 0)){
		srss3 = 0;
	}
	else if((srint == 4) && (four == 0)){
		srss4 = 0;
	}
	else if((srint == 5) && (five == 0)){
		srss5 = 0;
	}
	else if((srint == 6) && (six == 0)){
		srss6 = 0;
	}
	else if((srint == 7) && (seven == 0)){
		srss7 = 0;
	}
	else{
		srss1 = 1;
		srss2 = 1;
		srss3 = 1;
		srss4 = 1;
		srss5 = 1;
		srss6 = 1;
		srss7 = 1;
	}
	
}

/***********************************************FUNCTIONS FOR READING ALL SENSORS AS ARRAY**************************************************/

/********************************************************************************************************************************************
This function takes the serial reading of sensor and saves each corresponding sensor reading in an array sonar# for a specific amount of 
reading which is a multiple of 14. Must change number_readings value to how many readings of each sensor want to do.
********************************************************************************************************************************************/
void read_all_sensors_array(void){
	
	array_position = 0;
	i = 0;
	do{
		rc = ReadSerial(SONAR,&srchar);//reads serial from SONAR and puts in srchar
		srint = (int)srchar;//convert char reading to integer
		if(rc){//only run if rc == 1, meaning it successfully read a new value.
			
			if(srint < 8){//if srint less than 8 it is a sensor
				serial_sensor_number();//call this to read array of values of each sensor.
				//printf("sonar%dt[%d] = ",srint,array_position);
			}//end if sring < 8
			else{//srint > 8 so is a sensor reading.
				serial_sensor_value_array();
			}//end else srint > 8
			i = i + 1;
		}//end if( (rc) && (start_at_1 == true) )	

	}while(i < (number_readings*14));/*14 readings to get number and value for each sensor. So 14*# readings want
	+ 1 incase the readings started at a value and not the sensor number. */

}


/*******************************************************************************************************************************************
This function takes the new srint value that is = 8+ and checks to see which sensor flag was set. If a sensor flag was set this means that
the reading is the distance value for that sensor so the function will take the srint reading and set it equal to sonar#[array_position]
if doing an array reading.
*******************************************************************************************************************************************/
void serial_sensor_value_array(void){
	
	if(srss1 == 0){/*if srss1 flag was set that meant the reading before was a 1 and this tells the loop
	that the next reading is the distance reading for sensor 1.*/
		sonar1[array_position] = srint;//save the sensor reading in array.
		reset_srss_flags();//reset flag for sonar reading.
		all_sensors_read_check();
	}
	else if(srss2 == 0){
		sonar2[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if(srss3 == 0){
		sonar3[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if(srss4 == 0){
		sonar4[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if(srss5 == 0){
		sonar5[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if(srss6 == 0){
		sonar6[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if(srss7 == 0){
		sonar7[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else{}
	
}


/********************************************************************************************************************************************
This function takes the serial reading of sensor and saves each corresponding sensor reading in an array sonar# for a specific amount of 
reading which is a multiple of 14. Must change number_readings value to how many readings of each sensor want to do.
********************************************************************************************************************************************/
void read_all_sensors_array_triggered(void){
	
	array_position = 0;
	i = 0;
	do{
		rc = ReadSerial(SONAR,&srchar);//reads serial from SONAR and puts in srchar
		srint = (int)srchar;//convert char reading to integer
		if(rc){//only run if rc == 1, meaning it successfully read a new value.
			
			if(srint < 8){//if srint less than 8 it is a sensor
				serial_sensor_number_triggered();//call this to read array of values of each sensor.
				//printf("sonar%dt[%d] = ",srint,array_position);
			}//end if sring < 8
			else{//srint > 8 so is a sensor reading.
				serial_sensor_value_array_triggered();
			}//end else srint > 8
			i = i + 1;
		}//end if( (rc) && (start_at_1 == true) )	

	}while(i < (number_readings*14));/*14 readings to get number and value for each sensor. So 14*# readings want
	+ 1 incase the readings started at a value and not the sensor number. */
	
}


/*******************************************************************************************************************************************
This function takes the new srint value that is = 8+ and checks to see which sensor flag was set. If a sensor flag was set this means that
the reading is the distance value for that sensor. This also makes sure that the sensors trigger flag was set and if so the function will
take the srint reading and set it equal to sonar#[array_position] if doing an array reading.
*******************************************************************************************************************************************/	
void serial_sensor_value_array_triggered(void){
	
	if((srss1 == 0) && (one == 0)){/*if srss1 flag was set that meant the reading before was a 1 and this tells the loop
	that the next reading is the distance reading for sensor 1.*/
		sonar1[array_position] = srint;//save the sensor reading in array.
		reset_srss_flags();//reset flag for sonar reading.
		all_sensors_read_check();
	}
	else if((srss2 == 0) && (two == 0)){
		sonar2[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if((srss3 == 0) && (three == 0)){
		sonar3[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if((srss4 == 0) && (four == 0)){
		sonar4[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if((srss5 == 0) && (five == 0)){
		sonar5[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if((srss6 == 0) && (six == 0)){
		sonar6[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else if((srss7 == 0) && (seven == 0)){
		sonar7[array_position] = srint;
		reset_srss_flags();
		all_sensors_read_check();
	}
	else{}
	
}

/*********************************************************************************************************************


*********************************************************************************************************************/









/*********************************************************************************************************************/
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
/*********************************************************************************************************************/
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
/*********************************************************************************************************************/