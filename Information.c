/*****************************************INFORMATION****************************************

- Path for executable on NUC that sends a 1 to the TCPServer running on FPGA:
C:\\Users\\uhsam\\Desktop\\EthComm\\simple-socket-examples\\TCPClient_human.exe

- Path for executable on NUC that sends a 2 to the TCPServer running on FPGA:
C:\\Users\\uhsam\\Desktop\\EthComm\\simple-socket-examples\\TCPClient_nohuman.exe

- How to run Intel RealSense Camera and TCPClients on NUC for Ethernet communication with FPGA:
Run "Examples for Intel RealSense SDK 2.0" --> Solution Explorer --> CV-examples --> dnn -->
open "rs-dnn.cpp" --> click "Local Windows Debugger"

									PuTTY Connection:
FPGA
Serial line: COM3
Speed: 115200
Connection type: Serial

FPGA-ssh
Host Name (or IP address): 192.168.1.123
Port: 22
Connection type: SSH

									View FPGA on NUC:
 Using cable: Run "PuTTY (64-bit)" --> FPGA --> click "Load" --> click "Open"
 Using Ethernet: Run "PuTTY (64-bit)" --> FPGA-ssh --> click "Load" --> click "Open"


							View / Copy / Delete Files on FPGA:
Open "Swish" --> open FPGA --> Password: FCS15#one

								Access controller for SAMR:
Open "SAMR_UI"

******************************************INFORMATION****************************************/





/* When writing to HEX, LED etc. look at them as bits:
HEX# = bit-6 bit-5 bit-4 bit-3 bit-2 bit-1 bit-0 = 0000000 to turn them all on or 1111111 to turn them all off, 
but that is for binary, so we must write a hex number or decimal equivalent to the binary bits we want
LED = bit-9 bit-8 bit-7 bit-6 bit-5 bit-4 bit-3 bit-2 bit-1 bit-0 */



/*************************************SONAR SENSOR OPERATION************************************
SONAR 1 (Left Corner):
This sensor will be pointing outward at about a 45 degree angle from the left of the robot. The objects detected
are not in the direct path of the robot, but still need to be taken into account for and avoided.
The robot wheel sticks out 6-7[in] from the sensor placement, this sensors trigger range will be
< 24[in], meaning it will detect anything 17-18[in] from the corner of the robot. When this sensor
is triggered it will stop the robot and slightly rotate to the right and continue.

SONAR 2 (Left Middle):
This sensor will be pointing forward directly in front of the robot on the left side. An object 
detected could be directly in the path, but could also be on the outsite left part of the robot.
For this reason if only this sensor is triggered we will stop and rotate slightly right.

SONAR 3 (Left Bottom):
This sensor is pointed at about a 60 degree angle down from the front left of the robot. This allows it
to see the floor about 24[in] ahead of the robot. This will be used to detect dips in the floor,
stairs, walls or anything else uneven on the floor that the robot should avoid. To do this the 
sensor should read a value within a constant range (maybe 20-28[in]) and if the sensor ever reads
outside this range it means there is a significant change in the floor reading. This sensor will tell
the robot to stop and rotate right.

SONAR 4 (Middle Middle):
This is the most important sensor reading, if this sensor detects something it is directly in front
of the robot and we must analyze and decide if a person is there looking at samr or if the robot
encountered a wall or non-moving object and must avoid it. This sensor will be reading 24[in]
ahead.

SONAR 5 (Right Bottom):
This sensor is pointed at about a 60 degree angle down from the front right of the robot. This allows it
to see the floor about 24[in] ahead of the robot. This will be used to detect dips in the floor,
stairs, walls or anything else uneven on the floor that the robot should avoid. To do this the 
sensor should read a value within a constant range (maybe 20-28[in]) and if the sensor ever reads
outside this range it means there is a significant change in the floor reading. This sensor will tell
the robot to stop and rotate left.

SONAR 6 (Right Middle):
This sensor will be pointing forward directly in front of the robot on the right side. An object 
detected could be directly in the path, but could also be on the outsite right part of the robot. 
For this reason if only this sensor is triggered we will stop and rotate slightly left.

SONAR 7 (Right Corner):
This sensor will be pointing outward at about a 45 degree angle from the right of the robot. The objects detected
are not in the direct path of the robot, but still need to be taken into account for and avoided.
The robot wheel sticks out 6-7[in] from the sensor placement, this sensors trigger range will be
< 24[in], meaning it will detect anything 17-18[in] from the corner of the robot. When this sensor
is triggered it will stop the robot and slightly rotate to the left and continue.

SONAR 1, 2, 4, 6 & 7:
- All straight pointing sensors will have a triggering range of < 24[in].
- If Sonar 4 is ever triggered the algorithm MUST stop, people check and rotate left/right depending
on other sensors that are triggered.
- If Sonar 1 and/or 2 are triggered the robot must stop and rotate right until the sensors
are no longer being triggered.
- If Sonar 6 and/or 7 are triggered the robot must stop and rotate left until the sensors
are no longer being triggered.
- If Sonar 4 and Sonar 6 and/or 7 are triggered the robot must stop, check if person or wall
detected and act accordingly. Rotate left if a wall.
- If Sonar 4 and sonar 1 and/or 2 are triggered the robot must stop, check if person or wall
detected and act accordingly. Rotate right if a wall.

SONAR 3 & 5:
- All pointing at an angle toward ground in front of robot.
- Triggering will be if sensor reads outside specified range:
- If Sonar 3 triggered, stop and rotate right.
- If Sonar 5 triggered, stop and rotate left.

*************************************SONAR SENSOR OPERATION*************************************/




