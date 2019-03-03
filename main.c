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


 init();
	
	WriteLed(0x0);
    usleep( 100*1000 );
	
	printhello();
	
	for(;;){//turns on corresponding LED when switch is flipped on.
	WriteLed(ReadSwitches());}
	
return( 0 );
}
