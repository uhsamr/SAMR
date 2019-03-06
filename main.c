//When writing to HEX, LED etc. look at them as bits:
/* HEX# = bit-6 bit-5 bit-4 bit-3 bit-2 bit-1 bit-0 = 0000000 to turn them all on or 1111111 to turn them all off, 
 but that is for binary, so we must write a hex number or decimal equivalent to the binary bits we want */
/* LED = bit-9 bit-8 bit-7 bit-6 bit-5 bit-4 bit-3 bit-2 bit-1 bit-0 */
#include "samr_driver.h"
#include "functions.h"

int main() {

 //int i = 0;
 //unsigned int pw;
 //int pwgoodl, pwgoodr;
 int numberinms = 0, pushed = 0;
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
	
	
	for(;;){
		button_L_R_PWM();
		
		ReadPW(PW0,&ss1);
		ReadPW(PW1,&ss2);
		// ReadPW(PW2,&ss3);
		// ReadPW(PW3,&ss4);
		// ReadPW(PW4,&ss5);
		// ReadPW(PW5,&ss6);
		// ReadPW(PW6,&ss7);
		
		printf("Sonar Sensor 1: %d [in]\r\n\n",ss1);
		printf("Sonar Sensor 2: %d [in]\r\n\n",ss2);
		// printf("Sonar Sensor 3: %d [in]\r\n\n",ss3);
		// printf("Sonar Sensor 4: %d [in]\r\n\n",ss4);
		// printf("Sonar Sensor 5: %d [in]\r\n\n",ss5);
		// printf("Sonar Sensor 6: %d [in]\r\n\n",ss6);
		// printf("Sonar Sensor 7: %d [in]\r\n\n",ss7);
	}
return( 0 );
}
