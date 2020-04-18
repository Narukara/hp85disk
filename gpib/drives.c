/**
 @file gpib/drives.c
 @brief drive definitions for HP85 disk emulator project for AVR.
 @par Edit History
 - [1.0]   [Mike Gore]  Initial revision of file.
 @par Copyright &copy; 2014-2020 Mike Gore, All rights reserved. GPL
 @see http://github.com/magore/hp85disk
 @see http://github.com/magore/hp85disk/COPYRIGHT.md for Copyright details
*/

#include "user_config.h"

#include <stdint.h>
#include "defines.h"
#include "drives.h"
#include "gpib_hal.h"
#include "gpib.h"
#include "gpib_task.h"
#include "amigo.h"
#include "ss80.h"
#include <time.h>
#include "lifutils.h"


extern hpdir_t hpdir;

/// @brief Config Parser Stack
#define MAX_STACK 5
static int stack_ind = 0;
static int stack_p[MAX_STACK];

DeviceType Devices[MAX_DEVICES];

///@brief Active Printer Device
PRINTERDeviceType *PRINTERp = NULL;

///@brief Active SS80 Device
SS80DiskType *SS80p = NULL;
SS80StateType *SS80s = NULL;

#ifdef AMIGO
///@brief Active AMIGO Device
AMIGODiskType *AMIGOp = NULL;
AMIGOStateType *AMIGOs = NULL;
#endif

///@brief hpdir.ini file processing
hpdir_t hpdir;

#if defined (SET_DEFAULTS)
// =============================================
PRINTERDeviceType PRINTERDeviceDefault =
{
    { 
        2,      // GPIB Address
        0xff,   // PPR unused
        "/printer.txt"
    }
};

// =============================================
#if defined(HP9122D)
///@brief SS80 HP9122D Disk Definitions
SS80DiskType SS80DiskDefault =
{
    {			// HEADER
        0,          // GPIB Address
        0,          // PPR
        "/ss80.lif"			// File name
    },
    {			// CONFIG
        0x222   	// ID
    },
    {			// CONTROLLER
        0x8001,     // Installed Units 1 (15 is always on)
        100,        // GPIB data transfer rate in kB/s on the bus
        4,          // 5 = SS/80 integrated single-unit controller
    },
    {			// UNIT
        0,          // Generic Unit Type, 0 = fixed, 1 = floppy, 2 = tape
        0x00091220, // BCD Device number XX XX XY, X=Unit, Y=option
        0x100,      // Bytes per block
        1,          // Buffered Blocks
        0,          // Burst size = 0 for SS80
        2000,       // Block time in microseconds
        50,         // Continous transfer time in kB/s
        10000,      // Retry time in 0.01 secods
        10000,      // Access time in 0.01 seconds
        31,         // Maximum interleave factor
        1,          // Fixed volume byte, one bit per volume
        1           // Removable volume byte, one bit per volume
    },
    {			// VOLUME
        0,          // Maximum Cylinder  - not used
        0,          // Maximum Head      - not used
        0,          // Maximum Sector    - not used
        2463,       // Maximum Block Number
        1           // Interleave
    }
};

#endif // #if defined(HP9122D)

#if defined(HP9134D)
///@brief SS80 HP9134D Disk Definitions
SS80DiskType SS80DiskDefault =
{
    {			// HEADER
        0,          // GPIB Address
        0,          // PPR
        "/ss80.lif" // FILE name
    },
    {			    // CONFIG
        0x222       // ID
    },
    {			// CONTROLLER
        0x8001,     // Installed Units 1 (15 is always on)
        100,        // GPIB data transfer rate in kB/s on the bus
        4,          // 4 = SS/80 integrated single-unit controller
    },
    {			// UNIT
        0,          // Generic Unit Type, 0 = fixed
        0x091340,   // BCD Device number XX XX XY, X=Unit, Y=option
        0x100,      // Bytes per block
        1,          // Buffered Blocks
        0,          // Burst size = 0 for SS80
        2000,       // Block time in microseconds
        50,         // Continous transfer time in kB/s
        10000,      // Retry time in 0.01 secods
        10000,      // Access time in 0.01 seconds
        31,         // Maximum interleave factor
        1,          // Fixed volume byte, one bit per volume
        1           // Removable volume byte, one bit per volume
    },
    {			// VOLUME
        0,          // Maximum Cylinder  - not used
        0,          // Maximum Head      - not used
        0,          // Maximum Sector    - not used
        58175,      // Maximum Block Number
        1			// Interleave
    }
};
#endif // #if defined(HP9134D)

#ifdef AMIGO

#if defined(HP9121)
/// @brief  AMIGO D9121 ident Bytes per sector, sectors per track, heads, cylinders
AMIGODiskType AMIGODiskDefault =
{
    {			// HEADER
        1,          // GPIB Address
        1,          // PPR
        "/amigo.lif"    // FILE name
    },
    {			// CONFIG
        0x0104     // ID
    },
    {			// GEOMETRY
        256,        // Bytes Per Sector
        16,         // Sectors Per Track
        2,          // Sides
        35          // Cylinders
    }
};
#endif // #if defined(HP9121)

#endif // ifdef AMIGO

#endif // SET_DEFAULTS



///@brief Seach Devices[] for ANY definitions of a disk type
///@param type: disk type like SS80_TYPE
//@return Devices[] index fopr matching type
int find_type(int type)
{
    int i;
    for(i=0;i<MAX_DEVICES;++i)
    {
        if( Devices[i].TYPE == type)
            return(i);
    }
    return(-1);
}

/// @brief Count number of devices of a sertain type
///@param type: disk type like SS80_TYPE
int count_drive_types(uint8_t type)
{
    int i;
	int count = 0;
    for(i=0;i<MAX_DEVICES;++i)
    {
        if( Devices[i].TYPE == type )
			++count;
    }
	return(count);
}

