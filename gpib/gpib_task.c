/**
 @file gpib/gpib_task.c

 @brief High level GPIB command handler for HP85 disk emulator project for AVR.

 @par Edit History
 - [1.0]   [Mike Gore]  Initial revision of file.

 @par Copyright &copy; 2014-2020 Mike Gore, All rights reserved. GPL
 @see http://github.com/magore/hp85disk
 @see http://github.com/magore/hp85disk/COPYRIGHT.md for Copyright details

*/

#include "user_config.h"

#include "defines.h"
#include "drives.h"
#include "gpib_hal.h"
#include "gpib.h"
#include "gpib_task.h"
#include "amigo.h"
#include "ss80.h"
#include "vector.h"
#include "printer.h"
#include "debug.h"

/// @brief Config file name
char cfgfile[] = "/hpdisk.cfg";
FIL  fp_file;

/// @brief Debug flag - used to log GPIB and emulator messages
int debuglevel = 1;

/// @brief GPIB log file handel
FILE *gpib_log_fp = NULL;

///@brief Read Configuration File
void gpib_file_init()
{
    int errors;

    debuglevel = 0;

    errors = Read_Config(cfgfile);
    if(errors > 0)
        printf("%s had %d errors\n", cfgfile, errors);
    if(errors < 0)
        printf("%s open failure\n", cfgfile);

///@brief set any compile time defaults - but only those NOT already set by the config file
    set_Config_Defaults();

    if(mmc_wp_status())
        printf("Card is write protected\n");
}


/// @brief  Log GPIB transactions
///
/// @param str: message to log
/// @return  void
void gpib_log( char *str )
{
    if(gpib_log_fp != NULL )
        fprintf(gpib_log_fp,"%s\n", str);
}


