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

	if(source > PWMr) {
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
//* source: selects PW0-PM06 as input values.
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

	if(pw > PW6) {
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
	  break;
	default:
	  return -2;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+offset ) & ( unsigned long)( HW_REGS_MASK ) ));
    distance = ((float) *(addr)) * .02;
    *value = ((unsigned int) distance)/147;
 	return 0;
}

//*****************************************************************************************************************
//*  Serial Write
//*****************************************************************************************************************
//* Write Data to Serial Port
//*
//*
//* source: selects SER1 or SER2 input values.
//* data: data to write
//* Return: 0-Good State -1-Init Failure, -2-Invalid Source
//* ***************************************************************************************************************

int WriteSerial(unsigned int source,char * data,int sz) {

	unsigned int * addr = 0;
	unsigned int offset = 0;
	int i = 0;

	if(fd == -1) {
		return -1;
	}

	if(source > SER2) {
	  return -2;
	}

	switch (source) {

	case SER1:
	  offset = RS232_0_BASE;
	  break;
	case SER2:
	  offset = RS232_1_BASE;
	  break;
	default:
	  return -2;
	}


    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + offset) & ( unsigned long)( HW_REGS_MASK ) ));

    for(i=0; i < sz;i++) {

	 unsigned int reg = *data;

     *addr = reg;
     data++;
	}

  return 0;

}

//*****************************************************************************************************************
//*  Serial Read
//*****************************************************************************************************************
//* Read Data from Serial
//*
//* Read Byte from Serial Stream
//* source: selects SER1 or SER2 input values.
//* Return: 1-New Data  0-No Data
//* ***************************************************************************************************************

int ReadSerial(unsigned int source,char * data) {

	unsigned int * addr = 0;
	unsigned int offset = 0;

	if(fd == -1) {
		return 0;
	}

	if(source > SER2) {
	  return 0;
	}

	switch (source) {

	case SER1:
	  offset = RS232_0_BASE;
	  break;
	case SER2:
	  offset = RS232_1_BASE;
	  break;
	default:
	  return 0;
	}


    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + offset) & ( unsigned long)( HW_REGS_MASK ) ));


    unsigned int reg = *addr;
    unsigned int cnt = 0;

    cnt =  reg << 8;
    cnt = cnt >> 24;
    if(cnt > 0) {
	 char * byte = (char *) &reg;
     data[0] = byte[0];

     return 1;
	}

  return 0;
}

//*****************************************************************************************************************
//*  Write Relator RAM
//*****************************************************************************************************************
//* Write 16 Bits to Relator RAM. Used to program relationship configuration
//*
//* raddr: address to relator RAM (16 bit word)
//* rdata: 32 bit value. Only lower 16 bits are used.
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool WriteRelatorRAM(unsigned int raddr,unsigned int rdata) {
	unsigned int * addr = 0;
	if(fd == -1) {
		return false;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0x84 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = rdata;
   	usleep(1000);
    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0x80 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = raddr;
   	usleep(1000);

 	return true;
}

//*****************************************************************************************************************
//*  Read Relator RAM
//*****************************************************************************************************************
//* Read 16 Bits from Relator RAM.
//*
//* raddr: address to relator RAM (16 bit word)
//* rdata: 32 bit value. Only lower 16 bits are used. Data stored in rdata from read.
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool ReadRelatorRAM(unsigned int raddr,unsigned int *rdata) {
	unsigned int * addr = 0;

	if(fd == -1) {
		return false;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0x88 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = raddr;
 	usleep( 100);
    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0x8C ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*rdata = *addr;

	return true;
}

//*****************************************************************************************************************
//*  Start Relator Input Frame Collection
//*****************************************************************************************************************
//* Set Relator to Collect Input from Frame
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool StartRelatorInputFrame() {
	unsigned int * addr = 0;
	if(fd == -1) {
		return false;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0x90 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = 0x1ead2ead;
   	usleep(1000);

 	return true;
}

//*****************************************************************************************************************
//*  End Relator Input Frame Collection
//*****************************************************************************************************************
//* Indicate End of Relator Input Frame. Relator will process inputs and produce outputs. An interrupt will
//* indicate when frame output is available.
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool EndRelatorInputFrame() {
	unsigned int * addr = 0;
	if(fd == -1) {
		return false;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0x90 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = 0x3ead4ead;
   	usleep(1000);

 	return true;
}

//*****************************************************************************************************************
//*  Write Relator Input Frame Data
//*****************************************************************************************************************
//* Write an input state to the Relator. Must be in Input Frame Mode. Use only lower 8 bits. Legal values are
//* 1-125
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool WriteRelatorFrameInput(unsigned int rdata) {
	unsigned int * addr = 0;
	if(fd == -1) {
		return false;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0x94 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = rdata;
   	usleep(1000);

 	return true;
}


//*****************************************************************************************************************
//*  Read Relator Frame Output
//*****************************************************************************************************************
//* Returns the 128 bit vector from Relator. Output is broken into 4 32-bit words. Each on bit represents
//* output event based on position of bit
//*
//*  wd1 - (127 downto 96)
//*  wd2 - (95 downto 64)
//*  wd3 - (63 downto 32)
//*  wd4 - (31 downto 0)
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool ReadRelatorFrameOutput(unsigned int *wd1,unsigned int *wd2,unsigned int *wd3,unsigned int *wd4) {
	unsigned int * addr = 0;

	if(fd == -1) {
		return false;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0xAC ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*wd1 = *addr;
 	usleep( 100);

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0xA8 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*wd2 = *addr;
 	usleep( 100);

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0xA4 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*wd3 = *addr;
 	usleep( 100);

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0xA0 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*wd4 = *addr;
 	usleep( 100);


	return true;
}

//*****************************************************************************************************************
//*  Configure Relator
//*****************************************************************************************************************
//* Configure Relator with data from Relator RAM. Relator will read RAM to configure relationships to process.
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool ConfigureRelator() {
	unsigned int * addr = 0;
	if(fd == -1) {
		return false;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0x90 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*addr = 0x5ead6ead;
   	usleep(1000);

 	return true;
}

//*****************************************************************************************************************
//*  Read Relator State
//*****************************************************************************************************************
//* Relator State Register is a 32-bit Register.
//*
//* Last 3 Bits in Order:
//*  rel_program_in_progress
//*  rel_frame_input_mode
//*  rel_frame_eval_mode
//*
//* rdata: 32 bit value. Data stored in rdata from read.
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool ReadRelatorState(unsigned int *rdata) {
	unsigned int * addr = 0;

	if(fd == -1) {
		return false;
	}

    addr = (unsigned int *) (virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_1_BASE+0xB0 ) & ( unsigned long)( HW_REGS_MASK ) ));
 	*rdata = *addr;
 	usleep( 100);

	return true;
}
