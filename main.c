#include "samr_driver.h"

int main() {

 int i = 0;
 unsigned int pw;
 int pwgoodl, pwgoodr;

 init();

 for(i = 0; i < 10;i++) {
   WriteLed(i);
   usleep( 100*1000 );
 }

 for(i = 0; i < 7;i++) {
   WriteHex(HEX0,i);
   usleep( 100*1000 );
 }

 for(i = 0; i < 7;i++) {
   WriteHex(HEX1,i);
   usleep( 100*1000 );
 }

 for(i = 0; i < 7;i++) {
   WriteHex(HEX2,i);
   usleep( 100*1000 );
 }

 for(i = 0; i < 7;i++) {
   WriteHex(HEX3,i);
   usleep( 100*1000 );
 }

 for(i = 0; i < 7;i++) {
   WriteHex(HEX4,i);
   usleep( 100*1000 );
 }

 for(i = 0; i < 7;i++) {
   WriteHex(HEX5,i);
   usleep( 100*1000 );
 }

// for(;;)	{
	 // ReadPW(PW0,&pw);
	 // printf("PW reading in HEX: %x\r\n",pw);
	 // printf("PW reading in int: %x\r\n",pw);
// }
//to activate PWM bit 32 must be a 1.
	pwgoodl = WritePWM(PWMl,2147483648 + 10000);
	pwgoodr = WritePWM(PWMr,2147483648 + 5000);	
	
	 //ReadPW(PW0,&pw);
	printf("PWMl returned: %d\r\n",pwgoodl);
	printf("PWMr returned: %d\r\n",pwgoodr);
	  
	if((pwgoodl == 0) && (pwgoodr == 0)){
	printf("PWMr and PWMl are in Good State\r\n");
	}


 // for(;;) {
   // unsigned int sw = ReadSwitches();
   // unsigned int keys = ReadKeys();
   // printf("Switches: %08x, Keys: %08x \n",sw,keys);
   // usleep( 100*1000 );
 // }

return( 0 );
}