/****************************************************************
PWM:

Max output: 3.2947[V] using 4,999/5,000 duty cycle.
Lowest output: 1.15[mV] using 1/5,000 duty cycle.
Off output: 2.5[V] using 3,800/5,000 duty cycle.

FORWARD DUTY RANGE: 0-2.49[V] = 0 - 3,799. LOWER NUMBER, FASTER SPEED FORWARD.
REVERSE DUTY RANGE: 2.51[V] - 3.3[V]. 3,801 - 5,000. HIGHER NUMBER, FASTER SPEED BACKWARDS.

Due to 3.3[V] output limitation, we will reverse duty cycle. 
Outputs 2.51-3.3[V] will be reverse speed
and outputs 0-2.49[V] will be forward speed. 

*****************************************************************/

/****************************************************************
7-SEGMENT DISPLAY LETTERS:
0000.0000.0000.0000.0000.0000.0XXX.XXXX
0000.0000.0000.0000.0000.0000.0654.3210
						  
    WriteHex(HEX0,0x8);		A
    WriteHex(HEX0,0x60);	b
    WriteHex(HEX0,0x61);	C
    WriteHex(HEX0,0x72);	c
	WriteHex(HEX0,0x42);	d
	WriteHex(HEX1,0x6);		E
	WriteHex(HEX0,0x38);	F
	WriteHex(HEX0,0x21);	G
	WriteHex(HEX0,0x9);		H
	WriteHex(HEX0,0x68);	h
	WriteHex(HEX0,0x79);	I/1
	WriteHex(HEX0,0x43);	J
	WriteHex(HEX2,0x47);	L
	WriteHex(HEX0,0x6A);	n
	WriteHex(HEX4,0x40);	O/0
	WriteHex(HEX0,0x62);	o
	WriteHex(HEX0,0x18);	P
	WriteHex(HEX0,0xC);		q
	WriteHex(HEX0,0x7A);	r
	WriteHex(HEX0,0x12);	S/5
	WriteHex(HEX0,0x70);	t
	WriteHex(HEX0,0x41);	U
	WriteHex(HEX0,0x63);	u
	WriteHex(HEX0,0x44);	y
	WriteHex(HEX5,0x7F); 	OFF
	WriteHex(HEX5,0x24);	2
	WriteHex(HEX5,0x30);	3
	WriteHex(HEX5,0x19);	4
	WriteHex(HEX5,0x2);		6
	WriteHex(HEX5,0x7);		7
	WriteHex(HEX5,0x0);		8	
	WriteHex(HEX5,0x10);	9
*****************************************************************/



/****************************************************************
LEDS:
0000.0000.0000.0000.0000.00XX.XXXX.XXXX
0000.0000.0000.0000.0000.0098.7654.3210
	 
0x0 = 00.0000.0000
0x1 = 00.0000.0001
0x2 = 00.0000.0010
0x4 = 00.0000.0100
0x8 = 00.0000.1000
0x10 = 00.0001.0000
0x20 = 00.0010.0000
0x40 = 00.0100.0000
0x80 = 00.1000.0000
0x100 = 01.0000.0000
0x200 = 10.0000.0000
0x3FF = 11.1111.1111//all LEDs ON.
*****************************************************************/



/****************************************************************
KEYS:
0000.0000.0000.0000.0000.0000.0000.XXXX
0000.0000.0000.0000.0000.0000.0000.3210
Buttons are active low meaning all buttons read 0 when pressed.

	0xF = 00.0000.1111//No buttons pressed
	0xE = 00.0000.1110//KEY0 pressed
	0xD = 00.0000.1101//KEY1 pressed
	0xB = 00.0000.1011//KEY2 pressed
	0x7 = 00.0000.0111//KEY3 pressed
*****************************************************************/





