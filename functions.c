#include "functions.h"
#include "samr_driver.h"

//********************************************************************************************************************
unsigned int Average_Reading(unsigned int sensor[]){
	int i = 0;
	unsigned int average;
	
	for(i = 0; i < 100; ++i){
		average = average + sensor[i];
	}
	
	average = average / 100;
	return average;
}
//********************************************************************************************************************
void Stop_Motors(void){
	WritePWM(PWMl,STOP_DUTY);
	WritePWM(PWMr,STOP_DUTY);
}
//********************************************************************************************************************
							int one = 0, two = 0, three = 0, four = 0, five = 0, six = 0, seven = 0;
							unsigned int ss1, ss2, ss3, ss4, ss5, ss6, ss7;
void Test_All_Sensors(void){
/*call this in a for(;;) loops to constantly check if any of the sensors
read something within 24 inches, if they do it will print which sensor,
the reading and light up the corresponding LED.*/
		ReadPW(PW0,&ss1);
		ReadPW(PW1,&ss2);
		ReadPW(PW2,&ss3);
		ReadPW(PW3,&ss4);
		ReadPW(PW4,&ss5);
		ReadPW(PW5,&ss6);
		ReadPW(PW6,&ss7);
//sensor 1
		if((ss1 < 24) && (ss1 > 0)){//if first sonar sensor reads less than 2 feet
			printf("SS 1: %d [in]\r\n\n",ss1);
			//WritePWM(PWMl,LOW_DUTY);
			WriteLed(0x1);
			one = 0;}//max speed forward
		else{
			//WritePWM(PWMl,STOP_DUTY);
			one = 1;}
//sensor 2
		if((ss2 < 24) && (ss2 > 0)){//if second sonar sensor reads less than 2 feet
			//WritePWM(PWMr,LOW_DUTY);
			printf("SS 2: %d [in]\r\n\n",ss2);
			WriteLed(0x2);
			two = 0;}//max speed forward
		else{
			//WritePWM(PWMr,STOP_DUTY);
			two = 1;}
//sensor 3
		if((ss3 < 24) && (ss3 > 0)){
			printf("SS 3: %d [in]\r\n\n",ss3);
			WriteLed(0x4);
			three = 0;}
		else{three = 1;}
//sensor 4
		if((ss4 < 24) && (ss4 > 0)){
			printf("SS 4: %d [in]\r\n\n",ss4);
			WriteLed(0x10);
			four = 0;}
		else{four = 1;}
//sensor 5
		if((ss5 < 24) && (ss5 > 0)){
			printf("SS 5: %d [in]\r\n\n",ss5);
			WriteLed(0x20);
			five = 0;}
		else{five = 1;}
//sensor 6
		if((ss6 < 24) && (ss6 > 0)){
			printf("SS 6: %d [in]\r\n\n",ss6);
			WriteLed(0x40);
			six = 0;}
		else{six = 1;}
//sensor 7
		if((ss7 < 24) && (ss7 > 0)){
			printf("SS 7: %d [in]\r\n\n",ss7);
			WriteLed(0x80);
			seven = 0;}
		else{seven = 1;}
		
		if((one == 0) || (two == 0) || (three == 0) || (four == 0) || (five == 0) || (six == 0) || (seven == 0)){}
		else{WriteLed(0x0);}
}
//********************************************************************************************************************
							int pushed = 0;
							unsigned int dutyl = 3800, dutyr = 3800, buttons, switches;//start both duty cycles at off.
void button_L_R_PWM(void){
/*flip SW9 up to control left motor, flip SW0 up to control right motor,
flip SW1 up to stop both motors. Press KEY3 to decrease speed, press 
KEY2 to increase speed*/
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
//********************************************************************************************************************
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
//********************************************************************************************************************
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
//********************************************************************************************************************
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
//********************************************************************************************************************






