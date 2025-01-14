/**
 @file gpib/printer.h

 @brief HPGL printer capture code for HP85 disk emulator project for AVR.

 @par Edit History
 - [1.0]   [Mike Gore]  Initial revision of file.

 @par Copyright &copy; 2014-2020 Mike Gore, All rights reserved. GPL
 @see http://github.com/magore/hp85disk
 @see http://github.com/magore/hp85disk/COPYRIGHT.md for Copyright details

*/

#ifndef _PRINTER_H
#define _PRINTER_H

#include <user_config.h>

/* printer.c */
void printer_open ( char *name );
void printer_init ( void );
void printer_close ( void );
int receive_plot_flush ( void );
void printer_buffer ( uint16_t val );
int PRINTER_COMMANDS ( uint8_t ch );
void plot_echo ( int gpib_address );
#endif                                            // #ifndef _PRINTER_H