///@brief Convert a disk type into a string
///@param type: disk type like SS80_TYPE
///@return string pointer
char *type_to_str(int type)
{
    if(type == NO_TYPE)
        return("NO_TYPE");
    else if(type == AMIGO_TYPE)
        return("AMIGO_TYPE");
    else if(type == SS80_TYPE)
        return("SS80_TYPE");
    else if(type == PRINTER_TYPE)
        return("PRINTER_TYPE");
    return("INVALID TYPE");
}
///@brief Convert base address into a string identifier
///@param base: BASE_MAL, BASE_MTA or BASE_MSA only
///@return string "MLA", "MTA", "MSA" or error string
char *base_to_str(int base)
{
    if(base == BASE_MLA)
        return("MLA");
    else if(base == BASE_MTA)
        return("MTA");
    else if(base == BASE_MSA)
        return("MSA");
    return("*INVALID BASE*");
}

///@brief Find first free Devices[] slot
///@return Devices[] index of free slot or -1
int find_free()
{
    return(find_type(NO_TYPE));
}

///@brief Find a device with matching type AND address
///@param type: disk type
///@param address: GPIB device address 0 based
///@param base: BASE_MLA,BASE_MTA or BASE_MSA address range
///@return index of Devices[] or -1 if not found
int find_device(int type, int address, int base)
{
    int i;

    ///@skip Only interested in device addresses
    if(address < BASE_MLA || address >(BASE_MSA+30))
        return(-1);

    ///@brief Make sure address is in expected range
    if(address < base || address > (base+30))
        return(-1);

    ///@brief convert to device address
    address -= base;

    ///@brief search for device address
    for(i=0;i<MAX_DEVICES;++i)
    {
        if(Devices[i].TYPE == type && Devices[i].ADDRESS == address)
            return(i);
    }
    return(-1);
}

///@brief Set the Active disk or device pointers
/// Since we can be called multiple times per single GPIB state we do not
/// display state changes here. Other code displays the active state.
///@param index: Devices[] index
///@return 1 on success or 0 on fail
int set_active_device(int index)
{
    int type,address;

    ///@brief We also check for -1 
    /// So the result of find_device() can be used without additional tests
    if(index == -1)
    {
        return(0);
    }

    if(index < 0 || index >= MAX_DEVICES)
    {
        if(debuglevel & 1)
            printf("set_active_device:(%d) out of range\n", index);
        return(0);
    }

    type = Devices[index].TYPE;
    address = Devices[index].ADDRESS;
    if(address < 0 || address > 30)
    {
        if(debuglevel & 1)
            printf("set_active_device: index:%d address: %02XH out of range\n", index,address);
        return(0);
    }

    if(Devices[index].dev == NULL)
    {
        if(debuglevel & 1)
            printf("set_active_device: index:%d type:%d:%s, dev == NULL\n", 
                index,type,type_to_str(type));
        return(0);
    }

    if(type == NO_TYPE)
    {
        if(debuglevel & 1)
            printf("set_active_device: index %d uninitalized type:%d:%s\n", 
                index,type,type_to_str(type));
        return(0);
    }

    if(type == PRINTER_TYPE)
    {
        PRINTERp = (PRINTERDeviceType *) Devices[index].dev;
        return(1);
    }

    if(type == AMIGO_TYPE || type == SS80_TYPE) 
    {
        if(Devices[index].state == NULL)
        {
            if(debuglevel & 1)
                printf("set_active_device: index: %d type:%d:%s, state == NULL\n",
                     index,type,type_to_str(type));
            return(0);
        }
#ifdef AMIGO
        if(type == AMIGO_TYPE)
        {
            AMIGOp = (AMIGODiskType *) Devices[index].dev;
            AMIGOs = (AMIGOStateType *) Devices[index].state;
            return(1);
        }
#endif
        if(type == SS80_TYPE)
        {
            SS80p = (SS80DiskType *) Devices[index].dev;
            SS80s = (SS80StateType *) Devices[index].state;
            return(1);
        }
    }
    if(debuglevel & 1)
        printf("set_active_device:(%d) invalid type:%d:%s\n", 
            index,type,type_to_str(type));
    return(0);
}


///@brief Set Default Values for a new SS80 Device IF defaults have been defined
/// Most values in the CONTROLER and UNIT are defaults that should not need to be specified
/// Note all of the values are zeroed on allocation including strings
///@return void
void SS80_Set_Defaults(int index)
{
	int defindex = find_type(SS80_DEFAULT_TYPE);
	SS80DiskType *SS80p = (SS80DiskType *) Devices[index].dev;
	SS80DiskType *SS80DEFAULTp;

	if(defindex < 0 )
		return;

	SS80DEFAULTp = (SS80DiskType *) Devices[defindex].dev;

	SS80p->HEADER.ADDRESS				= SS80DEFAULTp->HEADER.ADDRESS;
	SS80p->HEADER.PPR					= SS80DEFAULTp->HEADER.PPR;
	SS80p->HEADER.NAME = stralloc(SS80DEFAULTp->HEADER.NAME);

	SS80p->CONFIG.ID					= SS80DEFAULTp->CONFIG.ID;
	SS80p->CONTROLLER.UNITS_INSTALLED	= SS80DEFAULTp->CONTROLLER.UNITS_INSTALLED;
	SS80p->CONTROLLER.TRANSFER_RATE		= SS80DEFAULTp->CONTROLLER.TRANSFER_RATE;
	SS80p->CONTROLLER.TYPE				= SS80DEFAULTp->CONTROLLER.TYPE;

	SS80p->UNIT.UNIT_TYPE				= SS80DEFAULTp->UNIT.UNIT_TYPE;
	SS80p->UNIT.DEVICE_NUMBER			= SS80DEFAULTp->UNIT.DEVICE_NUMBER;
	SS80p->UNIT.BYTES_PER_BLOCK			= SS80DEFAULTp->UNIT.BYTES_PER_BLOCK;
	SS80p->UNIT.BUFFERED_BLOCKS			= SS80DEFAULTp->UNIT.BUFFERED_BLOCKS;
	SS80p->UNIT.BURST_SIZE				= SS80DEFAULTp->UNIT.BURST_SIZE;
	SS80p->UNIT.BLOCK_TIME				= SS80DEFAULTp->UNIT.BLOCK_TIME;
	SS80p->UNIT.CONTINUOUS_TRANSFER_RATE= SS80DEFAULTp->UNIT.CONTINUOUS_TRANSFER_RATE;
	SS80p->UNIT.OPTIMAL_RETRY_TIME		= SS80DEFAULTp->UNIT.OPTIMAL_RETRY_TIME;
	SS80p->UNIT.ACCESS_TIME				= SS80DEFAULTp->UNIT.ACCESS_TIME;
	SS80p->UNIT.MAXIMUM_INTERLEAVE		= SS80DEFAULTp->UNIT.MAXIMUM_INTERLEAVE;
	SS80p->UNIT.FIXED_VOLUMES			= SS80DEFAULTp->UNIT.FIXED_VOLUMES;

	SS80p->VOLUME.MAX_CYLINDER			= SS80DEFAULTp->VOLUME.MAX_CYLINDER;
	SS80p->VOLUME.MAX_HEAD				= SS80DEFAULTp->VOLUME.MAX_HEAD;
	SS80p->VOLUME.MAX_SECTOR			= SS80DEFAULTp->VOLUME.MAX_SECTOR;
	SS80p->VOLUME.MAX_BLOCK_NUMBER		= SS80DEFAULTp->VOLUME.MAX_BLOCK_NUMBER;
	SS80p->VOLUME.INTERLEAVE			= SS80DEFAULTp->VOLUME.INTERLEAVE;
};


