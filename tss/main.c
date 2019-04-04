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

	int srnum, srvalue, srint, numberinms, LED;
	char srchar;
	//unsigned int ss1, ss2, ss3, ss4, ss5, ss6, ss7;
	unsigned int sonar1t, sonar2t, sonar3t, sonar4t, sonar5t, sonar6t, sonar7t;
	int rc, j = 0, i = 0, allsensorsread = 0, start_at_1 = 1, array_position;
	int srss1 = 1, srss2 = 1, srss3 = 1, srss4 = 1, srss5 = 1, srss6 = 1, srss7 = 1;//active low (srss# = serial read sonar sensor #
	 //these are arrays used to take average of readings of sensors for more accurate reading.
	unsigned int sonar1[2], sonar2[2], sonar3[2], sonar4[2], sonar5[2], sonar6[2], sonar7[2];
	
	void reset_srss_flags(void);
	
	void serial_sensor_number(void);
	
	void serial_sensor_value(void);
	
	void all_sensors_read_check(void);
	
	void serial_read_not_accurate(void);

/**************************************************START MAIN FUNCTION**************************************************/
int main() {

	init();
	WriteLed(0x0);//turn all LEDs off.		
	printroscoe();//print rOSCOE on displays
	Stop_Motors();//start motors at 3,800 (off)
	

	 
	for(;;) {
		usleep( 100*1000 );
		i = 0;
		do{

			rc = ReadSerial(SONAR,&srchar);//reads serial from SONAR and puts in srchar
			srint = (int)srchar;

			if(rc){
				
				if(srint < 8){//if srint less than 8 it is a sensor
					serial_sensor_number();
//printf("sonar%dt[%d] = ",srint,array_position);
				}//end if sring < 8
				
				else{//srint > 8 so is a sensor reading.
					serial_sensor_value();
				}//end else srint > 8
				
				i = i + 1;
				
			}//end if( (rc) && (start_at_1 == true) )	


		}while(i < 15);//14 readings to get number and value for each sensor. so 14*# readings want.
		
		// printf("SS 1: %d [in]\r\n\n",sonar1t);
		// printf("SS 2: %d [in]\r\n\n",sonar2t);
		// printf("SS 3: %d [in]\r\n\n",sonar3t);
		// printf("SS 4: %d [in]\r\n\n",sonar4t);
		// printf("SS 5: %d [in]\r\n\n",sonar5t);
		// printf("SS 6: %d [in]\r\n\n",sonar6t);
		// printf("SS 7: %d [in]\r\n\n",sonar7t);	
		// printf("GOT AVERAGE OF ALL SENSORS\n\n");	


				LED = 0;
//sensor 1 (left corner)
		if((sonar1t < 24) && (sonar1t >= 8)){//if first sonar sensor reads less than 2 feet
			printf("SS 1 triggered: %d [in]\r\n\n",sonar1t);
			LED = LED + 0x1;}//turn on first LEDR0}
		else{}
//sensor 2 (left middle)
		if((sonar2t < 24) && (sonar2t >= 8)){//if second sonar sensor reads less than 2 feet
			printf("SS 2 triggered: %d [in]\r\n\n",sonar2t);
			LED = LED + 0x2;}
		else{}
//sensor 3 (left floor)
		if((sonar3t < 24) && (sonar3t >= 8)){
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


	}//end for;;


return(0);}
/******************************************************************************************************************************************/
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

/******************************************************************************************************************************************/
	
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

}

/******************************************************************************************************************************************/

void serial_sensor_value(void){
	
	if(srss1 == 0){/*if srss1 flag was set that meant the reading before was a 1 and this tells the loop
	that the next reading is the distance reading for sensor 1.*/
		//sonar1[array_position] = srint;//save the sensor reading in array.
		sonar1t = srint;
		reset_srss_flags();//reset flag for sonar reading.
		//all_sensors_read_check();
	}
	else if(srss2 == 0){
		//sonar2[array_position] = srint;
		sonar2t = srint;
		reset_srss_flags();
		//all_sensors_read_check();
	}
	else if(srss3 == 0){
		//sonar3[array_position] = srint;
		sonar3t = srint;
		reset_srss_flags();
		//all_sensors_read_check();
	}
	else if(srss4 == 0){
		//sonar4[array_position] = srint;
		sonar4t = srint;
		reset_srss_flags();
		//all_sensors_read_check();
	}
	else if(srss5 == 0){
		//sonar5[array_position] = srint;
		sonar5t = srint;
		reset_srss_flags();
		//all_sensors_read_check();
	}
	else if(srss6 == 0){
		//sonar6[array_position] = srint;
		sonar6t = srint;
		reset_srss_flags();
		//all_sensors_read_check();
	}
	else if(srss7 == 0){
		//sonar7[array_position] = srint;
		sonar7t = srint;
		reset_srss_flags();
		//all_sensors_read_check();
	}
	else{}
	
}


