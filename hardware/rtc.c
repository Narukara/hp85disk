/**
 @file hardware/rtc.c

 @brief DS1307 RTC Driver AVR8.

 @par Edit History
 - [1.0]   [Mike Gore]  Initial revision of file.

 @par Copyright &copy; 2014-2020 Mike Gore, Inc. All rights reserved.

*/

#include "user_config.h"
#include "time.h"


/// ================================================================================
///@brief RTC BCD conversion functions

/// @brief Convert number >= 0 and <= 99 to BCD.
///
///  - BCD format has each hex nibble has a digit 0 .. 9
///
/// @param[in] data: number to convert.
/// @return  BCD value
/// @warning we assume the number is in range.
uint8_t BINtoBCD(uint8_t data)
{
    return(  ( (data/10U) << 4 ) | (data%10U) );
}


/// @brief Convert two "digit" BCD number to binary.
///
/// @param[in] data: BCD number.
///  - number >= 0 and <= 99 to BCD
/// @return  Binary value.
uint8_t BCDtoBIN(uint8_t data)
{
    return  ( ( ( (data&0xf0U) >> 4) * 10 ) + (data&0x0fU) );
}



/// ================================================================================
///@brief RTC HAL

///@brief RTC I2C WRITE function
///@param[in]  address: DS1307 I2C address
///@param[in]  index:   DS1307 internal address index pointer
///@param[out] buf:     write buffer
///@param[in]  len:     buffer length
///
///@return 1 = OK, 0 = ERROR
int8_t i2c_rtc_write(uint8_t address, uint8_t index, uint8_t *buf, uint8_t len)
{
	int8_t retry;
	int8_t ret = 0;
	int8_t i;
	uint8_t reg[DS1307_REG_SIZE+1];

	if(len < 1 || len > DS1307_REG_SIZE)
		return(0);

	reg[0] = index; 	/// DS1307 internal Address Pointer
	for(i=0;i<len;++i)
		reg[i+1] = buf[i];

	retry = 2;
	while(retry--)
	{
		if ( i2c_fn(address, TW_WRITE, reg, len+1) )
		{
			ret = 1;
			break;
		}
        printf("i2c_rtc_write data error\n");
		ret = 0;
	}
	return(ret);
}

///@brief RTC I2C READ function
///@param[in]  address: DS1307 I2C address
///@param[in]  index:   DS1307 internal address index pointer
///@param[out] buf:     read buffer
///@param[in]  len:     buffer length
///
///@return 1 = OK, 0 = ERROR
int8_t i2c_rtc_read(uint8_t address, uint8_t index, uint8_t *buf, uint8_t len)
{
	int8_t retry;
	int8_t ret = 0;

	if(len < 1 || len > DS1307_REG_SIZE)
		return(0);

	retry = 2;
    while(retry--)
	{
		if ( i2c_fn(address, TW_WRITE, (uint8_t *) &index, sizeof(index)) )
		{
			ret = 1;
			break;
		}
        printf("i2c_rtc_read address error\n");
		ret = 0;
    }
	if(!ret)
		return(0);
	
    if ( !i2c_fn(address, TW_READ, buf,len) )
	{
        printf("i2c_rtc_read data error\n");
        return(0);
    }
	return(1);
}

///@brief RTC I2C initialization code
///
///@return void
void i2c_rtc_init()
{
	i2c_init(100000);
}


/// ================================================================================
/// @brief RTC functions

/// @brief  Set DS1307 RTC from POSIX struct tm * structure.
///
/// @param[in] t: POSIX struct tm * time to set.
///
/// @return 1 on sucess.
/// @return 0 on fail.
uint8_t rtc_write(tm_t *t)
{
    uint8_t buf[DS1307_REG_SIZE];

    buf[0] =  BINtoBCD(t->tm_sec) & 0x7f;
    buf[1] =  BINtoBCD(t->tm_min) & 0x7f;
    buf[2] =  BINtoBCD(t->tm_hour) & 0x3f;
    buf[3] =  ((t->tm_wday & 7) + 1) & 0x0f;
    buf[4] =  BINtoBCD(t->tm_mday ) & 0x3f;
    buf[5] =  BINtoBCD(t->tm_mon + 1) & 0x1f;
    buf[6] =  BINtoBCD(t->tm_year - 100) & 0xff;  // 2000 = 0
    buf[7] = 0x93;                                // 32khz, out square wave

#ifdef RTC_DEBUG
    printf("rtc_write():%d, day:%d,mon:%d,hour:%d,min:%d,sec:%d, wday:%d\n",
        t->tm_year + 1900,
        t->tm_mon,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        t->tm_wday);
#endif

#ifdef RTC_DEBUG
    printf("%4x\n", t);
    int i;
    printf("rtc_write buf: ");
    for(i=0;i<7;++i)
        printf("%02x ", 0xff & buf[i]);
    printf("\n");
#endif


	if( !i2c_rtc_write(DS1307, 0, buf, sizeof(buf)) )
	{
        printf("rtc_write error\n");
        return (0);
    }

    return(1);
}