///@brief Allocate a Device structure for a disk or printer
///@param type: disk type
///@return Devices[] index on sucess or -1
int alloc_device(int type)
{
    int ind;
    int index = -1;

    // Find a free slot
    ind = find_free();
    if(ind == -1)
    {
        if(debuglevel & 1)
            printf("alloc_device: Device table is full\n", type);
        return(ind);
    }

    switch(type)
    {
		// Same as SS80 type but sets initial defaults for any remaining SS80 drives
        case SS80_DEFAULT_TYPE:
            Devices[ind].TYPE = type;
            Devices[ind].dev = safecalloc(sizeof(SS80DiskType)+7,1);
            Devices[ind].state = safecalloc(sizeof(SS80StateType)+7,1);
            index = ind;
            break;
        case SS80_TYPE:
            Devices[ind].TYPE = type;
            Devices[ind].dev = safecalloc(sizeof(SS80DiskType)+7,1);
            Devices[ind].state = safecalloc(sizeof(SS80StateType)+7,1);
            index = ind;
			SS80_Set_Defaults(index);	// Set any defaults we may have
            break;
#ifdef AMIGO
        case AMIGO_TYPE:
            Devices[ind].TYPE = type;
            Devices[ind].dev = safecalloc(sizeof(AMIGODiskType)+7,1);
            Devices[ind].state = safecalloc(sizeof(AMIGOStateType)+7,1);
            index = ind;
            break;
#endif
        case PRINTER_TYPE:
            Devices[ind].TYPE = type;
            Devices[ind].dev = safecalloc(sizeof(PRINTERDeviceType)+7,1);
            Devices[ind].state = NULL;
            index = ind;
            break;
        default:
            if(debuglevel & 1)
                printf("alloc_device: invalid type:%d:%s\n", type,type_to_str(type));
            break;
    }
    return(index);
}

// =============================================
/// @brief Init Config Parser Stack
/// Called only durring power up so we do not have to free memory
void init_Devices()
{
    int i;
    stack_ind = 0;
    for(i=0;i<MAX_DEVICES;++i)
    {
        Devices[i].TYPE = NO_TYPE;
        Devices[i].ADDRESS = 0;
        Devices[i].PPR = 0xff;
        Devices[i].BLOCKS = 0;
        memset(Devices[i].model, 0, sizeof(Devices[i].model) );
        Devices[i].dev = NULL;
        Devices[i].state = NULL;
    }
}

/// ===============================================
/// @brief Push Parser State
/// @param state: parser state
/// @return state
int push_state(int state)
{
    if(stack_ind < MAX_STACK)
        stack_p[stack_ind++] = state;
    else
        return(START_STATE);
    return(state);
}

/// ===============================================
/// @brief Pop Parser State
/// @return state
int pop_state()
{
    if(stack_ind > 0)
        return(stack_p[--stack_ind]);
    else
        return(START_STATE);
}

///@brief Config file line number
int lines = 0;

/// ===============================================
/// @brief assigned a value
///
/// - Used only for debugging
/// @param[in] str: string to examine
/// @param[in] minval: minimum value
/// @param[in] maxval: maximum value
/// @param[in] *val: value to set
///
/// @return  1 is matched and value in range, 0 not matched or out of range
bool assign_value(char *str, uint32_t minval, uint32_t maxval, uint32_t *val)
{
    uint32_t tmp;
    int bad = 0;
    char *ptr;

    // Skip spaces before assignment
    ptr = skipspaces(str);
    // Skip optional '='
    if(*ptr == '=')
    {
        ++ptr;
        // skip spaces after assignment
        ptr = skipspaces(ptr);
    }
    if(!*ptr)
    {
        if(debuglevel & 1)
            printf("line:%d, missing value\n", lines);
        bad = 1;
    }
    if(!bad)
    {
        // FIXME detect bad numbers
        tmp = get_value(ptr);
        *val = tmp;
        if((minval && (tmp < minval)))
        {
            printf("line:%d, %s is below range %d\n", lines, ptr,(int)minval);
            bad = 1;
        }
        if((maxval != 0xffffffffUL) && (tmp > maxval))
        {
            printf("line:%d, %s is above range %d\n", lines, ptr,(int)maxval);
            bad = 1;
        }
    }
    if(bad)
        return(0);
    return(1);
}


