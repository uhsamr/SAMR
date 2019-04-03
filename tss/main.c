/*
Team: Shasta Advertiser Mobile Robot (SAMR)
Sponsor: Roscoe Furgeson (Ferguson Control Systems)
Members: Brandon Kain, Matt Metoyer, Jaime Juarez & Matt Aguiano
Semesters: Fall 2018 - Spring 2019
Capstone Conference: 4/26/2019
*/
#include "samr_driver.h"
#include "functions.h"

	int srnum, srvalue, srint;
	char srchar;
	//unsigned int ss1, ss2, ss3, ss4, ss5, ss6, ss7;
	unsigned int sonar1t, sonar2t, sonar3t, sonar4t, sonar5t, sonar6t, sonar7t;
	int rc;
	int srss1 = 1, srss2 = 1, srss3 = 1, srss4 = 1, srss5 = 1, srss6 = 1, srss7 = 1;//active low (srss# = serial read sonar sensor #
	void reset_srss_flags(void);

/**************************************************START MAIN FUNCTION**************************************************/
int main() {

	init();
	WriteLed(0x0);//turn all LEDs off.		
	printroscoe();//print rOSCOE on displays
	Stop_Motors();//start motors at 3,800 (off)

	 
	for(;;) {

		usleep( 100*1000 );

		do{

			rc = ReadSerial(SONAR,&srchar);//reads serial from SONAR and puts in srchar
			srint = (int)srchar;
				
			if(rc){
				if(srint < 8){//if srint less than 8 it is a sensor
					if(srint == 1){//if sensor 1 set srss1 flag so we know next reading is for sensor 1.
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
					//srnum = srint;
				}
				else{
					if(srss1 == 0){//if sensor 1 set srss1 flag so we know next reading is for sensor 1.
						sonar1t = srint;
					}
					else if(srss2 == 0){
						sonar2t = srint;
					}
					else if(srss3 == 0){
						sonar3t = srint;
					}
					else if(srss4 == 0){
						sonar4t = srint;
					}
					else if(srss5 == 0){
						sonar5t = srint;
					}
					else if(srss6 == 0){
						sonar6t = srint;
					}
					else if(srss7 == 0){
						sonar7t = srint;
					}
					else{}
					reset_srss_flags();
				}
				
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
				
				
				// else{//if more than 8 print sensor value read
					// if((srint > 8) && (srint < 24)){
						// srvalue = srint;
						// printf("Sensor %d:	%d [in]\n\n",srnum, srvalue);
					// }
				// }
				
			}//end if rc == 1	
		}while(rc);




	}//end for;;


return(0);}

void reset_srss_flags(void){
	srss1 = 1;
	srss2 = 1;
	srss3 = 1;
	srss4 = 1;
	srss5 = 1;
	srss6 = 1;
	srss7 = 1;
}
/***************************************************END MAIN FUNCTION***************************************************/

