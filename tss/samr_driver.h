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
#define SER1 0
#define SER2 1
#define SONAR 2
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
//*  Serial Write
//*****************************************************************************************************************
//* Write Data to Serial Port
//*
//*
//* source: selects SER1 or SER2 input values.
//* data: data to write
//* Return: 0-Good State -1-Init Failure, -2-Invalid Source
//* ***************************************************************************************************************

int WriteSerial(unsigned int source,char * data,int sz);

//*****************************************************************************************************************
//*  Serial Read
//*****************************************************************************************************************
//* Read Data from Serial
//*
//* Read Byte from Serial Stream
//* source: selects SER1, SER2 or SONAR input values.
//* Return: 1-New Data  0-No Data
//* ***************************************************************************************************************

int ReadSerial(unsigned int source,char * data);

//*****************************************************************************************************************
//*  Write Relator RAM
//*****************************************************************************************************************
//* Write 16 Bits to Relator RAM. Used to program relationship configuration
//*
//* raddr: address to relator RAM (16 bit word)
//* rdata: 32 bit value. Only lower 16 bits are used.
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool WriteRelatorRAM(unsigned int raddr,unsigned int rdata);

//*****************************************************************************************************************
//*  Read Relator RAM
//*****************************************************************************************************************
//* Read 16 Bits from Relator RAM.
//*
//* raddr: address to relator RAM (16 bit word)
//* rdata: 32 bit value. Only lower 16 bits are used. Data stored in rdata from read.
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool ReadRelatorRAM(unsigned int raddr,unsigned int *rdata);

//*****************************************************************************************************************
//*  Start Relator Input Frame Collection
//*****************************************************************************************************************
//* Set Relator to Collect Input from Frame
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool StartRelatorInputFrame();

//*****************************************************************************************************************
//*  End Relator Input Frame Collection
//*****************************************************************************************************************
//* Indicate End of Relator Input Frame. Relator will process inputs and produce outputs. An interrupt will
//* indicate when frame output is available.
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool EndRelatorInputFrame();

//*****************************************************************************************************************
//*  Write Relator Input Frame Data
//*****************************************************************************************************************
//* Write an input state to the Relator. Must be in Input Frame Mode. Use only lower 8 bits. Legal values are
//* 1-125
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool WriteRelatorFrameInput(unsigned int rdata);

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

bool ReadRelatorFrameOutput(unsigned int *wd1,unsigned int *wd2,unsigned int *wd3,unsigned int *wd4);

//*****************************************************************************************************************
//*  Configure Relator
//*****************************************************************************************************************
//* Configure Relator with data from Relator RAM. Relator will read RAM to configure relationships to process.
//*
//* Return: true-success  false-fail
//* ***************************************************************************************************************

bool ConfigureRelator();

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

bool ReadRelatorState(unsigned int *rdata);


#endif