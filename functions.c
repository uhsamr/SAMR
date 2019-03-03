#include "functions.h"
#include "samr_driver.h"

void printhello(){
	WriteHex(HEX0,0x9);
	WriteHex(HEX1,0x6);
	WriteHex(HEX2,0x47);
	WriteHex(HEX3,0x47);
	WriteHex(HEX4,0x40);
	WriteHex(HEX5,0x7F);
}