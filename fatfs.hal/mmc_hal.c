/**
 @file fatfs/posix.c

 @brief Provides hardware abstraction layer to MMC.C

 @par Copyright &copy; 2014-2020 Mike Gore, All rights reserved. GPL  License
 @see http://github.com/magore/hp85disk
 @see http://github.com/magore/hp85disk/COPYRIGHT.md for specific Copyright details

 @par You are free to use this code under the terms of GPL
   please retain a copy of this notice in any code you use it in.

This is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option)
any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "user_config.h"

#ifdef AVR
#include <stdlib.h>
#endif

#include "mathio.h"

#include "time.h"
#include "timer.h"

#ifdef ESP8266
#include "hspi.h"
#endif

#include "fatfs.h"

extern DSTATUS Stat;

/// @brief MMC timeout counter in ms
uint16_t _mmc_timeout = 0;

/// @brief MMC SPI CLOCK cache
uint32_t _mmc_clock = 0;

// @brief MMC media status prescaler
uint16_t _mmc_pre = 0;

/**
 @brief 1000HZ timer task
 @return void
*/
static void mmc_task(void)
{
    if(_mmc_timeout)
        _mmc_timeout--;

// 100HZ
    if(_mmc_pre++  < 10)
        return;

    _mmc_pre = 0;

	if(!mmc_ins_status())
		set_error(1);

    mmc_disk_timerproc();
}


/// @brief  Install MMC timer task: mmc_task()
///
/// @see  mmc_task()
/// @return  void
MEMSPACE
void mmc_install_timer( void )
{
    _mmc_timeout = 0;
    if(set_timers(mmc_task,1) == -1)
        printf("MMC Clock task init failed\n");
}


/// @brief  MMC SPI setup and chip select
/// @return  void
void mmc_spi_init()
{
    chip_select_init(MMC_CS);
    mmc_slow();
}


/// @brief  MMC SPI setup and chip select
/// @return  void
void mmc_spi_begin()
{
    spi_begin(_mmc_clock,MMC_CS);
}


/// @brief  MMC SPI end and chip deselect
/// @return  void
void mmc_spi_end()
{
    spi_end(MMC_CS);
}


/// @brief  MMC set slow SPI bus speed
/// Only called when deselected
/// - Used during card detect phase
/// @return  void
void mmc_slow()
{
    _mmc_clock = MMC_SLOW;
}


/// @brief  MMC fast SPI bus speed
/// Only called when deselected
/// - Used during normal file IO phases
/// @return  void
void mmc_fast()
{
    _mmc_clock = MMC_FAST;
}


/// @brief SPI write buffer
/// @param[in] *data: transmit buffer
/// @param[in] count: number of bytes to write
/// @return  void
void mmc_spi_TX_buffer(const uint8_t *data, int count)
{
    spi_TX_buffer((uint8_t *) data,count);
}


/// @brief SPI read buffer
/// @param[in] *data: transmit buffer
/// @param[in] count: number of bytes to write
/// @return  void
void mmc_spi_RX_buffer(const uint8_t *data, int count)
{
    spi_RX_buffer((uint8_t *) data,count);
}


/// @brief SPI read and write 1 byte
/// @param[in] data: value to transmit
/// @return  uint8_t value read
uint8_t mmc_spi_TXRX(uint8_t data)
{
    return( spi_TXRX(data) );
}


/// @brief Set MMC timeout timer in Milliseconds
/// @param[in] ms: timeout in Milliseconds
/// @see mmc_test_timeout ( )
/// @return  void
MEMSPACE
void mmc_set_ms_timeout(uint16_t ms)
{
    mmc_cli();
    _mmc_timeout = ms;
    mmc_sei();
}


///@brief Wait for timeout
///@return 1 ready
///@return 0 timeout
MEMSPACE
int  mmc_test_timeout()
{

    if( Stat & STA_NODISK )
        return(1);

#ifdef ESP8266
    optimistic_yield(1000);
    wdt_reset();
#endif
    if(!_mmc_timeout)
    {
        printf("MMC TIMEOUT\n");
        Stat |= (STA_NODISK | STA_NOINIT);
        return(1);
    }
    return(0);
}


/// @brief Wait for time in milliseconds
/// @param[in] ms: timeout in Milliseconds
MEMSPACE
void mmc_ms_wait(int ms)
{
    mmc_set_ms_timeout(ms);
    while(!mmc_test_timeout())
        ;
}


/// @brief has the MMC interface been initialized yet ?
static int mmc_init_flag = 0;
/// @brief Initialize MMC and FatFs interface, display diagnostics.
///
/// @param[in] verbose: display initialisation messages
/// @return
MEMSPACE
int mmc_init(int verbose)
{
    int rc;

    char buff[MAX_NAME_LEN+1];

    Stat = 0;
	clear_error();

	// Card Detect
	GPIO_PIN_LATCH_LOW(GPIO_C7);
	GPIO_PIN_DIR_IN(GPIO_C7);

    mmc_spi_init();

    if( verbose)
    {
        printf("START MMC INIT\n");
    }
// we only install timers once!
    if(!mmc_init_flag)
        mmc_install_timer();

	if(!mmc_ins_status())
	{
		printf("*** MMC Card NOT Inserted! ***\n");
		return(RES_NOTRDY);	
	}


    if( verbose)
    {
		printf("MMC Card Inserted\n");

#if defined (FF_USE_LFN)
        printf("LFN Enabled");
#else
        printf("LFN Disabled");
#endif
        printf(", Code page: %u\n", FF_CODE_PAGE);
    }

    rc = disk_initialize(DEV_MMC);                // aliased to mmc_disk_initialize()

    if( rc != RES_OK  )
        put_rc(rc);

    if( rc == RES_OK)
        rc = f_mount(&Fatfs[0],"/", 0);

    if( rc != RES_OK || verbose)
        put_rc( rc );

    if (verbose )
    {
        DWORD blksize = 0;
        if(rc == RES_OK)
        {
            rc = disk_ioctl ( DEV_MMC, GET_BLOCK_SIZE, (void *) &blksize);
            if( rc != RES_OK)
            {
                put_rc( rc );
                printf("MMC Block Size - read failed\n");
            }
            else
            {
                printf("MMC Block Size: %ld\n", blksize);
            }
            if( rc == RES_OK)
            {
                strcpy(buff,"/");
                fatfs_status(buff);
            }
        }
        printf("END MMC INIT\n");
    }
    mmc_init_flag = 1;

	if(rc != RES_OK)
		set_error(1);

    return( rc ) ;
}


/// @brief  MMC Power ON
///
/// @return void
MEMSPACE
void mmc_power_on()
{
}


/// @brief  MMC Power OFF
///
/// @return void
MEMSPACE
void mmc_power_off()
{
}


/// @brief  MMC Card Inserted status
///
/// - We do not detect card insert status in this project.
/// @return 1 card inserted

MEMSPACE
int mmc_ins_status()
{
	// HI when card is NOT inserted
	if( GPIO_PIN_RD(GPIO_C7) )
		return (0);
	else
		return(1);;	
}


/// @brief  MMC Card Write Protect status
///
/// - We do not detect card write protect status in this project.
/// @return 0 == not write protected

MEMSPACE
int mmc_wp_status()
{
    return (0);
}
