#ifndef FUNCTIONS
#define FUNCTIONS
//https://en.cppreference.com/w/c/header

#include <math.h>//common mathematic functions
#include <stdlib.h>//general utilities (memory management, string conversions, random numbers, etc.)
#include <stdint.h>//fixed-width integer types
#include <stddef.h>//common macro definitions
#include <string.h>//string handling
#include <time.h>//time & date utilities
#include <signal.h>//signal handling
#include <limits.h>//sizes of basic types
#include <stdbool.h>

#include <stdio.h>      //compile: gcc TCPServer_linux.c -o TCPServer_linux
#include <sys/socket.h> //Warning: This program does not perform any error handling.
#include <netinet/in.h> //In any real program, you need to handle errors.
#include <arpa/inet.h>
#include <unistd.h>

/*		DEFINE GLOBAL VALUES		*/
#define ACTIVATE_PWM 0x80000000//this is to make bit 31 high to activate PWM.
#define STOP_DUTY 0x80000ED8//this value gives PWM output of 2.5[V] for motors to be off. 3,700 out of 5,000.
#define MAX_DUTY 0X80001387//this gives a max output for PWM of 3.2947[V]. 4,999 out of 5,000.
#define LOW_DUTY 0x80000001//this gives the lowest output for PWM of 1.15[mV].1 out of 5,000.
#define FORWARD_DUTY 0x80000D48//this gives a decent forward speed for both motors. 3,400 / 5,000
#define REVERSE_DUTY 0x80000000//
#define HAULT 3800

unsigned int Average_Reading(unsigned int sensor[], int number_readings);

void Stop_Motors(void);

void Forward_Motors(void);

void printhello(void);

void printroscoe(void);

unsigned int letter2segment(char letter);






#endif