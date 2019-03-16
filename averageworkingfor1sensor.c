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

int main() {

 //int i = 0;
 //unsigned int pw;
 //int pwgoodl, pwgoodr;
 int numberinms = 0, pushed = 0, i = 0, j = 0, k = 0;
 unsigned int switches, buttons;
 unsigned int ss1, ss2, ss3, ss4, ss5, ss6, ss7;
 unsigned int ss1t, ss2t, ss3t, ss4t, ss5t, ss6t, ss7t;
 unsigned int ss1ave, ss2ave, ss3ave, ss4ave, ss5ave, ss6ave, ss7ave;
 int one = 0, two = 0, three = 0, four = 0, five = 0, six = 0, seven = 0;
 unsigned int ss1temp[100], ss2temp[100];
 unsigned int dutyl = 3800, dutyr = 3800;//start both duty cycles at off.
 bool triggered = false;
 

 init();
 WriteLed(0x0);//turn all LEDs off.
 printhello();
	
	
	for(;;){
		
		if(triggered == false){
			
			
			
			
			
		ReadPW(PW0,&ss1);
		
/*If a sensor reads something less than 24 inches away it will set the corresponding
flag by setting the number sensor it is equal to 1. If a sensor doesnt detect anything
it will set flag to 0. Flag is active high.*/
//sensor 1
		if((ss1 < 24) && (ss1 > 0)){//if first sensor less than 2 feet.
			//printf("SS 1: %d [in]\r\n\n",ss1);
			WriteLed(0x1);
			one = 1;
			triggered = true;
			printf("SS1 has been triggered\n\n");
			printf("SS 1: %d [in]\r\n\n",ss1);
			ss1t = ss1;//save current value in temp to compare to.
		}
		else{
			one = 0;
			WriteLed(0x0);
		}
		
		
		
		
		
		}//end triggered == false
		else{//if triggered == true meaning a sensor was triggered.
			
		// wait function where numberinms is how many ms to wait.
		numberinms = 5;
		usleep( numberinms*1000 );
			
			
			
			
		if(one == 1){
			// for(i = 0; i < 100; ++i){//save 100 readings of sensor in array
				// ss1temp[i] = ReadPW(PW0,&ss1);
			// }
	
			if(i < 100){
				++i;
				ReadPW(PW0,&ss1);
				ss1temp[i] = ss1;
				//printf("SS 1: %d [in]\r\n\n",ss1);
			}
			else{
				ss1ave = Average_Reading(ss1temp);//get average of 100 readings
				printf("Trigger Value = %d [in] vs. Average Value: %d [in]\r\n\n",ss1t,ss1ave);
				if(ss1t == ss1ave){/*if the average is equal to the trigger value, it is probably a wall*/
					printf("SS1 is detecting a wall, keep moving.\n\n");
					triggered = false;//this will take it out of this loop and flags will be set to current.
				}
				else{/*if the average and trigger values are not same, it is probably a moving person or object
moving around so stay put and read an array of values again.*/
					printf("SS1 is detecting a moving object, stay put.\n\n");
					i = 0;
				}
			}
			
			
		}
		else{//sensor 1 was not triggered within 24 inches.
		}
		
		
		
		
		
		}//end if triggered == true
	}//end for(;;)
return(0);}




	// wait function where numberinms is how many ms to wait.
	// numberinms = 5000;
	// usleep( numberinms*1000 );	
	
	// for(;;){//turns on corresponding LED when switch is flipped on.
	// WriteLed(ReadSwitches());
	// }