/// ===============================================
///@brief Set Defaults for any missing disk or printer devices
/// These are only used if the Config file omits them or is empty
/// @return  void
void set_Config_Defaults()
{
#if defined(SET_DEFAULTS)
    int index;

    ///@brief Add optional hard coded devices for any that are missing
    if(find_type(SS80_TYPE) == -1)
    {
#if defined(HP9122D)
        printf("set_Config_Defaults: Using default SS/80 9122D\n");
#endif
#if defined(HP9134D)
        printf("set_Config_Defaults: Using default SS/80 9134L\n");
#endif
        index = find_free();
        if(index != -1)
        {
            Devices[index].TYPE  = SS80_TYPE;
            Devices[index].ADDRESS = SS80DiskDefault.HEADER.ADDRESS;
            Devices[index].PPR = SS80DiskDefault.HEADER.PPR;
            Devices[index].dev = (void *)&SS80DiskDefault;
            Devices[index].state = safecalloc(sizeof(SS80StateType)+7,1);
        }
    }
#ifdef AMIGO
    // Make sure we have a AMIGO defined
    if(find_type(AMIGO_TYPE) == -1)
    {
#if defined(HP9121)
        printf("set_Config_Defaults:  Using default Amigo 9121\n");
#endif
        index = find_free();
        if(index != -1)
        {
            Devices[index].TYPE  = AMIGO_TYPE;
            Devices[index].ADDRESS = AMIGODiskDefault.HEADER.ADDRESS;
            Devices[index].PPR = AMIGODiskDefault.HEADER.PPR;
            Devices[index].dev = (void *) &AMIGODiskDefault;
            Devices[index].state = safecalloc(sizeof(AMIGOStateType)+7,1);
        }
    }
#endif //#ifdef AMIGO

    // Make sure we have a PRINTER defined
    if(find_type(PRINTER_TYPE) == -1)
    {
        printf("set_Config_Defaults:  Using default PRINTER settings\n");
        index = find_free();
        if(index != -1)
        {
            Devices[index].TYPE  = PRINTER_TYPE;
            Devices[index].ADDRESS = PRINTERDeviceDefault.HEADER.ADDRESS;
            Devices[index].PPR = 0xff;
            Devices[index].dev = (void *) &PRINTERDeviceDefault;
            Devices[index].state = NULL;
        }
    }
#endif // SET_DEFAULTS

}

/// ===============================================
///@brief Set Device parameters from hpdir information
///
///@param[in] model: model string
///
///@return 1 on sucess or 0 on fail
void hpdir_set_device(int index)
{
    SS80DiskType *SS80p = NULL;
#ifdef AMIGO
    AMIGODiskType *AMIGOp = NULL;
#endif
	if(Devices[index].TYPE == SS80_TYPE)
	{
		SS80p = (SS80DiskType *) Devices[index].dev;
	}
#ifdef AMIGO
	else if(Devices[index].TYPE == AMIGO_TYPE)
	{
		AMIGOp = (AMIGODiskType *) Devices[index].dev;
	}
#endif
	else
	{
		printf("hpdir_parameters invalid TYPE\n");
		return;
	}

	if(Devices[index].TYPE == SS80_TYPE)
	{
		SS80p->CONFIG.ID 				= hpdir.ID;
		SS80p->UNIT.DEVICE_NUMBER 		= hpdir.DEVICE_NUMBER;
		SS80p->UNIT.BYTES_PER_BLOCK 	= hpdir.BYTES_PER_SECTOR;

		// CHS NOT used in this emulator!
		SS80p->VOLUME.MAX_CYLINDER 		= 0;	// hpdir.CYLINDERS-1;
		SS80p->VOLUME.MAX_HEAD 			= 0;	// hpdir.HEADS-1;
		SS80p->VOLUME.MAX_SECTOR 		= 0;	// hpdir.SECTORS-1;

		SS80p->VOLUME.MAX_BLOCK_NUMBER 	= hpdir.BLOCKS-1;
	}

#ifdef AMIGO
	if(Devices[index].TYPE == AMIGO_TYPE)
	{
		AMIGOp->CONFIG.ID = hpdir.ID;
		AMIGOp->GEOMETRY.BYTES_PER_SECTOR = hpdir.BYTES_PER_SECTOR;
		AMIGOp->GEOMETRY.SECTORS_PER_TRACK = hpdir.SECTORS;
		AMIGOp->GEOMETRY.HEADS = hpdir.HEADS;
		AMIGOp->GEOMETRY.CYLINDERS = hpdir.CYLINDERS;
	}
#endif
	Devices[index].BLOCKS = hpdir.BLOCKS;
	strncpy(Devices[index].model, hpdir.model, sizeof(Devices[index].model) -2);

}

/// ===============================================
///@brief Lookup model in and set drive parameters if found
///
///@param[in] index: Devices index
///@param[in] model: model string
///
///@return void
void hpdir_set_parameters(int index, char *model)
{
	if ( hpdir_find_drive( model, 0 ,1) )
		hpdir_set_device(index);
}

/// ===============================================
/// @brief Post Process COnfiguration file after reading
/// @return  void
void Post_Config()
{
    int i;
    long sectors;

    ///@brief Active SS80 Device
    SS80DiskType *SS80p = NULL;

#ifdef AMIGO
    ///@brief Active AMIGO Device
    AMIGODiskType *AMIGOp = NULL;
#endif

    for(i=0;i<MAX_DEVICES;++i)
    {
        if(Devices[i].TYPE == NO_TYPE)
            continue;

        if(Devices[i].TYPE == SS80_TYPE)
        {
            SS80p= (SS80DiskType *)Devices[i].dev;
			if( SS80p->UNIT.BYTES_PER_BLOCK != 256)
			{
				// SS80p->UNIT.BYTES_PER_BLOCK = 256;
				printf("Warning: %s BYTES_PER_BLOCK != 256, Adjusting to 256\n", Devices[i].model);
			}
			sectors = SS80p->VOLUME.MAX_BLOCK_NUMBER+1;
			Devices[i].BLOCKS = sectors;
        } // SS80_TYPE

#ifdef AMIGO
        if(Devices[i].TYPE == AMIGO_TYPE )
        {
            AMIGOp = (AMIGODiskType *)Devices[i].dev;
			if( AMIGOp->GEOMETRY.BYTES_PER_SECTOR != 256)
			{
				AMIGOp->GEOMETRY.BYTES_PER_SECTOR = 256;
				printf("Warning: %s BYTES_PER_SECTOR != 256, Adjusting to 256\n", Devices[i].model);
			}
			sectors = AMIGOp->GEOMETRY.SECTORS_PER_TRACK
				 * AMIGOp->GEOMETRY.HEADS
				 * AMIGOp->GEOMETRY.CYLINDERS;
			Devices[i].BLOCKS = sectors;
        } 
#endif // #ifdef AMIGO
    }
}

typedef union {
	uint8_t b;
	uint16_t w;
	uint32_t l;
} val_t;

