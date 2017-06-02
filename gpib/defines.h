/**
 @file gpib/defines.h

 @brief GPIB, AMIGO, SS80 and device defines.

 @par Edit History - [1.0]   [Mike Gore]  Initial revision of file.

 @par Copyright &copy; 2014 Anders Gustafsson All rights reserved..

 @par Copyright &copy; 2014 Mike Gore, Inc. All rights reserved.

*/

#ifndef _DEFINES_H
#define _DEFINES_H


#include "user_config.h"



// =============================================
///@brief Power on defaults  address and PPR for SS80 AMIGO and PRINTER
/// Can be changed if specified in user config
///@see ss80.c 

///@brief Default SS80 address 
#define SS80_DEFAULT_ADDRESS 0        /* SS80 default address */

///@brief Default SS80 Parallel Poll Response bit
/// 0 here is bit 8 on the BUS
#define SS80_DEFAULT_PPR 0            /* SS80 default PPR BIT */

#ifdef AMIGO
///@brief Default AMIGO address 
#define AMIGO_DEFAULT_ADDRESS 1       /* AMIGO default address */

///@brief Default AMIGO Parallel Poll Response bit 
/// 1 here is bit 7 on the BUS
#define AMIGO_DEFAULT_PPR 1           /* AMIGO default PPR BIT */
#endif

///@brief Default PRINTER address 
#define PRINTER_DEFAULT_ADDRESS 2     /* PRINTER default address */
///@brief printer do not use parallel poll

// =============================================


#define ABORT_FLAG 1  /*< user abort */
#define MEDIA_FLAG 2  /*< missing media */

#define HP9121D     //< HP9121 dual 270K AMIGO floppy drive
#define HP9134L     //< HP9134L 40M SS/80 Winchester drive

/// =========================================================
///@brief  GPIB defines provided from Anders Gustafsson <anders.gustafsson@pedago.fi>
#define GTL     0x01        //<  Go to local
#define SDC     0x04        //<  Selected device clear
#define PPC     0x05        //<  Parallell poll configure
#define GET     0x08        //<  Group execute trigger
#define TCT     0x09        //<  Take control
#define LLO     0x11        //<  Local lockout
#define DCL     0x14        //<  Device clear
#define PPU     0x15        //<  Parallell poll unconfigure
#define SPE     0x18        //<  Serial poll enable
#define SPD     0x19        //<  Serial poll disable

#define UNL          0x3F   //<  Unlisten
#define UNT          0x5F   //<  Untalk
/// =========================================================

#define BASE_MLA     0x20   //<  Base listen address = 0 (0+0x20)
#define BASE_MTA     0x40   //<  Base talk address = 0 (0+0x40)
#define BASE_MSA     0x60   //<  Base seconday address = 0 (0+0x60)

#define ERR_READ   0b00000001 	//< Read Error
#define ERR_WRITE  0b00000010	//< Write Error 
#define ERR_SEEK   0b00000100	//< Seek Error
#define ERR_WP     0b00001000	//< Write Protect Error
#define ERR_DISK   0b00010000	//< Disk Error
#define ERR_GPIB   0b00100000	//<GPIB Error

// =============================================
///@brief Fault bit and Message type
typedef struct
{
    int index;
    char *msg;
} fault_t;

// =============================================
/// LIF formating structures
///@see format.c
///@brief LIF disk label record
typedef struct
{
    uint16_t LIFid;
    uint16_t label[6];
    uint16_t dirstarthi;
    uint16_t dirstartlo;
    uint16_t s3000;
    uint16_t dummy;
    uint16_t dirlenhi;
    uint16_t dirlenlo;
    uint16_t version;
} VolumeLabelType;

///@brief LIF directory entry
typedef struct
{
    char filename[10];
    uint16_t filetype;
    uint16_t startaddhi;
    uint16_t startaddlo;
    uint16_t lengthhi;
    uint16_t lengthlo;
    char createtime[6];    //< BCD digits
    uint16_t volnumber;
    uint16_t implementationhi;
    uint16_t implementationlo;
} DirEntryType;
// =============================================
#endif     // #ifndef _DEFINES_H
