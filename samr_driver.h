#ifndef SAMR_DRIVERS
#define SAMR_DRIVERS

//*****************************************************************************************************************
//*
//*  Device Driver for interface from CPU to FPGA
//*
//* ***************************************************************************************************************

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"
//Hardware Base Registers
#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
// Constants
#define HEX0 0
#define HEX1 1
#define HEX2 2
#define HEX3 3
#define HEX4 4
#define HEX5 5
#define PWM1 0
#define PWM2 1
#define PWMl 2
#define PWMr 3
#define PW0 0
#define PW1 1
#define PW2 2
#define PW3 3
#define PW4 4
#define PW5 5
#define PW6 6
#define PW7 7
#define PW8 8
#define PW9 9
#define PW10 10
#define PW11 11
#define PW12 12
#define PW13 13
//Register Addresses
#define LEDR_ADDR 0x4
#define HEX0_ADDR 0x8
#define HEX1_ADDR 0xc
#define HEX2_ADDR 0x10
#define HEX3_ADDR 0x14
#define HEX4_ADDR 0x18
#define HEX5_ADDR 0x1C
#define PWM1_ADDR 0x20
#define PWM2_ADDR 0x24
#define PWMl_ADDR 0x28
#define PWMr_ADDR 0x2c
#define SWITCH_ADDR 0x3c
#define KEY_ADDR 0x40
#define PW0_ADDR 0x44
#define PW1_ADDR 0x4c
#define PW2_ADDR 0x50
#define PW3_ADDR 0x54
#define PW4_ADDR 0x58
#define PW5_ADDR 0x5C
#define PW6_ADDR 0x60
#define PW7_ADDR 0x64
#define PW8_ADDR 0x68
#define PW9_ADDR 0x6C
#define PW10_ADDR 0x70
#define PW11_ADDR 0x74
#define PW12_ADDR 0x78
#define PW13_ADDR 0x7C

//*****************************************************************************************************************
//*  Init
//*****************************************************************************************************************
//* Initialize Device Driver. Must be invoked before use of other functions
//*
//* Return: 0-Good State 1-Failure
//* ***************************************************************************************************************

int init();

//*****************************************************************************************************************
//*  Write LED
//*****************************************************************************************************************
//* Turn LEDs on or off. 32 bit register. Use bits 9-0 of register for LED control
//*
//* bit layout 32 -> 0
//* Return: 0-Good State -1-Failure
//* ***************************************************************************************************************

int WriteLed(unsigned int value);

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

int WriteHex(unsigned int hex,unsigned int value);

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

int WritePWM(unsigned int source,unsigned int value);

//*****************************************************************************************************************
//*  Read Switches
//*****************************************************************************************************************
//* Read Position of each of the 10 Switch Positions into a 32 Bit Register
//*
//* bit layout 32 -> 0
//*
//* Return: 32 Bit Register. Bit Positions 9-0 stores state of each switch
//* ***************************************************************************************************************

unsigned int ReadSwitches();

//*****************************************************************************************************************
//*  Read Keys
//*****************************************************************************************************************
//* Read key of each of the 4 Key Positions into a 32 Bit Register. Key Press is Active Low
//*
//* bit layout 32 -> 0
//*
//* Return: 32 Bit Register. Bit Positions 3-0 stores state of each key
//* ***************************************************************************************************************

unsigned int ReadKeys();

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

int ReadPW(unsigned int pw,unsigned int * value);
#endif