/// ===============================================
/// @brief Read and parse a config file using POSIX functions
/// Set all drive parameters and debuglevel 
///
/// @param name: config file name to process
/// @return  number of parse errors
int Read_Config(char *name)
{
    int ret,len;
    FILE *cfg;
    int state = START_STATE;
    int errors = 0;
    int index = 0;
	val_t val;

    ///@brief Printer Device
    PRINTERDeviceType *PRINTERp = NULL;
    ///@brief SS80 Device
    SS80DiskType *SS80p = NULL;

#ifdef AMIGO
    ///@brief AMIGO Device
    AMIGODiskType *AMIGOp = NULL;
#endif

    char *ptr;
    char str[128];
    char token[128];
    char arg[128];

    init_Devices();

    lines = 0;

    printf("Reading: %s\n", name);
    cfg = fopen(name, "rb");
    if(cfg == NULL)
    {
        ++errors;
        //FIXME
        perror("Read_Config - open");
        printf("Read_Config: open(%s) failed\n", name);
        set_Config_Defaults();
        return(errors);
    }

    while( (ptr = fgets(str, sizeof(str)-2, cfg)) != NULL)
    {
        ++lines;

        ptr = str;

        trim_tail(ptr);
        ptr = skipspaces(ptr);
        len = strlen(ptr);

        if(!len)
            continue;

        // Skip comments
        if(*ptr == '#')
            continue;

		*token = 0;
		*arg = 0;
		val.l = 0;

		// To save on code we process a token and optional argument here
		ptr = get_token(ptr,token, sizeof(token)-2);

		// Argument
		ptr = get_token(ptr,arg, sizeof(arg)-2);
		if( MATCHI(arg,"=") )
			ptr = get_token(ptr,arg,sizeof(arg)-2);
		val.l = get_value(arg);

        //FIXME check for state and last state
        if( MATCHI(token,"END") )
        {
            state = pop_state();
            continue;
        }

        switch(state)
        {
        case START_STATE:

            if( MATCHI (token,"SS80_DEFAULT") )
            {
                push_state(state);
				state = SS80_STATE;
				index = alloc_device(SS80_DEFAULT_TYPE);
				if(index == -1)
					state = START_STATE;
				else
					SS80p = (SS80DiskType *) Devices[index].dev;
            }

            else if( MATCHI (token,"SS80") || MATCHI (token, "CS80") )
            {
                push_state(state);
				state = SS80_STATE;
				index = alloc_device(SS80_TYPE);
				if(index == -1)
				{
					state = START_STATE;
				}
				else
				{
					SS80p = (SS80DiskType *) Devices[index].dev;
					hpdir_set_parameters(index,arg);	// Also sets Devices[index].model
				}
			}

#ifdef AMIGO
            else if( MATCHI (token,"AMIGO") )
            {
                push_state(state);
                state = AMIGO_STATE;
                index = alloc_device(AMIGO_TYPE);
                if(index == -1)
				{
                    state = START_STATE;
				}
                else
				{
                    AMIGOp = (AMIGODiskType *) Devices[index].dev;
					hpdir_set_parameters(index,arg);	// Also sets Devices[index.model
				}
			}
#endif
            else if( MATCHI (token,"PRINTER") )
            {
                push_state(state);
                state = PRINTER_STATE;
                index = alloc_device(PRINTER_TYPE);
                if(index == -1)
                    state = START_STATE;
                else
                    PRINTERp = (PRINTERDeviceType *) Devices[index].dev;
            }
            else if( MATCHI (token,"DEBUG") )
            {
				debuglevel = val.w;
            }
            else if( MATCHI (token,"PRINTER_DEFAULT_ADDRESS") )
            {
                //FIXME REMOVE from config
                printf("Skipping %s, at line:%d\n", str,lines);
            }
            else
            {
                printf("Unexpected START token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;

        case PRINTER_STATE:
            if( MATCHI (token,"CONFIG") )
            {
                push_state(state);
                state = PRINTER_CONFIG;
            }
            else
            {
                printf("Unexpected PRINTER token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;

        case PRINTER_CONFIG:
            if( MATCHI (token,"ADDRESS") )
            {
				if(val.b > 31)
				{
					printf("Fatal PRINTER ADDRESS out of range: %ld disabled:%d\n", val.l, lines);
					val.b = 0xff;
                    ++errors;
				}
                Devices[index].ADDRESS = val.b;
                PRINTERp->HEADER.ADDRESS  = val.b;
                // NO PPR
                Devices[index].PPR = 0xff;
                PRINTERp->HEADER.PPR = 0xff;
            }
            else
            {
                printf("Unexpected PRINTER CONFIG token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;

        case SS80_STATE:
            if( MATCHI (token,"HEADER") )
            {
                push_state(state);
                state = SS80_HEADER;
            }
            else if( MATCHI (token,"CONFIG") )
            {
                push_state(state);
                state = SS80_CONFIG;
            }
            else if( MATCHI (token,"CONTROLLER") )
            {
                push_state(state);
                state = SS80_CONTROLLER;
            }
            else if( MATCHI (token,"UNIT") )
            {
                push_state(state);
                state = SS80_UNIT;
            }
            else if( MATCHI (token,"VOLUME") )
            {
                push_state(state);
                state = SS80_VOLUME;
            }
            else
            {
                printf("Unexpected SS80 START token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;


        case SS80_HEADER:
            if( MATCHI (token,"ADDRESS") )
            {
                if(val.b > 31)
				{
					printf("Fatal SS80 ADDRESS > 31 at line d\n", lines);
					val.b = 0xff;
					++errors;
				}
                Devices[index].ADDRESS = val.b;
                SS80p->HEADER.ADDRESS  = val.b;
            }
            else if( MATCHI (token,"PPR") )
            {
                if(val.b > 7)
				{
					printf("Warning SS80 PPR > 7 at line:%d\n", lines);
					val.b = 0xff;
					++errors;
				}
                Devices[index].PPR = val.b;
                SS80p->HEADER.PPR = val.b;
            }
            else if( MATCHI (token,"FILE") )
            {
                SS80p->HEADER.NAME = stralloc(arg);
            }
            else
            {
                printf("Unexpected SS80 CONFIG token: %s, at line:%d\n", token,lines);
                ++errors;
            }
            break;

        case SS80_CONFIG:
            if( MATCHI (token,"ID") )
            {
                SS80p->CONFIG.ID = val.w;
            }
            else
            {
                printf("Unexpected SS80 CONFIG token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;

        case SS80_CONTROLLER:
            if( MATCHI (token,"UNITS_INSTALLED") )
            {
                SS80p->CONTROLLER.UNITS_INSTALLED = val.w;
            }
            else if( MATCHI (token,"TRANSFER_RATE") )
            {
                SS80p->CONTROLLER.TRANSFER_RATE = val.w;
            }
            else if( MATCHI (token,"TYPE") )
            {
                SS80p->CONTROLLER.TYPE = val.w;
            }
            else
            {
                printf("Unexpected SS80 CONTROLLER token: %s, at line:%d\n", token,lines);
                ++errors;
            }
            break;


        case SS80_UNIT:
            if( MATCHI (token,"UNIT_TYPE") )
            {
                SS80p->UNIT.UNIT_TYPE = val.w;
            }
            else if( MATCHI (token,"DEVICE_NUMBER") )
            {
                SS80p->UNIT.DEVICE_NUMBER = val.l;
            }
            else if( MATCHI (token,"BYTES_PER_BLOCK") )
            {
                if(val.w > 0x1000)
				{
					printf("Fatal: SS80 BYTES_PER_BLOCK > 0x1000, set to 256 at line:%d\n", lines);
					val.w = 256;
                    ++errors;
				}
                SS80p->UNIT.BYTES_PER_BLOCK = val.w;
            }
            else if( MATCHI (token,"BUFFERED_BLOCKS") )
            {
				if(val.b > 1)
				{
					printf("Fatal: SS80 BUFFERED_BLOCKS > 1, set to 1 at line:%d\n", lines);
                    ++errors;
					val.w = 1;
				}
                SS80p->UNIT.BUFFERED_BLOCKS = val.b;
            }
            else if( MATCHI (token,"BURST_SIZE") )
            {
				if(val.b > 0)
				{
					printf("Fatal: SS80 BURST_SIZE > 0, set to 1 at line:%d\n", lines);
                    ++errors;
					val.w = 0;
				}
                SS80p->UNIT.BURST_SIZE = val.b;
            }
            else if( MATCHI (token,"BLOCK_TIME") )
            {
                SS80p->UNIT.BLOCK_TIME = val.w;
            }
            else if( MATCHI (token,"CONTINUOUS_TRANSFER_RATE") || MATCHI (token,"CONTINOUS_TRANSFER_RATE") )
            {
                SS80p->UNIT.CONTINUOUS_TRANSFER_RATE = val.w;
            }
            else if( MATCHI (token,"OPTIMAL_RETRY_TIME") )
            {
                SS80p->UNIT.OPTIMAL_RETRY_TIME = val.w;
            }
            else if( MATCHI (token,"ACCESS_TIME") )
            {
                SS80p->UNIT.ACCESS_TIME = val.w;
            }
            else if( MATCHI (token,"MAXIMUM_INTERLEAVE") )
            {
                SS80p->UNIT.MAXIMUM_INTERLEAVE = val.b;
            }
            else if( MATCHI (token,"FIXED_VOLUMES") )
            {
                SS80p->UNIT.FIXED_VOLUMES = val.b;
            }
            else if( MATCHI (token,"REMOVABLE_VOLUMES") )
            {
                SS80p->UNIT.REMOVABLE_VOLUMES = val.b;
            }
            else
            {
                printf("Unexpected SS80 UNIT token: %s, at line:%d\n", token,lines);
                ++errors;
            }
            break;

        case SS80_VOLUME:
            if( MATCHI (token,"MAX_CYLINDER") )
            {
                SS80p->VOLUME.MAX_CYLINDER = val.l;
            }
            else if( MATCHI (token,"MAX_HEAD") )
            {
                SS80p->VOLUME.MAX_HEAD = val.b;
            }
            else if( MATCHI (token,"MAX_SECTOR") )
            {
                SS80p->VOLUME.MAX_SECTOR = val.w;
            }
            else if( MATCHI (token,"MAX_BLOCK_NUMBER") )
            {
                SS80p->VOLUME.MAX_BLOCK_NUMBER = val.w;
            }
            else if( MATCHI (token,"INTERLEAVE") )
            {
                SS80p->VOLUME.INTERLEAVE = val.b;

            }
            else
            {
                printf("Unexpected SS80 VOLUME token: %s, at line:%d\n", token,lines);
                ++errors;
            }
            break;

#ifdef AMIGO
        case AMIGO_STATE:
            if( MATCHI (token,"HEADER") )
            {
                push_state(state);
                state = AMIGO_HEADER;
            }
            else if( MATCHI (token,"CONFIG") )
            {
                push_state(state);
                state = AMIGO_CONFIG;
            }
            else if( MATCHI (token,"GEOMETRY") )
            {
                push_state(state);
                state = AMIGO_GEOMETRY;
            }
            else
            {
                printf("Unexpected AMIGO START token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;

        case AMIGO_HEADER:
            if( MATCHI (token,"DRIVE") )
            {
                //skip this
                printf("Skipping %s, at line:%d\n", str,lines);
            }
            else if( MATCHI (token,"ADDRESS") )
            {
                if(val.b > 31)
				{
					printf("Fatal AMIGO ADDRESS: %d > 31 disabled at line:%d\n", str,lines);
					val.b = 0xff;
					++errors;
				}
                Devices[index].ADDRESS = val.b;
                AMIGOp->HEADER.ADDRESS = val.b;
            }
            else if( MATCHI (token,"PPR") )
            {
                if(val.b > 7)
				{
					printf("Warning AMIGO ADDRESS: %d > 7, disabled at line:%d\n", str,lines);
					val.b = 0xff;
					++errors;
				}
                Devices[index].PPR = val.b;
                AMIGOp->HEADER.PPR = val.b;
            }
            else if( MATCHI (token,"FILE") )
            {
                AMIGOp->HEADER.NAME = stralloc(arg);
            }
            else
            {
                printf("Unexpected HEADER CONFIG token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;

        case AMIGO_CONFIG:
            if( MATCHI (token,"ID") )
            {
                AMIGOp->CONFIG.ID = val.w;
            }
            else
            {
                printf("Unexpected AMIGO CONFIG token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;

        case AMIGO_GEOMETRY:
            if( MATCHI (token,"BYTES_PER_SECTOR") )
            {
                if(val.w > 0x1000)
				{
					printf("Fatal: AMIGO BYTES_PER_SECTOR > 0x1000, set to 256 at line:%d\n", lines);
					val.w = 256;
                    ++errors;
				}
                AMIGOp->GEOMETRY.BYTES_PER_SECTOR = val.w;
            }
            else if( MATCHI (token,"SECTORS_PER_TRACK") )
            {
                AMIGOp->GEOMETRY.SECTORS_PER_TRACK = val.w;
            }
            else if( MATCHI (token,"HEADS") )
            {
                AMIGOp->GEOMETRY.HEADS = val.w;
            }
            else if( MATCHI (token,"CYLINDERS") )
            {
                AMIGOp->GEOMETRY.CYLINDERS = val.w;
            }
            else
            {
                printf("Unexpected AMIGO GEMETRY token: %s, at line:%d\n", str,lines);
                ++errors;
            }
            break;
#endif // #ifdef AMIGO

        default:
            printf("Fatal Unexpected STATE %d at line:%d\n", state, lines);
            ++errors;
            break;

        } // switch
    } //while
    if(state != START_STATE)
    {
        printf("Missing END statement at line:%d\n", lines);
        ++errors;
    }
    printf("Read_Config: read(%d) lines\n", lines);
    if(errors)
        printf("Read_Config: ****** errors(%d) ******\n", errors);

    ret = fclose(cfg);
    if(ret == EOF)
    {
        perror("Read_Config - close error");
        ++errors;
    }

	// Post process device values
	Post_Config();

    return(errors);
}



/// ===============================================
/// @brief Display Configuration File variable
/// @param str: title
/// @param val: variable value
/// @return  void
void print_var_P(__memx const char *str, uint32_t val)
{
    char tmp[64];
    int i=0;
    while( *str && i < 62)
        tmp[i++] = *str++;
    tmp[i++] = 0;

    printf("    %-25s = %8lxH (%ld)\n", tmp, val, val); 
}

/// ===============================================
/// @brief Display Configuration File string
/// @param *str: title
/// @param *arg: string
/// @return  void
void print_str_P(__memx const char *str, char *arg)
{
    char tmp[64];
    int i=0;
    while( *str && i < 62)
        tmp[i++] = *str++;
    tmp[i++] = 0;
    printf("    %-25s = \"%s\"\n", tmp, arg);   
}

/// ===============================================
/// @brief Display Configuration device address saummary
/// @return  void
void display_Addresses()
{
    int i;
    int address;
    printf("Listen and Talk Address Settings\n");
    for(i=0;i<MAX_DEVICES;++i)
    {
        if(Devices[i].TYPE == NO_TYPE)
            continue;
        address = Devices[i].ADDRESS;

        if(Devices[i].TYPE == SS80_TYPE)
        {
            printf("  SS80_MLA    = %02XH\n",BASE_MLA + address );
            printf("  SS80_MTA    = %02XH\n",BASE_MTA + address );
            printf("  SS80_MSA    = %02XH\n",BASE_MSA + address );
        }
#ifdef AMIGO
        if(Devices[i].TYPE == AMIGO_TYPE )
        {
            printf("  AMIGO_MLA   = %02XH\n",BASE_MLA + address );
            printf("  AMIGO_MTA   = %02XH\n",BASE_MTA + address );
            printf("  AMIGO_MSA   = %02XH\n",BASE_MSA + address );
        }
#endif
        if(Devices[i].TYPE == PRINTER_TYPE )
        {
            printf("  PRINTER_MLA = %02XH\n",BASE_MLA + address );
            printf("  PRINTER_MTA = %02XH\n",BASE_MTA + address );
            printf("  PRINTER_MSA = %02XH\n",BASE_MSA + address );
        }
    }
    printf("\n");
}

/// ===============================================
/// @brief Display current Configuration File values
/// @return  void
void display_Config()
{
    int i;
    ///@brief Active Printer Device
    PRINTERDeviceType *PRINTERp = NULL;
    ///@brief Active SS80 Device
    SS80DiskType *SS80p = NULL;

#ifdef AMIGO
    ///@brief Active AMIGO Device
    AMIGODiskType *AMIGOp = NULL;
#endif

    printf("Current Configuration Settings\n");
    for(i=0;i<MAX_DEVICES;++i)
    {
        if(Devices[i].TYPE == NO_TYPE)
            continue;

        if(Devices[i].TYPE == SS80_TYPE)
        {
            SS80p= (SS80DiskType *)Devices[i].dev;

            printf("SS80 %s\n", Devices[i].model);
			printf("  # HP85 BASIC ADDRESS :D7%d0\n", (int) SS80p->HEADER.ADDRESS);
            printf("  CONFIG\n");
                print_var("ADDRESS", (uint32_t) SS80p->HEADER.ADDRESS);
                print_var("PPR", (uint32_t) SS80p->HEADER.PPR);
                print_str("FILE", SS80p->HEADER.NAME);
			printf("  END\n");
            printf("  HEADER\n");
                print_var("ID", (uint32_t) SS80p->CONFIG.ID);
			printf("  END\n");
            printf("  CONTROLLER\n");
                print_var("UNITS_INSTALLED", (uint32_t) SS80p->CONTROLLER.UNITS_INSTALLED);
                print_var("TRANSFER_RATE", (uint32_t)  SS80p->CONTROLLER.TRANSFER_RATE);
                print_var("TYPE", (uint32_t)  SS80p->CONTROLLER.TYPE);
			printf("  END\n");
            printf("  UNIT\n");
                print_var("UNIT_TYPE", (uint32_t)SS80p->UNIT.UNIT_TYPE);
                print_var("DEVICE_NUMBER", (uint32_t)SS80p->UNIT.DEVICE_NUMBER);
                print_var("BYTES_PER_BLOCK", (uint32_t)SS80p->UNIT.BYTES_PER_BLOCK);
                print_var("BUFFERED_BLOCKS", (uint32_t)SS80p->UNIT.BUFFERED_BLOCKS);
                print_var("BURST_SIZE", (uint32_t)SS80p->UNIT.BURST_SIZE);
                print_var("BLOCK_TIME", (uint32_t)SS80p->UNIT.BLOCK_TIME);
                print_var("CONTINUOUS_TRANSFER_RATE", (uint32_t)SS80p->UNIT.CONTINUOUS_TRANSFER_RATE);
                print_var("OPTIMAL_RETRY_TIME", (uint32_t)SS80p->UNIT.OPTIMAL_RETRY_TIME);
                print_var("ACCESS_TIME", (uint32_t)SS80p->UNIT.ACCESS_TIME);
                print_var("MAXIMUM_INTERLEAVE", (uint32_t)SS80p->UNIT.MAXIMUM_INTERLEAVE);
                print_var("FIXED_VOLUMES", (uint32_t)SS80p->UNIT.FIXED_VOLUMES);
                print_var("REMOVABLE_VOLUMES", (uint32_t)SS80p->UNIT.REMOVABLE_VOLUMES);
			printf("  END\n");
            printf("  VOLUME\n");
                print_var("MAX_CYLINDER", (uint32_t)SS80p->VOLUME.MAX_CYLINDER);
                print_var("MAX_HEAD", (uint32_t)SS80p->VOLUME.MAX_HEAD);
                print_var("MAX_SECTOR", (uint32_t)SS80p->VOLUME.MAX_SECTOR);
                print_var("MAX_BLOCK_NUMBER", (uint32_t)SS80p->VOLUME.MAX_BLOCK_NUMBER);
                print_var("INTERLEAVE", (uint32_t)SS80p->VOLUME.INTERLEAVE);
                print_var("# BLOCKS", (uint32_t)SS80p->VOLUME.MAX_BLOCK_NUMBER+1);
			printf("  END\n");
        } // SS80_TYPE

#ifdef AMIGO
        if(Devices[i].TYPE == AMIGO_TYPE )
        {
            AMIGOp= (AMIGODiskType *)Devices[i].dev;

            printf("AMIGO %s\n", Devices[i].model);
			printf("  # HP85 BASIC ADDRESS :D7%d0\n", (int) AMIGOp->HEADER.ADDRESS);
            printf("  HEADER\n");
                print_var("ADDRESS", (uint32_t) AMIGOp->HEADER.ADDRESS);
                print_var("PPR", (uint32_t) AMIGOp->HEADER.PPR);
                print_str("FILE", AMIGOp->HEADER.NAME);
			printf("  END\n");
            printf("  CONFIG\n");
                print_var("ID", (uint32_t) AMIGOp->CONFIG.ID);
			printf("  END\n");
            printf("  GEOMETRY\n");
                print_var("BYTES_PER_SECTOR", (uint32_t) AMIGOp->GEOMETRY.BYTES_PER_SECTOR);
                print_var("SECTORS_PER_TRACK", (uint32_t) AMIGOp->GEOMETRY.SECTORS_PER_TRACK);
                print_var("HEADS", (uint32_t) AMIGOp->GEOMETRY.HEADS);
                print_var("CYLINDERS", (uint32_t) AMIGOp->GEOMETRY.CYLINDERS);
                print_var("# BLOCKS", (uint32_t)AMIGOp->GEOMETRY.CYLINDERS * AMIGOp->GEOMETRY.SECTORS_PER_TRACK * AMIGOp->GEOMETRY.HEADS );
			printf("  END\n");
        } 
#endif // #ifdef AMIGO

        if(Devices[i].TYPE == PRINTER_TYPE )
        {
            PRINTERp= (PRINTERDeviceType *)Devices[i].dev;

            printf("PRINTER\n");
            printf("  CONFIG\n");
                print_var("ADDRESS", (uint32_t) PRINTERp->HEADER.ADDRESS);
			printf("  END\n");
        }
		printf("END\n");
		printf("\n");
    }
    printf("END\n");
    printf("\n");
}

/// ===============================================
/// @brief Format devices that have no image file
/// @return  void
void format_drives()
{
    int i;
    struct stat st;
    long sectors;
    char label[32];
    int ss80 = 1;
    int amigo = 1;

    ///@brief Active SS80 Device
    SS80DiskType *SS80p = NULL;

#ifdef AMIGO
    ///@brief Active AMIGO Device
    AMIGODiskType *AMIGOp = NULL;
#endif

    for(i=0;i<MAX_DEVICES;++i)
    {
        if(Devices[i].TYPE == NO_TYPE)
            continue;

        if(Devices[i].TYPE == SS80_TYPE)
        {
            SS80p= (SS80DiskType *)Devices[i].dev;

            if(stat(SS80p->HEADER.NAME, &st) == -1) 
            {
                if( SS80p->UNIT.BYTES_PER_BLOCK != 256)
                {
                    printf("Can not use non 256 byte sectors\n");
                    continue;
                }
                //SS80p->VOLUME.MAX_CYLINDER;
                //SS80p->VOLUME.MAX_HEAD;
                //SS80p->VOLUME.MAX_SECTOR;
                sectors = Devices[i].BLOCKS;
                sprintf(label,"SS80-%d", ss80);
#ifdef LIF_SUPPORT
                lif_create_image(SS80p->HEADER.NAME,
                    label,
                    lif_dir_count(sectors), 
                    sectors);
#else
                printf("please create a SS80 LIF image with %ld sectors and 128 directory sectors\n", sectors);
#endif

            }
            ss80++;
        } // SS80_TYPE

#ifdef AMIGO
        if(Devices[i].TYPE == AMIGO_TYPE )
        {
            AMIGOp= (AMIGODiskType *)Devices[i].dev;
            if(stat(AMIGOp->HEADER.NAME, &st) == -1) 
            {
                if( AMIGOp->GEOMETRY.BYTES_PER_SECTOR != 256)
                {
                    printf("Can not use non 256 byte sectors\n");
                    continue;
                }
                sectors = Devices[i].BLOCKS;
                sprintf(label,"AMIGO%d", amigo);
#ifdef LIF_SUPPORT
                lif_create_image(AMIGOp->HEADER.NAME,
                    label,
                    lif_dir_count(sectors), 
                    sectors);
#else
                printf("please create a AMIGO LIF image with %ld sectors and 15 directory sectors\n", sectors);
#endif
            }
            amigo++;
        } 
#endif // #ifdef AMIGO
    }
    printf("\n");
}
