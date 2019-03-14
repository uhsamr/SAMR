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
 int numberinms = 0, pushed = 0, one = 0, two = 0, three = 0;
 unsigned int ss1, ss2, ss3, ss4, ss5, ss6, ss7, switches, buttons;
 unsigned int dutyl = 3800, dutyr = 3800;//start both duty cycles at off.

 init();
	
	WriteLed(0x0);//turn all LEDs off.
	
	printhello();
	
	// wait function where numberinms is how many ms to wait.
	// numberinms = 5000;
	// usleep( numberinms*1000 );	
	
	// for(;;){//turns on corresponding LED when switch is flipped on.
	// WriteLed(ReadSwitches());
	// }
	
	WritePWM(PWMl,STOP_DUTY);
	WritePWM(PWMr,STOP_DUTY);
	
	for(;;){
		
		ReadPW(PW0,&ss1);
		ReadPW(PW1,&ss2);
		ReadPW(PW2,&ss3);
		ReadPW(PW3,&ss4);
		ReadPW(PW4,&ss5);
		ReadPW(PW5,&ss6);
		ReadPW(PW6,&ss7);

		if((ss1 < 24) && (ss1 > 0)){//if first sonar sensor reads less than 2 feet
			printf("SS 1: %d [in]\r\n\n",ss1);
			//WritePWM(PWMl,LOW_DUTY);
			WriteLed(0x3);
			one = 0;
		}//max speed forward
		else{
			//WritePWM(PWMl,STOP_DUTY);
			one = 1;
		}
		
		if((ss2 < 24) && (ss2 > 0)){//if second sonar sensor reads less than 2 feet
			//WritePWM(PWMr,LOW_DUTY);
			printf("SS 2: %d [in]\r\n\n",ss2);
			WriteLed(0x18);
			two = 0;
		}//max speed forward
		else{
			//WritePWM(PWMr,STOP_DUTY);
			two = 1;
		}
		
		if((ss3 < 24) && (ss3 > 0)){
			printf("SS 3: %d [in]\r\n\n",ss3);
			WriteLed(0x180);
			three = 0;
		}
		else{
			three = 1;
		}
		
		if((ss4 < 24) && (ss4 > 0)){
		printf("SS 4: %d [in]\r\n\n",ss4);
		}
		else{

		}
		
		if((ss5 < 24) && (ss5 > 0)){
		printf("SS 5: %d [in]\r\n\n",ss5);
		}
		else{

		}
		
		if((ss6 < 24) && (ss6 > 0)){
		printf("SS 6: %d [in]\r\n\n",ss6);
		}
		else{

		}
		
		if((ss7 < 24) && (ss7 > 0)){
		printf("SS 7: %d [in]\r\n\n",ss7);
		}
		else{

		}
		
		if((one == 0) || (two == 0) || (three == 0)){}
		else{WriteLed(0x0);}
		
		
	}//end for(;;)
return( 0 );
}



