/// @brief  Check if SS80 listening address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int SS80_is_MLA(int address)
{
    int index = find_device(SS80_TYPE, address, BASE_MLA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}


/// @brief  Check if SS80 talking address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int SS80_is_MTA(int address)
{
    int index = find_device(SS80_TYPE, address, BASE_MTA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}


/// @brief  Check if SS80 secondary address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int SS80_is_MSA(int address)
{
    int index = find_device(SS80_TYPE, address, BASE_MSA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}


#ifdef AMIGO
/// @brief  Check if AMIGO listening address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int AMIGO_is_MLA(int address)
{
    int index = find_device(AMIGO_TYPE, address, BASE_MLA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}


/// @brief  Check if AMIGO talking address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int AMIGO_is_MTA(int address)
{
    int index = find_device(AMIGO_TYPE, address, BASE_MTA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}


/// @brief  Check if AMIGO secondary address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int AMIGO_is_MSA(int address)
{
    int index = find_device(AMIGO_TYPE, address, BASE_MSA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}
#endif                                            // #ifdef AMIGO

/// @brief  Check if PRINTER listening address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int PRINTER_is_MLA(int address)
{
    int index = find_device(PRINTER_TYPE, address, BASE_MLA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}


/// @brief  Check if PRINTER talking address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int PRINTER_is_MTA(int address)
{
    int index = find_device(PRINTER_TYPE, address, BASE_MTA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}


/// @brief  Check if PRINTER secondary address
///
/// - Used at power up, Bus IFC or user aborts
/// @return  1 if true or 0
int PRINTER_is_MSA(int address)
{
    int index = find_device(PRINTER_TYPE, address, BASE_MSA);
    if(index == -1)
        return(0);
    return(set_active_device(index));
}


/// @brief  Trace GPIB activity passively - saving to a log file
/// @param[in] name: file name to save to
/// @param[in] detail: if non-zero do full handshake trace
///
/// - We use the natural GPIB handshaking to slow GPIB talker/listeners
/// so we can keep up
/// - A keypress will exit the trace and close the file
///
/// @param name: File name to save log file to.
/// @return  void
///   Exit on Key Press

void gpib_trace_task( char *name , int detail)
{
    int ch;

    if(name && *name)
    {
        name = skipspaces(name);
        printf("Capturing GPIB BUS to:%s\n", name);
        if(detail)
            printf("FULL GPIB BUS handshake logging requested\n");
        printf("Press ANY key to exit\n");

        gpib_log_fp = fopen(name,"wb");
        if(gpib_log_fp == NULL)
        {
            perror("open failed");
            printf("exiting...\n");
            return;
        }
    }
    else
    {
        gpib_log_fp = stdout;
    }

    // gpib_state_init();                            // Init PPR talking and listening states
	// gpib_state_init is in gpib_init_devices()
    gpib_init_devices();

    gpib_decode_header(gpib_log_fp);
    while(1)                                      // Main loop, forever
    {
        if(uart_keyhit(0))
            break;

        ch = gpib_read_byte(detail);
        if(!detail)
            gpib_decode(ch);
    }

    printf("Done\n");
    if(gpib_log_fp)
    {
        fclose(gpib_log_fp);
        printf("Capturing Closed\n");
        gpib_log_fp = stdout;
    }
}


/// @brief Check for GPIB errors and timeouts
///
/// - Reset GPIB bus on IFC or user keypress
/// - Display debug messages if debuglevel permits.
/// - Check media insert state
///
/// @param val: command or data byte and control or error flags.
///   - Lower 8 bits: Data or Command.
///     - If ATN is LOW then we strip parity from the byte.
///   - Upper 8 bits: Status and Errors.
///     - @see gpib.h _FLAGS defines for a full list.
///     - An error implies the data byte can't be trusted
///     - Control Line Flags.
///       - EOI_FLAG
///       - SRQ_FLAG
///       - ATN_FLAG
///       - REN_FLAG
///       - PP_FLAG
///     - Error Flags:
///       - IFC_FLAG
///  - EOI_FLAG
///  - ATN_FLAG
/// @see gpib.h _FLAGS defines for a full list.
///
/// @return  val
/// @return ABORT flag on user keypress

uint16_t gpib_error_test(uint16_t val)
{

    if(val & ERROR_MASK || uart_keyhit(0) || mmc_ins_status() != 1 )
    {
        val &= ERROR_MASK;

///@brief IFC is and important state so display it for most messages
        if(debuglevel & (GPIB_ERR + GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_TOP_LEVEL_BUS_DECODE + GPIB_DEVICE_STATE_MESSAGES))
        {
/// Bus Clear, reseat all states, etc
            if(val & IFC_FLAG)
                printf("<IFC>\n");
        }

        if(debuglevel & (GPIB_ERR + GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_TOP_LEVEL_BUS_DECODE + GPIB_DEVICE_STATE_MESSAGES))
        {
            if(val & TIMEOUT_FLAG)
                printf("<TIMEOUT>\n");
            if(val & BUS_ERROR_FLAG)
                printf("<BUS>\n");
        }

        if(uart_keyhit(0))
            printf("<INTERRUPT>\n");

        if( mmc_ins_status() != 1 )
            printf("<MEDIA MISSING>\n");

        if(val & IFC_FLAG)
        {
            gpib_init_devices();
        }

        if(uart_keyhit(0))
        {
            gpib_init_devices();
            return(ABORT_FLAG);
        }

/// @todo  do we want to always exit here ?
///  low level GPIB functions are still useful even without a DISK
        if( mmc_ins_status() != 1 )
        {
            return(ABORT_FLAG);
        }

// Wait for IFC free
        if(val & IFC_FLAG )
        {
            while(GPIB_IO_RD(IFC) == 0)
                ;
        }
        return(val);
    }
    return(0);
}


/// @brief  Initialize ALL emulated devices SS80, AMIGO and printer
///
/// - Used at power up, Bus IFC or user aborts
/// @return  void
void gpib_init_devices(void)
{
    gpib_bus_init();                              // Not busy - we have to free all pins on the BUS

// Init PPR talking and listening states
// Enable this 14 April 2020 - testing MIke Gore
    gpib_state_init();

    SS80_init();                                  // SS80 state init

#ifdef AMIGO
    amigo_init();                                 // AMIGO state init
#endif

    printer_close();                              // Close any open fprinter files
}


/// @brief  Process all GPIB Secondary Commands
///
/// - Dispatch emulator handler based on address
/// - if unread is true we "unread" val so the corresponding device
/// emulator can process it.
/// - Unread allow us to write each emulator as a pure function.
///
/// @param[in] val: GPIB secondary command and control flags.
/// @param[in] unread: if 1 unread val prior to emulator call.
///
/// @return GPIB status from emulator command.
/// @return 0 if no emulator command processed.

uint16_t GPIB_COMMANDS(uint16_t val, uint8_t unread)
{
    uint16_t status;

///@brief talking ?
    if(talking != UNT)
    {

#ifdef AMIGO
        if ( AMIGO_is_MLA(listening) )
        {
            if(unread)
                gpib_unread(val);
// secondary was previously set
            status = AMIGO_COMMANDS(secondary);
            secondary = 0;
            return(status);
        }
#endif                                    // ifdef AMIGO

        if ( SS80_is_MLA(listening) )
        {
            if(unread)
                gpib_unread(val);
// secondary was previously set
            status = SS80_COMMANDS(secondary);
            secondary = 0;
            return(status);
        }

        if ( PRINTER_is_MLA(listening) )
        {
            if(unread)
                gpib_unread(val);
            status = PRINTER_COMMANDS(secondary);
            secondary = 0;
            return(status);
        }
    }

///@brief listening ?
    if(listening != UNL)
    {
#ifdef AMIGO
        if ( AMIGO_is_MTA(talking) )
        {
            if(unread)
                gpib_unread(val);
            status = AMIGO_COMMANDS(secondary);
            secondary = 0;
            return(status);
        }
#endif

        if ( SS80_is_MTA(talking) )
        {
            if(unread)
                gpib_unread(val);
            status = SS80_COMMANDS(secondary);
            secondary = 0;
            return(status);
        }

        if ( PRINTER_is_MTA(talking) )
        {
            if(unread)
                gpib_unread(val);
            status = PRINTER_COMMANDS(secondary);
            secondary = 0;
            return(status);
        }
    }
    return(0);
}


/// @brief  Top most main GPIB device emulator task.
/// This is main() for GPIB state machine loop
/// All tasks are dispatched from here
///
/// - Initializes BUS, Devices, State.
/// - Reads and processes GPIB control or data bytes
/// and calls emulator task.
/// @return  void

void gpib_task(void)
{
    uint16_t val;
    uint8_t ch;
    uint16_t status;

    gpib_init_devices();                          // Init devices
	// gpib_state_init() is now done in gpib_init_devices
    // gpib_state_init();                            // Init PPR talking and listening states

    gpib_log_fp = NULL;

    while(1)
    {


        val = gpib_read_byte(NO_TRACE);

#if SDEBUG
        if(debuglevel & GPIB_TOP_LEVEL_BUS_DECODE)
            gpib_decode(val);
#endif
        status = gpib_error_test(val);
        if(status & ABORT_FLAG)
        {
            return;
        }
        else if(status & MEDIA_FLAG)
        {
            return;
        }
        else if(status)
        {
            continue;
        }
///@brief GPIB commands with ATN set (COMMANDS)
        if(val & ATN_FLAG)
        {
            ch = val & CMD_MASK;
            if(ch <= 0x1f)
            {
                GPIB(ch);
                continue;
            }
///@brief GPIB listen
            if(ch >= 0x20 && ch <= 0x3f)
            {
                GPIB_LISTEN(ch);
                continue;
            }
///@brief GPIB talk
            if(ch >= 0x40 && ch <= 0x5f)
            {
                GPIB_TALK(ch);
                continue;
            }

///@brief GPIB secondary
/// Note: We know ch >= 0x60 && ch <= 0x7f because of previous tests

            if( listening && lastcmd == UNT)
            {
                secondary = 0;
                GPIB_SECONDARY_ADDRESS(ch);
                continue;
            }

///@brief We have to keep track of secondary address that may happen out of order with older AMIGO protocol
/// this method works for SS80 as well
            secondary = ch;
///@brief GPIB_COMMANDS does most of the work
            status = GPIB_COMMANDS(secondary,0);
            status = gpib_error_test(status);
            if(status & ( ABORT_FLAG | MEDIA_FLAG ))
            {
                return;
            }
            if(status)
            {
                continue;
            }
            continue;

        }                                         // GPIB ATN

///@brief GPIB commands without ATN set (DATA)
        else                                      // GPIB Data
        {
            if ( PRINTER_is_MLA(listening) )
            {
                printer_buffer( 0xff & val );
                continue;
            }

            if(!secondary)
                continue;

// ONLY secondar commands at this point
///@brief GPIB_COMMANDS does most of the work
            status = GPIB_COMMANDS(val,1);
            status = gpib_error_test(status);

            if(status & ( ABORT_FLAG | MEDIA_FLAG ))
            {
                return;
            }
            if(status)
            {
                continue;
            }
            continue;
        }
    }                                             // while(1)
    return;
}


/// @brief  Send drive identify- 2 bytes
///
/// - References
///  - Transparent Command IDENT
///  - SS80 4-31
///  - CS80 pg 4-27, 3-10
///  - A11
///
/// @param[in] ch: channel
///
/// @return  0 on GPIB error returns error flags
/// @see gpib.h ERROR_MASK for a full list.

int Send_Identify(uint8_t ch, uint16_t ID)
{
    uint16_t status = EOI_FLAG;
    uint8_t tmp[2];

    V2B_MSB(tmp,0,2,ID);
    if(gpib_write_str(tmp,2, &status) != 2)
    {
        if(debuglevel & (GPIB_ERR + GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[IDENT Unit:%02XH=%04XH FAILED]\n",
                (int)ch,(int)ID);
        return(status & ERROR_MASK);
    }
#if SDEBUG
    if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
        printf("[IDENT Unit:%02XH=%04XH]\n", (int)ch,(int)ID);
#endif
    return (status & ERROR_MASK);
}


/// @brief  Main GPIB command handler
/// Commands 0x00 .. 0x1f
/// - We are only ever called with ATN=1 commands.
/// - ch = GPIB command and status (ie it has the ATN_FLAG set).
/// - Notes: We track any device states and call handelers.
///
/// @param[in] ch 8 bit command
///
/// @return  0 on sucess
/// @return GPIB error flags on fail
/// @see gpib.h ERROR_MASK defines for a full list.

int GPIB(uint8_t ch)
{
///@brief Parallel Poll Configure
///TODO
    if(ch == PPC)
    {
        if(debuglevel & (GPIB_ERR + GPIB_PPR + GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_TODO))
            printf("[PPC unsupported TODO]\n");
        spoll = 0;
        return 0;
    }
///@brief Parallel Poll Unconfigure
///TODO
    if(ch == PPU)
    {
        if(debuglevel & (GPIB_ERR + GPIB_PPR + GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_TODO))
            printf("[PPU unsupported TODO]\n");
        spoll = 0;
        return 0;
    }

/// @todo FIXME
#if defined(SPOLL)
///@brief Serial Poll Enable
    if(ch == SPE)
    {
#if SDEBUG
        if(debuglevel & (GPIB_PPR + GPIB_BUS_OR_CMD_BYTE_MESSAGES))
            printf("[SPE]\n");
#endif
        spoll = 1;
        if(SS80_is_MTA(talking))
        {
            return( SS80_Report() );
        }
        return 0;
    }

///@brief Serial Poll Disable
    if(ch == SPD)
    {
#if SDEBUG
        if(debuglevel & (GPIB_PPR + GPIB_BUS_OR_CMD_BYTE_MESSAGES))
            printf("[SPD]\n");
#endif
        spoll = 0;
        return 0;
    }
#endif                                        // #if defined(SPOLL)

///@brief Selected Device Clear
    if(ch == SDC )
    {
#if SDEBUG
        if(debuglevel & GPIB_BUS_OR_CMD_BYTE_MESSAGES)
            printf("[SDC]\n");
#endif
        if(SS80_is_MLA(listening))
        {
///  Note: Suposed to be unsupported in SS80 - pg 4-2
///  CS80 3-4
#if SDEBUG
            if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
                printf("[SDC SS80]\n");
#endif
            return(SS80_Selected_Device_Clear(SS80s->unitNO) );
        }

#ifdef AMIGO
        if(AMIGO_is_MLA(listening))
        {
///  Note: Suposed to be unsupported in SS80 - pg 4-2
#if SDEBUG
            if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
                printf("[SDC AMIGO]\n");
#endif
            return( amigo_cmd_clear() );
        }
#endif                                    // #ifdef AMIGO

/// @todo FIXME
        return( 0 );
    }

///@brief   (Universal) Device Clear
    if(ch == DCL )
    {
#if SDEBUG
        if(debuglevel & GPIB_BUS_OR_CMD_BYTE_MESSAGES)
            printf("[DCL]\n");
#endif
        SS80_Universal_Device_Clear();

#ifdef AMIGO
        amigo_cmd_clear();
#endif

/// @todo Fixme
        printer_close();
        return( 0 );
    }

    if(debuglevel & (GPIB_ERR + GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_TODO))
        printf("[GPIB (%02XH) not defined TODO]\n", 0xff & ch);
    return(0);
}


/// @brief  Process all GPIB Listen commands
/// - We only ever called with ATN=1 commands.
///
/// @param[in] ch 8 bit listen command
///
/// @return  0

int GPIB_LISTEN(uint8_t ch)
{
    listening_last = listening;
    listening = ch;

    listen_cleanup();

///  NOTE: we must track the "addressed state" of each device
///  so we can determine its state - ie: command vs Secondary states
///@brief  Universal Unlisten
    if(ch == UNL)
    {
        listening = 0;

#if SDEBUG
        if(debuglevel & GPIB_BUS_OR_CMD_BYTE_MESSAGES)
        {
            printf("[UNL]\n");
///@brief add a line break if we both Untalk and Unlisten
            if(lastcmd == UNT)
                printf("\n");
        }
#endif
        return(0);
    }

#ifdef AMIGO
    if(AMIGO_is_MLA(ch))
    {
#if SDEBUG
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[LA %02XH AMIGO]\n", 0xff & ch);
#endif
        return(0);
    }
#endif                                        // #ifdef AMIGO

    if(SS80_is_MLA(ch))
    {
#if SDEBUG
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[LA %02XH SS80]\n", 0xff & ch);
#endif
        return(0);
    }

    if(PRINTER_is_MLA(ch))
    {
#if SDEBUG
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[LA %02XH PRINTER]\n", 0xff & ch);
#endif
        if(talking != UNT)
        {
///@brief NULL creates a file named based on date and time
// FIXME use printer structure to permit multiple printers
            printer_open(NULL);
        }
        return(0);
    }
#if SDEBUG
    if(debuglevel & GPIB_BUS_OR_CMD_BYTE_MESSAGES)
        printf("[LA %02XH]\n", 0xff & ch);
#endif
    return(0);
}                                                 // Listen Primary Address group


/// @brief  Process all GPIB Talk commands
/// - We only ever called with ATN=1 commands.
///
/// @param[in] ch 8 bit talk command
///
/// @return  0

int GPIB_TALK(uint8_t ch)
{

///  NOTE: we must track the "addressed state" of each device
///  so we can determine its state - ie: command vs Secondary states
///  save talking state
    talking_last = talking;
    talking = ch;

    talk_cleanup();

///@brief  Universal Untalk
    if(ch == UNT)
    {
//FIXME talking = 0 ????
#if SDEBUG
        if(debuglevel & GPIB_BUS_OR_CMD_BYTE_MESSAGES)
            printf("[UNT]\n");
#endif
        return(0);
    }

    if(SS80_is_MTA(ch))
    {
#if SDEBUG
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[TA %02XH SS80]\n", 0xff & ch);
#endif

        if (spoll)
        {
            SS80_Report();
        }
        return(0);
    }

#ifdef AMIGO
    if(AMIGO_is_MTA(ch))
    {
#if SDEBUG
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[TA %02XH AMIGO]\n", 0xff & ch);
#endif
        return(0);
    }
#endif                                        // #ifdef AMIGO

    if(PRINTER_is_MTA(ch))
    {
#if SDEBUG
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[TA %02XH PRINTER]\n", 0xff & ch);
#endif
        return(0);
    }

    if(PRINTER_is_MLA(listening))
    {
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[PRINTER OPEN]\n");
        printer_open(NULL);
        return(0);
    }

#if SDEBUG
    if(debuglevel & GPIB_BUS_OR_CMD_BYTE_MESSAGES)
        printf("[TA %02XH]\n", 0xff & ch);
#endif
    return(0);
}                                                 // Talk Address primary address group


/// @brief  Process all GPIB Secondary Addresses
/// - We only ever called with ATN=1 commands.
/// Secondary Address, talking == UNT
///
/// @param[in] ch 8 bit secondary address.
///
/// @return  0
int GPIB_SECONDARY_ADDRESS(uint8_t ch)
{
///  NOTE: We now know that ch >= 0x60 && ch <= 0x7f
///  Previous tests ensure this fact is true and because CMD_MASK == 0x7f

///  note: any errors will reset lastcmd
///  Universal Talk mode
///  Treat this as a Secondary Address ?
///  SS80 Ident 4-31
///  If we have our secondary address then send IDENT
    if(SS80_is_MSA(ch) )
    {
#if SDEBUG
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[SA %02XH SS80]\n", 0xff & ch);
#endif
///@brief ch = secondary address
        gpib_disable_PPR(SS80p->HEADER.PPR);
        return(Send_Identify( ch, SS80p->CONFIG.ID) );

    }

#ifdef AMIGO
    if(AMIGO_is_MSA(ch) )
    {
/// @todo
///     Two identify bytes should be repeated until untalked
#if SDEBUG
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[SA %02XH AMIGO]\n", 0xff & ch);
#endif
///@brief ch = secondary address
        gpib_disable_PPR(AMIGOp->HEADER.PPR);
        return( Send_Identify( ch, AMIGOp->CONFIG.ID) );
    }
#endif                                        // #ifdef AMIGO

#if SDEBUG
    if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
        printf("[SA %02XH, listen:%02XH, talk:%02XH]\n",
            0xff & ch, 0xff & listening, 0xff & talking);
#endif
    return(0);
}


/// @brief  Called when the listen address changes.
///
/// - Used to cleanup or close at the end of any listen address transition.
/// - Also called when GPIB bus reset or unlisten.
/// For now we just use this to close printer capture
/// @return  void
void listen_cleanup()
{
    if(listening_last)
    {
        int index = find_device(PRINTER_TYPE, listening_last, BASE_MLA);
        if(index == -1)
            return;

//We should not set the active device globally
//FIXME if we have to then printer close should temprarily do so
        if(debuglevel & (GPIB_BUS_OR_CMD_BYTE_MESSAGES + GPIB_DEVICE_STATE_MESSAGES))
            printf("[PRINTER close]\n");
        printer_close();
    }
}


/// @brief  Called when the GPIB talk address changes
///
/// - Used to cleanup or close at the end of a talk transation.
/// - Also called when GPIB bus reset or untalk.
/// - Not used in this emulator.
///
/// @return  void

void talk_cleanup()
{

}


/// @brief  HEX and ASCII dump of string in human-readable format
///
/// - Used only for debugging
/// @param[in] ptr: data
/// @param[in] length: length of data string
///
/// @return  void

void DumpData(unsigned char *ptr,int length)
{
    int i,j;
    char ch;
    printf("[Dump: %d]\n",length);
    for(j=0;j<80&&(j*16<length);j++)
    {
        printf("\n");
        for(i=0;i<16 && (i+j*16<length);i++)
        {
            ch = *(ptr+i+j*16);
            printf(" %02X",ch&0xFF);
        }
        printf(" | ");
        for(i=0;i<16 && (i+j*16<length);i++)
        {
            if(*(ptr+i+j*16)>' ') printf("%c",(*(ptr+i+j*16))&0xFF);
            else printf(".");
        }
    }
    printf("\n");
}
