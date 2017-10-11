/* common.h:
 *
 *  Common include file
 */
#ifndef __COMMON_H
#define __COMON_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "stm32f3_discovery_accelerometer.h"
#include "stm32f3_discovery_gyroscope.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
/* Baud rate for serial port */
#define UARTBAUDRATE     38400
//#define UARTBAUDRATE     9600

//#define USE_UART     1
#define USE_USB      1

#define USE_USB_INTERRUPT_DEFAULT   1

/* definitions */
#define FALSE 0
#define TRUE  1

/* I/O's */

/*
 *  Build version information
 */
extern const time_t VersionBuildDate;
extern const char VersionBuildUser[];
/* 3D magnetic sensor */

void MagneticSensorTask(void);
void Init_3DMagneticSensor(void);

/*
 *  Simple table driven parser for a command line interface.
 *
 */

#define CMD_INTERACTIVE   0
#define CMD_SHORT_HELP    1
#define CMD_LONG_HELP     2

typedef struct {
  char *cmdname;       /* String containing the name of the command */
  void (*func)(int);   /* Pointer to the action function */
  char *help;          /* Help string for the command */
} parse_table;

#define ADD_CMD(name,f,helptxt) \
const parse_table f##E __attribute__ ((section(".parsetable." name))) = { \
    .cmdname = name,  \
    .func    = f, \
    .help    = helptxt };

void TaskInput(void);
int parse(char *buf, int len,
	  const parse_table *table);  /* Parse the buffer and call commands */ 

int fetch_int32_arg(int32_t *dest);      /* Fetch an integer argument */
int fetch_uint32_arg(uint32_t *dest);  /* Fetch a UWORD argument */
int fetch_string_arg(char **dest); /* Fetch a string argument */

/*
 *  Dumping functions
 */
void DumpBuffer(uint8_t *buffer, uint32_t count, uint32_t address);
void CmdDump(int action);

/*
 * terminal.c functions 
 */


typedef enum {
#ifdef USE_UART
  INDEX_UART,
#endif
#ifdef USE_USB
  INDEX_USB,
#endif
  INDEX_MAX } PortIndex_e;


void TerminalInit(void);
uint32_t TerminalRead(uint32_t index, uint8_t *ptr, uint32_t len);
int TerminalReadNonBlock(uint32_t index,char *c);
int TerminalReadAnyNonBlock(char *c);
uint32_t TerminalInputBufferWrite(uint32_t index, char *p, uint32_t len);

/* i2c init */

void i2cInit(void);
uint32_t MangeticConverstionReady(void);
extern I2C_HandleTypeDef i2c1;
/*
 * main.c functions
 */
void Error_Handler(void);

/* mycode.s functions, and variables */
void my_Loop( void );
void my_Init( void );
void my_Tick( void );
extern volatile uint32_t myTickCount;

#endif
