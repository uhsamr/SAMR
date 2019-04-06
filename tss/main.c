/*
Team: Shasta Advertiser Mobile Robot (SAMR)
Sponsor: Roscoe Furgeson (Ferguson Control Systems)
Members: Brandon Kain, Matt Metoyer, Jaime Juarez & Matt Aguiano
Semesters: Fall 2018 - Spring 2019
Capstone Conference: 4/26/2019

				NETGEAR Router	
URL: 192.168.1.1
Username: admin
Password: FCS15#one
NUC: 192.168.1.100
FPGA: 192.168.1.123

					SWISH
Password: FCS15#one
*/
#include "samr_driver.h"
#include "functions.h"

	int srint, numberinms, LED;
	char srchar;
	int rc, j = 0, i = 0, allsensorsread = 0, start_at_1 = 1, array_position;
	unsigned int sonar1t, sonar2t, sonar3t, sonar4t, sonar5t, sonar6t, sonar7t;
	int srss1 = 1, srss2 = 1, srss3 = 1, srss4 = 1, srss5 = 1, srss6 = 1, srss7 = 1;//active low (srss# = serial read sonar sensor #
	 //these are arrays used to take average of readings of sensors for more accurate reading.
	unsigned int sonar1[2], sonar2[2], sonar3[2], sonar4[2], sonar5[2], sonar6[2], sonar7[2];
	int number_readings;
	
	
	void get_average_of_all_sensors(void);
	
	void check_if_anything_detected(void);
	
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
/**************************************************START MAIN FUNCTION**************************************************/
int main() {

	init();
	number_readings = 2;
	WriteLed(0x0);//turn all LEDs off.		
	printroscoe();//print rOSCOE on displays
	Stop_Motors();//start motors at 3,800 (off)
	 
	for(;;) {
		
		read_all_sensors_single();
		
		/*ONLY NEED THIS WHEN DOING ARRAY READING*/
		//read_all_sensors_array();	
		//get_average_of_all_sensors();

		check_if_anything_detected();

	}//end for;;

return(0);}

/********************************************************************************************************************************************
This function takes all sonar# arrays and calls Average_Reading for each sensor and stores the average of all
values in sonar#t.
********************************************************************************************************************************************/
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

/********************************************************************************************************************************************
This function takes the sonar#t values and checks if any of the sensors readings are within the specified range for
detection. If any sensor reading is in detection range then the value will be saved in ss#t, this is a trigger saving
to be compared later. If sensor detect then their corresponding trigger flag will also be set. This variable corresonds
to the number of sensor. If sensor 1 detected something then flag "one" will be set. Flags are active low (0 = set).
If any of the flags were set then this function sets "triggered" = true.
********************************************************************************************************************************************/
void check_if_anything_detected(void){
			
	LED = 0;
//sensor 1 (left corner)
	if((sonar1t < 24) && (sonar1t > 8)){//if first sonar sensor reads less than 2 feet
		printf("SS 1 triggered: %d [in]\r\n\n",sonar1t);
		LED = LED + 0x1;}//turn on first LEDR0}
	else{}
//sensor 2 (left middle)
	if((sonar2t < 24) && (sonar2t > 8)){//if second sonar sensor reads less than 2 feet
		printf("SS 2 triggered: %d [in]\r\n\n",sonar2t);
		LED = LED + 0x2;}
	else{}
//sensor 3 (left floor)
	if((sonar3t < 24) && (sonar3t > 8)){
		printf("SS 3 triggered: %d [in]\r\n\n",sonar3t);
		LED = LED + 0x4;}
	else{}
//sensor 4 (middle middle)
	if((sonar4t < 24) && (sonar4t > 8)){
		printf("SS 4 triggered: %d [in]\r\n\n",sonar4t);
		LED = LED + 0x8;}
	else{}
//sensor 5 (right floor)
	if((sonar5t < 24) && (sonar5t > 8)){
		printf("SS 5 triggered: %d [in]\r\n\n",sonar5t);
		LED = LED + 0x10;}
	else{}
//sensor 6 (right middle)
	if((sonar6t < 24) && (sonar6t > 8)){
		printf("SS 6 triggered: %d [in]\r\n\n",sonar6t);
		LED = LED + 0x20;}
	else{}
//sensor 7 (right corner)
	if((sonar7t < 24) && (sonar7t > 8)){
		printf("SS 7 triggered: %d [in]\r\n\n",sonar7t);
		LED = LED + 0x40;}
	else{}
	WriteLed(LED);
	
	
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
	else if(srint == 2)){
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








