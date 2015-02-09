/**
 @file hardware/spi.h

 @brief SPI Driver AVR8.

 @par Edit History
 - [1.0]   [Mike Gore]  Initial revision of file.

 @par Copyright &copy; 2014 Mike Gore, Inc. All rights reserved.

*/

#ifndef _SPI_H
#define _SPI_H

#include "hardware.h"

#define SPI0_MODE0  0
#define SPI0_MODE1  1
#define SPI0_MODE2  2
#define SPI0_MODE3  3

/* spi.c */
void SPI0_Speed ( uint32_t speed );
uint32_t SPI0_Get_Speed ( void );
void SPI0_Mode ( int mode );
int SPI0_Get_Mode ( void );
void SPI0_Init ( void );
uint8_t SPI0_WriteReadByte ( uint8_t Data );

#endif
