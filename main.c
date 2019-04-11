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

void check_if_anything_detected(void);

void turn_right(void);

void turn_left(void);

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

void reset_duty_and_stop_motors(void);
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

		/*This function takes all "sonar#t" values and checks to see if they detected anything within their
		range, if so it sets that sensors flag and saves the "sonar#t" value to a "sonar#t" variable. If sonar1t 
		detected something then ss1t saves sonar1t value and the flag "one" will be set to 0 becuase the flags 
		are active low. This will also set the triggered flag to true if ANY sensor flag is set.*/
		check_if_anything_detected();

			
		while(triggered == true){/*.*/
			reset_duty_and_stop_motors();
			
			//numberinms = 500;//NOT SURE IF THIS IS GOOD OR NOT
			//usleep( numberinms*1000 );//NOT SURE IF THIS IS GOOD OR NOT

			/*triggered is already set, but we stopped motors and read all sensors and check if anything detected again
			to make sure. If not then will skip all avoiding functions and read one more ti
			me before breaking while loop.*/
			read_all_sensors_single();
			check_if_anything_detected();

			if(triggered == true){
				printf("running sensor_4_triggered\n\n");
				sensor_4_triggered();

				printf("running avoid_objects\n\n");
				avoid_objects();
				
				printf("running sensor_1_and_7_triggered\n\n");
				sensor_1_and_7_triggered();
			}
			else{
			}
			
			reset_duty_and_stop_motors();
			
			read_all_sensors_single();
			check_if_anything_detected();
		
		}//end while triggered == true;
		
		Forward_Motors();	
		
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
			//Forward_Motors();	
			triggered = false;
		}
		else if((one == 0) || (two == 0) || (three == 0) || (four == 0) || (five == 0) || (six == 0) || (seven == 0)){
			triggered = true;
		}
		else{//if all trigger flags == 1 (nothing was detected) then turn off all LEDs and do not print anything.
			//Forward_Motors();	
			//printf("MOTORS FORWARD\n\n");
			triggered = false;
		}
}// END check_if_anything_detected


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


/********************************************************************************************************************
This function will be called if the sensor combinations that are being used to detect a human or wall were not set.
This means that the robot must take the sensors that were triggered and try avoiding the objects that triggered them.
********************************************************************************************************************/
void avoid_objects(void){
	reset_duty_and_stop_motors();
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

	reset_duty_and_stop_motors();
				
	while((five == 0) || ((six == 0) && (seven == 0))){//line 7 & 8 Sensor Scenarios for SAMR
		turn_left();//starts turning motors
		read_all_sensors_single_triggered();//only reads sensors triggered and changes their sonar#t value
		check_if_anything_detected();//takes new sonar#t value of triggered sensors and updates triggered flags.
		//rotate left
	}//end while
	
	reset_duty_and_stop_motors();
}


/********************************************************************************************************************
This function is called if walldetected and humancheck are both true. It stays in a while loop as long as sensor 4's
wall trigger is set. It then checks other sensors wall trigger flags to move accordingly. Each run the function makes
sure sensor 4 wall trigger is still set. Checks other sensors wall trigger flags. Moves motors accordingly. Reads
all sensors triggered again to update sonar#t values. Determines if the wall is avoided by seeing if new sonar# reading
is still within the range that triggered it.
********************************************************************************************************************/
void sensor_4_triggered(void){
	reset_duty_and_stop_motors();
	
	while(four == 0){/*All of these sensor scenarios will only happen if ss4 detected a wall 
	also becuase it is the middle sensor and must be triggered in order to check for a person.*/
		if((two == 0) && (six == 0) && (seven == 0)){//line 15
			turn_left();//starts turning motors left.
			/*only reads sensors whose flags were set and updates their sonar#t value to be checked again*/
			check_if_anything_detected();
		}
		else if((one == 0) && (two == 0) && (six == 0)){//line 16 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else if((six == 0) && (seven == 0)){//line 10 Sensor Scenarios for SAMR
			turn_left();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else if((one == 0) && (two == 0)){//line 11 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else if((two == 0) && (six == 0)){//line 14 Sensor Scenarios for SAMR
			turn_left();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else if((one == 0) || (two == 0)){//line 12 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else if((six == 0) || (seven == 0)){//line 13 Sensor Scenarios for SAMR
			turn_left();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else{//else ONLY sensor 4 detected something?
		}
		
	}//end while ss4 detects a wall
	
	reset_duty_and_stop_motors();
	
}// end sensor_4_triggered


/********************************************************************************************************************


********************************************************************************************************************/
void sensor_1_and_7_triggered(void){
	reset_duty_and_stop_motors();
	
	/*These scenarios will only happen if ss1 and ss7 detected a wall. This is because
	if the robot detects something on both sides it needs to be careful going between 
	them or completely turn around and avoid it.*/
	while((one == 0) && (seven == 0)){
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
		if((two == 0) && (six == 0)){//line 20 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else if((two == 0)){//line 19 Sensor Scenarios for SAMR
			turn_right();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else if((six == 0)){//line 18 Sensor Scenarios for SAMR
			turn_left();
			read_all_sensors_single_triggered();
			check_if_anything_detected();
		}
		else{//only sensor 1 & 7 detected something?
		}
		
	}//end if ss1 & ss7 detected a wall	

	reset_duty_and_stop_motors();
	
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
void reset_duty_and_stop_motors(void){
	Stop_Motors();//stop motors to avoid / human detect
	dutyl = 3800;//reset duty cycles to 3800, which is "0" point.
	dutyr = 3800;	
}








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