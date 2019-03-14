#include "functions.h"
#include "samr_driver.h"


unsigned int DC(unsigned int duty){
	duty = 5000 - duty;
	return duty;
}

/*flip SW9 up to control left motor, flip SW0 up to control right motor,
flip SW1 up to stop both motors. Press KEY3 to decrease speed, press 
KEY2 to increase speed*/
int pushed = 0;
unsigned int dutyl = 3800, dutyr = 3800, buttons, switches;//start both duty cycles at off.
void button_L_R_PWM(void){
		buttons = ReadKeys();
		switches = ReadSwitches();
		if(switches == 0x200){//SW9 up to control left motor
			if((buttons == 0x7) && (pushed == 0)){//KEY3 increases duty cycle
				dutyl = dutyl + 100;
				pushed = 1;
			}
			else if((buttons == 0xB) && (pushed == 0)){//KEY2 decreases duty cycle
				dutyl = dutyl - 100;
				pushed = 1;
			}
			else if(pushed == 1){//if either button was pressed check to see if it was released.
				if(buttons == 0xF){pushed =0;}
				else{pushed = 1;}
			}
			else{pushed = 0;}//do nothing if no buttons pressed
			if(dutyl > 5000){dutyl = 5000;}
			else if(dutyl < 0){dutyl = 0;}
			else{}
			WritePWM(PWMl,ACTIVATE_PWM + dutyl);
		}
		else if(switches == 0x1){//SW0 up to control right motor
			if((buttons == 0x7) && (pushed == 0)){//KEY3 increases duty cycle
				dutyr = dutyr + 100;
				pushed = 1;
			}
			else if((buttons == 0xB) && (pushed == 0)){//KEY2 decreases duty cycle
				dutyr = dutyr - 100;
				pushed = 1;
			}
			else if(pushed == 1){//if either button was pressed check to see if it was released.
				if(buttons == 0xF){pushed =0;}
				else{pushed = 1;}
			}
			else{pushed = 0;}//do nothing if no buttons pressed
			if(dutyr > 5000){dutyr = 5000;}
			else if(dutyr < 0){dutyr = 0;}
			else{}
			WritePWM(PWMr,ACTIVATE_PWM + dutyr);
		}
		else if(switches == 0x2){//SW1 turns both motors off
			WritePWM(PWMl,STOP_DUTY);
			WritePWM(PWMr,STOP_DUTY);
		}
		else{//no switches up leave them as is
			WritePWM(PWMl,ACTIVATE_PWM + dutyl);
			WritePWM(PWMr,ACTIVATE_PWM + dutyr);
		}	
}

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

void switch_controlled_duty_cycle(void){
	unsigned int duty;
	unsigned int switches;
	switches = ReadSwitches();
	WriteLed(ReadSwitches());
	switch(switches){
		case 0x0:
		duty = 1;
		break;
		case 0x1:
		duty = 500;
		break;
		case 0x2:
		duty = 1000;
		break;
		case 0x4:
		duty = 1500;
		break;
		case 0x8:
		duty = 2000;
		break;
		case 0x10:
		duty = 2500;
		break;
		case 0x20:
		duty = 3000;
		break;
		case 0x40:
		duty = 3500;
		break;
		case 0x80:
		duty = 4000;
		break;
		case 0x100:
		duty = 4500;
		break;
		case 0x200:
		duty = 4999;//MAX FORWARD DUTY CYCLE 3.2[V] OUTPUT
		break;
		default:
		duty = 0;
		break;
	}//end switch statement
	WritePWM(PWM1,ACTIVATE_PWM + duty);	
}

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
			display = 0x61;
		break;
		
		case 'c':
			display = 0x72;
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
			display = 0x7A;		
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