/********************************************************************************************************************/

/***********************************************************************************************************************/
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


void serial_read_not_accurate(void){
			usleep( 100*1000 );
		array_position = 0;
		do{

			rc = ReadSerial(SONAR,&srchar);//reads serial from SONAR and puts in srchar
			srint = (int)srchar;

			if( (srint < 8) && (start_at_1 == 1)){/*This is checking to see if
			a number < 8 is read so we can start the count this way.*/
				start_at_1 = 0;//set flag (active low)
			}
			

			if( (rc) && (start_at_1 == 0) ){
				
				if(srint < 8){//if srint less than 8 it is a sensor
					serial_sensor_number();
//printf("sonar%dt[%d] = ",srint,array_position);
				}//end if sring < 8
				
				else{//srint > 8 so is a sensor reading.
					serial_sensor_value();
				}//end else srint > 8
				
				i = i + 1;
				
			}//end if( (rc) && (start_at_1 == true) )	


		}while(i < 14);//14 readings to get number and value for each sensor. so 14*# readings want.
		i = 0;
		
		sonar1t = Average_Reading(sonar1);
		sonar2t = Average_Reading(sonar2);
		sonar3t = Average_Reading(sonar3);	
		sonar4t = Average_Reading(sonar4);
		sonar5t = Average_Reading(sonar5);
		sonar6t = Average_Reading(sonar6);
		sonar7t = Average_Reading(sonar7);

		printf("SS 1: %d [in]\r\n\n",sonar1t);
		printf("SS 2: %d [in]\r\n\n",sonar2t);
		printf("SS 3: %d [in]\r\n\n",sonar3t);
		printf("SS 4: %d [in]\r\n\n",sonar4t);
		printf("SS 5: %d [in]\r\n\n",sonar5t);
		printf("SS 6: %d [in]\r\n\n",sonar6t);
		printf("SS 7: %d [in]\r\n\n",sonar7t);	
		printf("GOT AVERAGE OF ALL SENSORS\n\n");	

		LED = 0;
//sensor 1 (left corner)
		if((sonar1t < 24) && (sonar1t >= 8)){//if first sonar sensor reads less than 2 feet
			printf("SS 1 triggered: %d [in]\r\n\n",sonar1t);
			LED = LED + 0x1;}//turn on first LEDR0}
		else{}
//sensor 2 (left middle)
		if((sonar2t < 24) && (sonar2t >= 8)){//if second sonar sensor reads less than 2 feet
			printf("SS 2 triggered: %d [in]\r\n\n",sonar2t);
			LED = LED + 0x2;}
		else{}
//sensor 3 (left floor)
		if((sonar3t < 24) && (sonar3t >= 8)){
			printf("SS 3 triggered: %d [in]\r\n\n",sonar3t);
			LED = LED + 0x4;}
		else{}
//sensor 4 (middle middle)
		if((sonar4t < 24) && (sonar4t >= 8)){
			printf("SS 4 triggered: %d [in]\r\n\n",sonar4t);
			LED = LED + 0x10;}
		else{}
//sensor 5 (right floor)
		if((sonar5t < 24) && (sonar5t >= 8)){
			printf("SS 5 triggered: %d [in]\r\n\n",sonar5t);
			LED = LED + 0x20;}
		else{}
//sensor 6 (right middle)
		if((sonar6t < 24) && (sonar6t >= 8)){
			printf("SS 6 triggered: %d [in]\r\n\n",sonar6t);
			LED = LED + 0x40;}
		else{}
//sensor 7 (right corner)
		if((sonar7t < 24) && (sonar7t >= 8)){
			printf("SS 7 triggered: %d [in]\r\n\n",sonar7t);
			LED = LED + 0x80;}
		else{}
		WriteLed(LED);
	
}









