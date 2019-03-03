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


 for(;;) {
   unsigned int sw = ReadSwitches();
   unsigned int keys = ReadKeys();
   printf("Switches: %08x, Keys: %08x \n",sw,keys);
   usleep( 100*1000 );
 }

return( 0 );
}
