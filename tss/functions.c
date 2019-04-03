#include "functions.h"
#include "samr_driver.h"

/********************************************************************************************************************/
unsigned int Average_Reading(unsigned int sensor[]){
	int k = 0;
	unsigned int average = 0;
	
	for(k = 0; k < 100; ++k){
		//printf("SS 1: %d [in]\r\n\n",sensor[k]);
		average = average + sensor[k];
		sensor[k] = 0;
	}
	
	average = average / 100;
	return average;
}
/********************************************************************************************************************
Calling this function will stop both motors.
*/
void Stop_Motors(void){
	WritePWM(PWMl,STOP_DUTY);
	WritePWM(PWMr,STOP_DUTY);
}
/********************************************************************************************************************
Calling this function will make both motors go forward at a safe speed.
*/
void Forward_Motors(void){
	WritePWM(PWMl,FORWARD_DUTY);
	WritePWM(PWMr,FORWARD_DUTY);
}
/********************************************************************************************************************/
unsigned int letter2segment(char letter){
	unsigned int display;
	switch(letter){
		case 'A':
			display = 0x8;
		break;
		
		case 'b':
			display = 0x60;
		break;
		
		case 'C':
			display = 0x46;
		break;
		
		case 'c':
			display = 0x27;
		break;
		
		case 'd':
			display = 0x21;
		break;
		
		case 'E':
			display = 0x6;
		break;
		
		case 'F':
			display = 0x38;
		break;
		
		case 'G':
			display = 0x21;
		break;
		
		case 'H':
			display = 0x9;
		break;
		
		case 'h':
			display = 0x68;
		break;
		
		case 'I':
			display = 0x79;
		break;
		
		case 'J':
			display = 0x43;
		break;
		
		case 'L':
			display = 0x47;
		break;
		
		case 'n':
			display = 0x6A;
		break;
		
		case 'O':
			display = 0x40;
		break;
		
		case 'o':
			display = 0x62;
		break;
		
		case 'P':
			display = 0x18;
		break;
		
		case 'q':
			display = 0xC;
		break;
		
		case 'r':
			display = 0x2F;		
		break;
		
		case 'S':
			display = 0x12;
		break;
		
		case 't':
			display = 0x70;
		break;
		
		case 'U':
			display = 0x41;
		break;
		
		case 'u':
			display = 0x63;
		break;
		
		case 'y':
			display = 0x44;
		break;
		
		case '1':
			display = 0x79;
		break;
		
		case '2':
			display = 0x24;
		break;
		
		case '3':
			display = 0x30;
		break;
		
		case '4':
			display = 0x19;
		break;
		
		case '5':
			display = 0x12;
		break;
		
		case '6':
			display = 0x2;
		break;
		
		case '7':
			display = 0x7;
		break;
		
		case '8':
			display = 0x0;
		break;
		
		case '9':
			display = 0x10;
		break;
		
		case 'z':
			display = 0x7F;	//DISPLAY OFF	
		break;
		default:
			display = 0x7F;
		break;
	}
	return display;
/*  //Example of using function to display HELLO
	WriteHex(HEX0,letter2segment('H'));
	WriteHex(HEX1,letter2segment('E'));
	WriteHex(HEX2,letter2segment('L'));
	WriteHex(HEX3,letter2segment('L'));
	WriteHex(HEX4,letter2segment('O'));
	WriteHex(HEX5,letter2segment('z'));
*/
}
/********************************************************************************************************************/
void printhello(void){
	// WriteHex(HEX0,0x9);
	// WriteHex(HEX1,0x6);
	// WriteHex(HEX2,0x47);
	// WriteHex(HEX3,0x47);
	// WriteHex(HEX4,0x40);
	// WriteHex(HEX5,0x7F);
	WriteHex(HEX0,letter2segment('H'));
	WriteHex(HEX1,letter2segment('E'));
	WriteHex(HEX2,letter2segment('L'));
	WriteHex(HEX3,letter2segment('L'));
	WriteHex(HEX4,letter2segment('O'));
	WriteHex(HEX5,letter2segment('z'));
}
/********************************************************************************************************************/
void printroscoe(void){
	// WriteHex(HEX0,0x9);
	// WriteHex(HEX1,0x6);
	// WriteHex(HEX2,0x47);
	// WriteHex(HEX3,0x47);
	// WriteHex(HEX4,0x40);
	// WriteHex(HEX5,0x7F);
	WriteHex(HEX0,letter2segment('r'));
	WriteHex(HEX1,letter2segment('O'));
	WriteHex(HEX2,letter2segment('S'));
	WriteHex(HEX3,letter2segment('C'));
	WriteHex(HEX4,letter2segment('O'));
	WriteHex(HEX5,letter2segment('E'));
}

/********************************************************************************************************************/






