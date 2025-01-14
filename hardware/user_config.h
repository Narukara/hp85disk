/**
 @file user_config.h

 @brief Master Include for FatFs, RTC, Timers AVR8 - Part of HP85 disk emulator.

 @par Edit History
 - [1.0]   [Mike Gore]  Initial revision of file.

 @par Copyright &copy; 2014-2020 Mike Gore, Inc. All rights reserved.

*/

#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

#define AVR 1
#define MEMSPACE                                  /**/

#define SYSTEM_TASK_HZ 1000L
#define HAVE_HIRES_TIMER 1

// FATFS
#ifdef AVR

#ifndef MMC_SLOW
#define MMC_SLOW (500000UL)
#endif

#ifndef MMC_FAST
#define MMC_FAST (5000000UL)
#endif
#endif                                            // AVR

#define NO_SCANF

#if !defined(F_CPU)
#error F_CPU undefined
#endif

#include <hardware/cpu.h>

///  standard includes
///#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>

#include <avr/pgmspace.h>
#include <avr/portpins.h>
#include <avr/io.h>
#include <assert.h>
#include <avr/interrupt.h>

#include "hardware/gpio-1284p.h"
#include "hardware/hal.h"

#include "hardware/bits.h"
#include "hardware/delay.h"
#include "hardware/ram.h"

#include <util/twi.h>

#include "lib/parsing.h"
#include "lib/stringsup.h"
#include "printf/mathio.h"

#define printf(format, args...) printf_P(PSTR(format), ##args)
#define snprintf(s, size, format, args...) snprintf_P(s, size, PSTR(format), ##args)
#define sprintf(s, format, args...) sprintf_P(s, PSTR(format), ##args)

void copyright( void );

#include "lib/time.h"
#include "lib/timer.h"
#include "lib/queue.h"
#include "hardware/rtc.h"

#include "fatfs.sup/fatfs.h"

#ifndef NULL
#define NULL        ((void *) 0)
#endif

typedef enum { false, true }
bool;

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long int size_t;
#endif

#define Mem_Clear(a) memset(a, 0, sizeof(a))
#define Mem_Set(a,b) memset(a, (int) b, sizeof(a))

#define UART_DEVICE_CNT     1                     /**< UART device number */

#include "ram.h"
#include "rs232.h"
#include "spi.h"
#include "rtc.h"
// #include "TWI_AVR8.h"

#ifdef I2C_SUPPORT
#include "i2c.h"
#endif

#ifdef LCD_SUPPORT
// #include "LCD.h"
// #include "display/lcd_printf.h"
#endif

#include "posix/posix.h"

#include "gpib/debug.h"

#ifdef PORTIO_TESTS
#include "portio.h"
#endif

// sys.c defines alternative safe functions
#ifndef free
#define free(p) safefree(p)
#endif
#ifndef calloc
#define calloc(n,s) safecalloc(n,s)
#endif
#ifndef malloc
#define malloc(s) safemalloc(s)
#endif
#endif