/// @brief  Read DS1307 RTC into POSIX struct tm * structure.
///
/// @param[out] t: struct tm * POSIX time returned.
/// @return  1 on sucess.
/// @return 0 on fail.
uint8_t rtc_read(tm_t *t)
{
    uint8_t buf[DS1307_REG_SIZE];

	if( !i2c_rtc_read(DS1307, 0, buf, sizeof(buf)) )
	{
        printf("rtc_read error\n");
        return (0);
    }

#ifdef RTC_DEBUG
    printf("%4x\n", t);
    int i;
    printf("rtc_read buf: ");
    for(i=0;i<7;++i)
        printf("%02x ", 0xff & buf[i]);
    printf("\n");
#endif

    t->tm_sec =   BCDtoBIN( buf[0] & 0x7f);
    t->tm_min =   BCDtoBIN( buf[1] & 0x7f);
    t->tm_hour =  BCDtoBIN( buf[2] & 0x3f);
    t->tm_wday =  ( buf[3] & 0x07) - 1;
    t->tm_mday =  BCDtoBIN( buf[4] & 0x3f) ;
    t->tm_mon=    BCDtoBIN( buf[5] & 0x1f) - 1;
    t->tm_year =  BCDtoBIN( buf[6] & 0xff) + 100;

#ifdef RTC_DEBUG
    printf("rtc_read():%d, day:%d,mon:%d,hour:%d,min:%d,sec:%d, wday:%d\n",
        t->tm_year + 1900,
        t->tm_mon,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        t->tm_wday);
#endif

    return 1;
}



/// @brief Set DS1307 run state.
/// @param[in] run state.
///  - 1 = run.
///  - 0 = stop.
///  - -1 = check run state.
///
/// @return  run state 0/1 on success.
/// @return -1 on error.
int rtc_run(int run)
{
	uint8_t reg;


	if( !i2c_rtc_read(DS1307, 0, (uint8_t *) &reg, sizeof(reg)) )
	{
		printf("rtc_run read error\n");
		return(-1);
	}

    if(run == -1)
        return ((reg & 0x80) ? 0 : 1);

    reg = ( reg & 0x7f) | (run ? 0 : 0x80);

	if( !i2c_rtc_write(DS1307, 0, (uint8_t *) &reg, sizeof(reg)) )
	{
        printf("rtc_run write error\n");
        return -1;
    }
    return(run);
}

/// @brief DS1307 run test
///
/// - display error if read error occurs.
///
/// @return 1 if running.
/// @return 0 if not running.
/// @return -1 ERROR
int8_t rtc_run_test()
{
	return ( rtc_run(-1) );
}


/// @brief Initialize DS1307 rtc if not initialied - or if forced.
///
/// @param[in] force: force initialiation flag.
/// - If 1 then alwasy force initialiation.
/// @param[in] seconds: POSIX EPOCH time in seconds.
///
/// @return  run state 0 = STOP, 1 = RUN
/// @return -1 = ERROR
uint8_t rtc_init (int force, time_t seconds)
{
    int8_t    state;

    tm_t *tmp;

    i2c_rtc_init();

    if(!force)
    {
        state = rtc_run(-1);
        if(state < 0)
        {
            rtc_ok = 0;
            return 0;
        }
        if(state == 0)                            // stopped
            force = 1;
    }

    if(force)                                     // INIT
    {
        if(rtc_run(0) < 0)                        // STOP RTC
        {
            rtc_ok = 0;                           // Fail
            return 0;
        }

        tmp = gmtime(&seconds);

        if(tmp == NULL || !rtc_write(tmp))
        {
            printf("rtc_init  write epoch failed\n");
            rtc_ok = 0;
            return 0;
        }

        if(rtc_run(1) < 0)                        // START RTC
        {
            rtc_ok = 0;                           // Fail
            return 0;
        }
    }
    rtc_ok = 1;
    return 1;
}



/// These are defined in my FatFS code support
/// @brief FAT time structure reference.
/// @see rtc.h
/// @see http://lxr.free-electrons.com/source/fs/fat/misc.c
/// @verbatim
/// typedef struct
/// {
///     WORD   year;                                  /* 2000..2099 */
///     BYTE   month;                                 /* 1..12 */
///     BYTE   mday;                                  /* 1.. 31 */
///     BYTE   wday;                                  /* 1..7 */
///     BYTE   hour;                                  /* 0..23 */
///     BYTE   min;                                   /* 0..59 */
///     BYTE   sec;                                   /* 0..59 */
/// } RTC;
/// @endverbatim

#if 0
/// @brief Convert Linux POSIX tm_t * to FAT32 time.
///
/// @param[in] t: POSIX struct tm * to convert.
///
/// @return  FAT32 time.
uint32_t tm_to_fat(tm_t *t)
{
    uint32_t fat;
/* Pack date and time into a uint32_t variable */
    fat = ((uint32_t)(t->tm_year - 80) << 25)
        | (((uint32_t)t->tm_mon+1) << 21)
        | (((uint32_t)t->tm_mday) << 16)
        | ((uint32_t)t->tm_hour << 11)
        | ((uint32_t)t->tm_min << 5)
        | ((uint32_t)t->tm_sec >> 1);
    return(fat);
}
#endif

#if 0
/// @brief Read DS1307 RTC and convert to FAT32 time.
///
/// @return FAT32 time.
/// @see tm_to_fat().
uint32_t get_fattime (void)
{
    tm_t t;

/* Get local time */
    if(!rtc_read(&t))
        return(0);
    return( tm_to_fat(&t) );
}
#endif
