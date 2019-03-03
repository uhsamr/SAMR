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
0x3FF = 11.1111.1111
*****************************************************************/



/****************************************************************
KEYS:
0000.0000.0000.0000.0000.0000.0000.XXXX
0000.0000.0000.0000.0000.0000.0000.3210
	 
	0x0 = 00.0000.0000//No buttons pressed
	0x1 = 00.0000.0001//KEY0 pressed
	0x2 = 00.0000.0010//KEY1 pressed
	0x4 = 00.0000.0100//KEY2 pressed
	0x4 = 00.0000.0100//KEY3 pressed
*****************************************************************/




