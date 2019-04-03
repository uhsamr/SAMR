/*
Team: Shasta Advertiser Mobile Robot (SAMR)
Sponsor: Roscoe Furgeson (Ferguson Control Systems)
Members: Brandon Kain, Matt Metoyer, Jaime Juarez & Matt Aguiano
Semesters: Fall 2018 - Spring 2019
Capstone Conference: 4/26/2019
*/
#include "samr_driver.h"
#include "functions.h"

	int srnum, srvalue, srint, numberinms;
	char srchar;
	//unsigned int ss1, ss2, ss3, ss4, ss5, ss6, ss7;
	unsigned int sonar1t, sonar2t, sonar3t, sonar4t, sonar5t, sonar6t, sonar7t;
	int rc, j = 0, i = 0, allsensorsread = 0;
	int srss1 = 1, srss2 = 1, srss3 = 1, srss4 = 1, srss5 = 1, srss6 = 1, srss7 = 1;//active low (srss# = serial read sonar sensor #
	 //these are arrays used to take average of readings of sensors for more accurate reading.
	unsigned int sonar1[101], sonar2[101], sonar3[101], sonar4[101], sonar5[101], sonar6[101], sonar7[101];
	
	void reset_srss_flags(void);
	
	void serial_sensor_number(void);
	
	void serial_sensor_value(void);

/**************************************************START MAIN FUNCTION**************************************************/
int main() {

	init();
	WriteLed(0x0);//turn all LEDs off.		
	printroscoe();//print rOSCOE on displays
	Stop_Motors();//start motors at 3,800 (off)

	 
	for(;;) {

		usleep( 100*1000 );

		//do{
		for(j = 0;j < 1401; ++j){/*1414 because each read will only return 1 sensor number or 1 sensor reading.
		That means 2 readings for each sensor and 7 sensors so takes 14 runs to get 1 value for each. To get 100 readings
		will take 14*100. Doing 1 extra reading incase starts at a value and not sensor number.*/

			rc = ReadSerial(SONAR,&srchar);//reads serial from SONAR and puts in srchar
			srint = (int)srchar;
				
			if(rc){
				
				if(srint < 8){//if srint less than 8 it is a sensor
				
					serial_sensor_number();
					
				}//end if sring < 8
				else{//srint > 8 so is a sensor reading.
				
					serial_sensor_value();
					
				}//end else srint > 8
				
			}//end if rc == 1	
			numberinms = 179;
			usleep( numberinms );/*numberinms itself is time in [us] to wait. We are looping 14 times as much so waiting 347[us] 14 times 
			is equivalent to waiting 2.5[ms] 100 times.*/
		}//end for 1401 loop
		/*if(i > 100){
			break;//break from while(rc) loop.}
		while(rc);*/
		i = 0;//reset index for arrays.
		reset_srss_flags();//
		//sensor 1 (left corner)
				if((sonar1t < 24) && (sonar1t > 8)){//if first sonar sensor reads less than 2 feet
					printf("SS 1 triggered: %d [in]\r\n\n",sonar1t);
					WriteLed(0x1);}//turn on first LEDR0}
				else{}
		//sensor 2 (left middle)
				if((sonar2t < 24) && (sonar2t > 8)){//if second sonar sensor reads less than 2 feet
					printf("SS 2 triggered: %d [in]\r\n\n",sonar2t);
					WriteLed(0x2);}
				else{}
		//sensor 3 (left floor)
				if((sonar3t < 24) && (sonar3t > 8)){
					printf("SS 3 triggered: %d [in]\r\n\n",sonar3t);
					WriteLed(0x4);}
				else{}
		//sensor 4 (middle middle)
				if((sonar4t < 24) && (sonar4t > 8)){
					printf("SS 4 triggered: %d [in]\r\n\n",sonar4t);
					WriteLed(0x10);}
				else{}
		//sensor 5 (right floor)
				if((sonar5t < 24) && (sonar5t > 8)){
					printf("SS 5 triggered: %d [in]\r\n\n",sonar5t);
					WriteLed(0x20);}
				else{}
		//sensor 6 (right middle)
				if((sonar6t < 24) && (sonar6t > 8)){
					printf("SS 6 triggered: %d [in]\r\n\n",sonar6t);
					WriteLed(0x40);}
				else{}
		//sensor 7 (right corner)
				if((sonar7t < 24) && (sonar7t > 8)){
					printf("SS 7 triggered: %d [in]\r\n\n",sonar7t);
					WriteLed(0x80);}
				else{}




	}//end for;;


return(0);}
/******************************************************************************************************************************************/
void reset_srss_flags(void){
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
		sonar1[i] = srint;//save the sensor reading in array.
		reset_srss_flags();//reset flag for sonar reading.
		allsensorsread = allsensorsread + 1;//add to number of sensors read for array position.
		if(allsensorsread == 7){//if all 7 read, reset number and increment array position for all.
			allsensorsread = 0;
			i = i + 1;
		}
	}
	else if(srss2 == 0){
		sonar2[i] = srint;
		reset_srss_flags();
		allsensorsread = allsensorsread + 1;
		if(allsensorsread == 7){
			allsensorsread = 0;
			i = i + 1;
		}
	}
	else if(srss3 == 0){
		sonar3[i] = srint;
		reset_srss_flags();
		allsensorsread = allsensorsread + 1;
		if(allsensorsread == 7){
			allsensorsread = 0;
			i = i + 1;
		}
	}
	else if(srss4 == 0){
		sonar4[i] = srint;
		reset_srss_flags();
		allsensorsread = allsensorsread + 1;
		if(allsensorsread == 7){
			allsensorsread = 0;
			i = i + 1;
		}
	}
	else if(srss5 == 0){
		sonar5[i] = srint;
		reset_srss_flags();
		allsensorsread = allsensorsread + 1;
		if(allsensorsread == 7){
			allsensorsread = 0;
			i = i + 1;
		}
	}
	else if(srss6 == 0){
		sonar6[i] = srint;
		reset_srss_flags();
		allsensorsread = allsensorsread + 1;
		if(allsensorsread == 7){
			allsensorsread = 0;
			i = i + 1;
		}
	}
	else if(srss7 == 0){
		sonar7[i] = srint;
		reset_srss_flags();
		allsensorsread = allsensorsread + 1;
		if(allsensorsread == 7){
			allsensorsread = 0;
			i = i + 1;
		}
	}
	else{}
	
}
	
	
/******************************************************************************************************************************************/

