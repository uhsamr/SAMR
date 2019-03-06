//*****************************************************************************************************************
//*
//*  Device Driver for interface from CPU to FPGA
//*
//* ***************************************************************************************************************

#include "samr_driver.h"

// File scope variables for base address and file descriptor

static void *virtual_base = 0;
static int fd = -1;

//*****************************************************************************************************************
//*  Endian Conversion Support Functions
//*****************************************************************************************************************

unsigned int swap32(unsigned int x)
{
	return (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000));
}

unsigned short swap16(unsigned short value)
{
    return (((value & 0xFF) << 8) |
            ((value & 0xFF00) >> 8));
}

//*****************************************************************************************************************
//*  Init
//*****************************************************************************************************************
//* Initialize Device Driver. Must be invoked before use of other functions
//*
//* Return: 0-Good State 1-Failure
//* ***************************************************************************************************************

int init() {

	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}

    return 0;
}

//*****************************************************************************************************************
//*  Write LED
//*****************************************************************************************************************
//* Turn LEDs on or off. 32 bit register. Use bits 9-0 of register for LED control
//*
//* bit layout 32 -> 0
//* Return: 0-Good State -1-Failure
//* ***************************************************************************************************************

int WriteLed(unsigned int value) {

	unsigned int * addr = 0;
	if(fd == -1) {
		return -1;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+LEDR_ADDR) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = value;
 	return 0;
}

//*****************************************************************************************************************
//*  Write HEX
//*****************************************************************************************************************
//* Control HEX Display. Manage 7 Segments per HEX. 32 bit register.
//*
//* bit layout 32 -> 0
//*
//* hex: selects HEX0-HEX5, Use HEX0-HEX5 as input values.
//* value: HEX Segment. Use bits 6-0 of register for HEX Segment control
//* Return: 0-Good State -1-Init Failure, -2-Invalid Hex
//* ***************************************************************************************************************

int WriteHex(unsigned int hex,unsigned int value) {

	unsigned int * addr = 0;
	unsigned int offset = 0;

	if(fd == -1) {
		return -1;
	}

	if(hex > HEX5) {
	  return -2;
	}

	switch (hex) {

	case HEX0:
	  offset = HEX0_ADDR;
	  break;
	case HEX1:
	  offset = HEX1_ADDR;
	  break;
	case HEX2:
	  offset = HEX2_ADDR;
	  break;
	case HEX3:
	  offset = HEX3_ADDR;
	  break;
	case HEX4:
	  offset = HEX4_ADDR;
	  break;
	case HEX5:
	  offset = HEX5_ADDR;
	  break;
	default:
	  return -2;
	}


    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+offset ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = value;
 	return 0;
}

//*****************************************************************************************************************
//*  Write PWM
//*****************************************************************************************************************
//* Control Duty Cycle of 10 kHz PWM. 32 bit register.
//*
//* bit layout 32 -> 0
//*
//* source: selects PWM1, PWM2, PWMl, PWMr as input values.
//* value: Register. Use bits 32 for enable, 15-0 of register for Duty Cycle
//* Return: 0-Good State -1-Init Failure, -2-Invalid Hex
//* ***************************************************************************************************************

int WritePWM(unsigned int source,unsigned int value) {

	unsigned int * addr = 0;
	unsigned int offset = 0;

	if(fd == -1) {
		return -1;
	}

	if(source > 3) {
	  return -2;
	}

	switch (source) {

	case PWM1:
	  offset = PWM1_ADDR;
	  break;
	case PWM2:
	  offset = PWM2_ADDR;
	  break;
	case PWMl:
	  offset = PWMl_ADDR;
	  break;
	case PWMr:
	  offset = PWMr_ADDR;
	  break;
	default:
	  return -2;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+offset ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = value;
 	return 0;
}

//*****************************************************************************************************************
//*  Read Switches
//*****************************************************************************************************************
//* Read Position of each of the 10 Switch Positions into a 32 Bit Register
//*
//* bit layout 32 -> 0
//*
//* Return: 32 Bit Register. Bit Positions 9-0 stores state of each switch
//* ***************************************************************************************************************

unsigned int ReadSwitches() {

	unsigned int * addr = 0;
	unsigned int value = 0;

	if(fd == -1) {
		return -1;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+SWITCH_ADDR ) & ( unsigned long)( HW_REGS_MASK ) ));
 	value = *addr;
 	return value;
}

//*****************************************************************************************************************
//*  Read Keys
//*****************************************************************************************************************
//* Read key of each of the 4 Key Positions into a 32 Bit Register. Key Press is Active Low
//*
//* bit layout 32 -> 0
//*
//* Return: 32 Bit Register. Bit Positions 3-0 stores state of each key
//* ***************************************************************************************************************

unsigned int ReadKeys() {

	unsigned int * addr = 0;
	unsigned int value = 0;

	if(fd == -1) {
		return -1;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+KEY_ADDR ) & ( unsigned long)( HW_REGS_MASK ) ));
 	value = *addr;
 	return value;
}

//*****************************************************************************************************************
//*  Read PW
//*****************************************************************************************************************
//* Read Number of On Pulses. Provides Pulse Duration. 32 bit register.
//*
//* bit layout 32 -> 0
//*
//* source: selects PW0-PM13as input values.
//* value: Store results in Register.
//* Return: 0-Good State -1-Init Failure, -2-Invalid PW
//* ***************************************************************************************************************

int ReadPW(unsigned int pw,unsigned int * value) {

	unsigned int * addr = 0;
	unsigned int offset = 0;
	float distance = 0.0;

	if(fd == -1) {
		return -1;
	}

	if(pw > PW13) {
	  return -2;
	}

	switch (pw) {

	case PW0:
	  offset = PW0_ADDR;
	  break;
	case PW1:
	  offset = PW1_ADDR;
	  break;
	case PW2:
	  offset = PW2_ADDR;
	  break;
	case PW3:
	  offset = PW3_ADDR;
	  break;
	case PW4:
	  offset = PW4_ADDR;
	  break;
	case PW5:
	  offset = PW5_ADDR;
	  break;
	case PW6:
	  offset = PW6_ADDR;
	  break;
	case PW7:
	  offset = PW7_ADDR;
	  break;
	case PW8:
	  offset = PW8_ADDR;
	  break;
	case PW9:
	  offset = PW9_ADDR;
	  break;
	case PW10:
	  offset = PW10_ADDR;
	  break;
	case PW11:
	  offset = PW11_ADDR;
	  break;
	case PW12:
	  offset = PW12_ADDR;
	  break;
	case PW13:
	  offset = PW13_ADDR;
	  break;
	default:
	  return -2;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+offset ) & ( unsigned long)( HW_REGS_MASK ) ));
    distance = ((float) *(addr)) * .02;
    *value = ((unsigned int) distance)/147;
 	return 0;
}