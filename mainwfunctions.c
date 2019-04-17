/*****************Roscoe Information*******************************************
 2 Serial Devices and Updated Driver to Support. Also, provided example in main.c. 
 The two Serial Devices are on GPIO_1:
 GPIO pins on FPGA:
 .rs232_0_external_interface_RXD(GPIO_1[0]),
 .rs232_0_external_interface_TXD(GPIO_1[1]),
 .rs232_1_external_interface_RXD(GPIO_1[2]),
 .rs232_1_external_interface_TXD(GPIO_1[3]),
******************************************************************************/

/******************SABERTOOTH MDC INFO****************************************
 Sabertooth MDC operates in Mode 1: Analog Mode. 
 Normal Sabertooth operating voltages: 0-2.5V commands REVERSE motion, 2.5V-5V commands FOWARD motion. 2.5V commands STOP.
 Output voltage of FPGA/MSP board is only 0-3.3V, so we will wire the motors backwards.
 Allows full duty cycle available for foward motion
 0V output commands full speed foward motion, 2.5V commands STOP still. 3.3 V commands our full backwards motion

******************************************************************************/




#include "samr_driver.h"
#include "functions.h"
#include "autonomous.h"




int remoteForward(int);
int remoteBack(int);
int remoteLeft(int, int);
int remoteRight(int, int);


int main() {


  int dutyl = 0, dutyr = 0, recvChar;
  int dutysetForward = 0;
  int dutysetBack = 0; 
  int dutysetLeft_leftwheelofthesamrrobotwow = 0;
  int dutysetLeft_rightwheelofthesamrrobotwow = 0;
  int dutysetRight_leftwheelofthesamrrobotwow = 0;
  int dutysetRight_rightwheelofthesamrrobotwow = 0;
  char d;

  init();									


 /****************************************************************************
	 USE SER1 TO READ DATA.
	 
	 RX pin to TX of BLE				TX pin to RX of BLE
	 GPIO1 top left pin 				GPIO1 top right pin

	 TO SET DUTY CYCLE:
	 WritePWM(PWM#, ACTIVATE_PWM + ####)
	 
	 For Foward Command: add 1 to 3799. 1 for full speed forward, 3799 for crawl.
	 For Backward Command: add 3801 to 5000. 3801 for crawl, 4999 for full speed back.
	 
	 Left motor: PWMl					Right motor: PWMr
	 GPIO0 bottom left pin				GPIO0 bottom right pin
 ***************************************************************************/
 
/* 
Initialize with motors stopped.
*/

  WritePWM(PWMl, STOP_DUTY);	//Right motor STOP
  WritePWM(PWMr, STOP_DUTY);	//Left motor STOP
  
  

 while (1) {										

	 recvChar = ReadSerial(SER1, &d);	//assign data from RX serial pin to holder 'd'.
	 
	 printf("recvChar: %d \n d: %c\n", recvChar, d);   //'d' holds the character sent from device. 

if(recvChar){

	 switch (d) {				
	 
	case 'F': //FORWARD
			dutyl = 3800;
			dutysetForward = 2800;						//change duty cycle with this value. 
				if(dutyl == dutysetForward){  				//If F has already been sent, stay at that speed
					dutyl = dutyr;
					WritePWM(PWMl, ACTIVATE_PWM + dutyl);   
					WritePWM(PWMr, ACTIVATE_PWM + dutyr);	
				}
				else if(dutyl != dutysetForward){	
					remoteForward(dutysetForward);
				}
			dutyl = dutysetForward;
	 break;
		 
		 
	 case 'B': //BACK	
			dutyl = 3800;
			dutysetBack = 4800;							//change duty cycle with this value.
				if(dutyl == dutysetBack){					//If B has already been sent, stay at that speed
					dutyl = dutyr;
					WritePWM(PWMl, ACTIVATE_PWM + dutyl);   
					WritePWM(PWMr, ACTIVATE_PWM + dutyr);	
				}
				else if (dutyl != dutysetBack){
					remoteBack(dutysetBack);
				}
			dutyl = dutysetBack;
	 break;
		 
		 
	 case 'L': //LEFT
			dutyl = 3800;
			dutysetLeft_leftwheelofthesamrrobotwow = 4400;
			dutysetLeft_rightwheelofthesamrrobotwow = 3200;
				if(dutyl == dutysetLeft_leftwheelofthesamrrobotwow){
					dutyl = dutysetLeft_leftwheelofthesamrrobotwow;
					dutyr = dutysetLeft_rightwheelofthesamrrobotwow;
					WritePWM(PWMl, ACTIVATE_PWM + dutyl);  
					WritePWM(PWMr, ACTIVATE_PWM + dutyr);	
				}
				else if(dutyl != dutysetLeft_leftwheelofthesamrrobotwow){
					remoteLeft(dutysetLeft_leftwheelofthesamrrobotwow, dutysetLeft_rightwheelofthesamrrobotwow);
				}
			dutyl = dutysetLeft_leftwheelofthesamrrobotwow;
		 break;
		 
		 
	 case 'R': //RIGHT
			dutyl = 3450;
			dutysetRight_leftwheelofthesamrrobotwow = 3200;
			dutysetRight_rightwheelofthesamrrobotwow = 4400;
				if(dutyl == dutysetRight_leftwheelofthesamrrobotwow){
					dutyl = dutysetRight_leftwheelofthesamrrobotwow;
					dutyr = dutysetRight_rightwheelofthesamrrobotwow;
					WritePWM(PWMl, ACTIVATE_PWM + dutyl);   //Left motor backward
					WritePWM(PWMr, ACTIVATE_PWM + dutyr);	//Right motor backward
				}
				else if(dutyl != dutysetRight_leftwheelofthesamrrobotwow){
					remoteRight(dutysetRight_leftwheelofthesamrrobotwow, dutysetRight_rightwheelofthesamrrobotwow);
		 }                                                                                                                                   
		 break;
		 
	 case 'A': 
	 
		//printf("inside case 'A'\n\n");
		autonomous();
		break;


	 case 'n': //STOP
		 printf("not really Jay");
		 WritePWM(PWMl, STOP_DUTY);
		 WritePWM(PWMr, STOP_DUTY);
		 dutyl = HAULT;
		 dutyr = HAULT;
		 break;
		
					} //closes switch case
	 
				} //closes if statement checking serial read
			
			printf("\n");
			
		} //closes infinite while

	return( 0 );

} //closes main



/*********************************************************************************************************************
Function declarations
*********************************************************************************************************************/

int remoteForward(int a)
{
	int dutyl = 3800, dutyr = 3800;
	
	WritePWM(PWMl, ACTIVATE_PWM + dutyl);  //begin with a temporary stop 
	WritePWM(PWMr, ACTIVATE_PWM + dutyr);
	
	do{
			dutyl = dutyl - 1;
			dutyr = dutyl;
			WritePWM(PWMl, ACTIVATE_PWM + dutyl);   //Left motor INCREMENT foward
			WritePWM(PWMr, ACTIVATE_PWM + dutyr);	//Right motor INCREMENT foward
		} while(dutyl != a);
		
	dutyl = a;
	dutyr = a;
	 printf("FUCK JAY\n");  //test
	 WritePWM(PWMl, ACTIVATE_PWM + dutyl);   //Left motor foward
	 WritePWM(PWMr, ACTIVATE_PWM + dutyr);	//Right motor foward
		 
		 
	 dutyl = a;
	
 	return a;
}

int remoteBack(int a)
{
	int dutyl = 3800, dutyr = 3800;
	
	WritePWM(PWMl, ACTIVATE_PWM + dutyl);  
	WritePWM(PWMr, ACTIVATE_PWM + dutyr);
	
	do
	{
			dutyl = dutyl + 1;
			dutyr = dutyl;
			WritePWM(PWMl, ACTIVATE_PWM + dutyl);   //Left motor INCREMENT back
			WritePWM(PWMr, ACTIVATE_PWM + dutyr);	//Right motor INCREMENT back
	}while(dutyl != a);
		
	dutyl = a;
	dutyr = a;
	 printf("FUCK JAY\n");  //test
	 WritePWM(PWMl, ACTIVATE_PWM + dutyl);   
	 WritePWM(PWMr, ACTIVATE_PWM + dutyr);	
		 
		 
	 dutyl = a;
	
 	return a;
	
}

int remoteLeft(int a, int b)
{
	int dutyl = 3800, dutyr = 3800;
	
	WritePWM(PWMl, ACTIVATE_PWM + dutyl);  
	WritePWM(PWMr, ACTIVATE_PWM + dutyr);
	
	do
	{		
			dutyl = dutyl + 50;
			dutyr = dutyr - 50;
			WritePWM(PWMl, ACTIVATE_PWM + dutyl);   
			WritePWM(PWMr, ACTIVATE_PWM + dutyr);		
	}while(dutyl != a);
	
	dutyl = a;
	dutyr = b;
	 printf("LOSER JAY\n");  //test
	 WritePWM(PWMl, ACTIVATE_PWM + dutyl);  
	 WritePWM(PWMr, ACTIVATE_PWM + dutyr);	
	
	return a;
}

int remoteRight(int a, int b)
{
	int dutyl = 3800, dutyr = 3800;
	
	WritePWM(PWMl, ACTIVATE_PWM + dutyl);  
	WritePWM(PWMr, ACTIVATE_PWM + dutyr);
	
	do
	{		
			dutyl = dutyl - 50;
			dutyr = dutyr + 50;
			WritePWM(PWMl, ACTIVATE_PWM + dutyl);   
			WritePWM(PWMr, ACTIVATE_PWM + dutyr);		
	}while(dutyl != a);
	
	dutyl = a;
	dutyr = b;
	 printf("Right-wing JAY\n");  //test
	 WritePWM(PWMl, ACTIVATE_PWM + dutyl);  
	 WritePWM(PWMr, ACTIVATE_PWM + dutyr);	
	
	return a;
}
