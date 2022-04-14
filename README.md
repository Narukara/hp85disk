\section README

# Documentation
## HP85 Disk Emulator Copyright (C) 2014-2020 Mike Gore 
  * This Project Emulates **AMIGO** and **SS80** disk drives used by the **HP85** series computers.
  * The images are stored in **LIF** format used by the **HP85** series computers
  * See [COPYRIGHT](COPYRIGHT.md) for a full copyright notice for the project
  * Documentation created using Doxygen available at
    * https://rawgit.com/magore/hp85disk/master/doxygen/html/index.html
    * All emulated disk images are just regular files stored on a standard FAT32 formatted SD Card

## Features for the **HP85** in brief
  * **NOTE: Later sections go into more detail**
  * This project emulates **GPIB drives** and **HPGL printer**
    * Each emulated disk image is a **LIF** encoded file on a FAT32 formatted SD CARD.
    * **You MUST have the GPIB module installed in your HP85 in order to connect to the hp85disk emulator**
   * [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) fully defines each disk image on SD Card
     * Disk images are **LIF** encoded files that are compatible with **HP85A/B** and many other computers
     * Missing disk image files are created automatically if 
  * The emulator will automatically create missing **LIF** images defined in [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) on the SD CARD
  * There are disk images for **AMIGO** and **SS80** disks
    * **AMIGO** drives work with **HP85A** 
    * **SS80** drives work with **HP85B** (or **HP85A** with **PRM-85** with modified EMS and Electronic disk ROM add on board see links)
      * You may have up to 4 disks with V1 hardware and 8 with V2 hardware
  * There is a **Printer emulator** that can capture and save printer data to a time stamped file.
  * Built in command processor with lots of useful features
    * Want to translate between plain text **BASIC** files and **HP85** BASIC programs? You Can!
      * See full details later in this document
    * Access the hp85disk command processor via its USB interface and your computer with a serial terminal program
      * See full details later in this document for serial terminal configuration and finding the device port name
    * Any key press halts the emulator and waits for a user command
      * After finishing any user commend and options press the Enter/Return key to return to disk emulation
    * Type **help** for a list of top level commands
      * Each help item has its own help
      * Example: **lif help**
   * **LIF** manipulation tools are built in see later sections for details
   * Convert *TeleDisk* images into **LIF**
   * The emulator **RTC** can be used for time stamping plot files and files added into lif images


___ 



## Credits

## HP85 disk emulator V2 circuit board layout design by (C) 2018-2020 Jay Hamlin
## V2 board design - github master branch targets both old and new board by Jay Hamlin
## V2 code is now working
  * [Jay Hamlin designed this board](board/V2/releases)
  * V2 hardware adds
   * **PCB Circuit board design**
   * **GPIB BUS drivers**
     * 48Mma drive required by the GPIB spec
   * **I2C** level converters and standard Qwiic Bus interface - 3.3V
   * **RTC** clock for time stamping files added to **LIF** images, etc
   * Optional I2C LCD interface
     * work in progress
   * Advanced Hardware Reset circuit
   * Micro size SD card interface
  * [Current design files](https://github.com/magore/hp85disk/tree/master/board/releases/HP85Disk_release_03)
## V2 Pictures by Jay Hamlin
![](https://github.com/magore/hp85disk/blob/master/hp85disk-1.png)
![](https://github.com/magore/hp85disk/blob/master/hp85disk-2.png)



## HP85 disk emulator V1 board design (C) 2014-2020 Mike Gore
## [V1 board readme](board/V1/README.md)
  * [My original board design without **GPIB** buffers](board/V1/README.md)
  * Limited control and BUS drive power 
  * If you are using a **V1** board changes the update the [Makefile](Makefile) **BOARD ?= 1**
    * About half of the 48Mma drive required by the **GPIB** spec
    * However we can read any pin any time - useful for tracing/debugging
  * **RTC** -  **DS1307** for time stamping 


## HP Disk Emulator by Anders Gustafsson
**Anders Gustafsson was extremely helpful in getting my project started.**
*In fact I really owe the very existence of this project to his original project*
 * You can visit his project at this site:
   * <http://www.dalton.ax/hpdisk>
   * <http://www.elektor-labs.com/project/hpdisk-an-sd-based-disk-emulator-for-gpib-instruments-and-computers.13693.html>

Anders originally provided me his source code in 2014 and many details of his project *which I am very thankful for.*
NOTE: 
 I ended up rewriting virtually all of Anders project.
 This was mainly a personal exercise in fully understanding the code and NOT because of any problems with his original work.
 I did this one part at a time as I read the HP documents covering the protocols and specifications.
 Although rewritten I have maintained the basic concept of using  state machines for **GPIB** ,**AMIGO** and **SS80** state tracking.

## The HPDir project was a vital documentation source for this project
  * <http://www.hp9845.net/9845/projects/hpdir>
  * I use the **hpdir.ini** of their project for creating disk images

### Other HPDir resources for disk image manipulation
  * [HPDrive project has very useful references an tools for creating HP disk **LIF** images compatible with this project](http://www.hp9845.net/9845/projects/hpdrive)
    * Copyright � 2010 **A. K�ckes**
  * [HPDir project has very useful references and tools for manipulating HP disk **LIF** images compatible with this project](http://www.hp9845.net/9845/projects/hpdrive)
    * Copyright � 2010 **A. K�ckes**
  * [ See **LIF** Documentation - part of the HPDir project for details](http://www.hp9845.net/9845/projects/hpdir/#lif_filesystem)
    * Copyright � 2010 **A. K�ckes**


## My TeleDisk to LIF conversion utility
 * I used the lzss libraries and documentation by **Dave Dunfield**
   * Copyright 2007-2008 Dave Dunfield All rights reserved.
 * Documentation from **Jean-Franois DEL NERO**
   * Copyright (C) 2006-2014 Jean-Franois DEL NERO
[lif/teledisk](lif/teledisk)
 * [lif/teledisk](lif/teledisk)
   * My **TELEDISK LIF** extractor
   * Important Contributions (My converted would not have been possible without these)
     * **Dave Dunfield**, LZSS Code and *TeleDisk* documentation
       * Copyright 2007-2008 Dave Dunfield All rights reserved.
       * [td0_lzss.h](lif/teledisk/td0_lzss.h)
       * [td0_lzss.c](lif/teledisk/td0_lzss.c)
         * *LZSS decoder*
       * [td0notes.txt](lif/teledisk/td0notes.txt)
         * *TeleDisk Documentation*
     * **Jean-Franois DEL NERO**, **TeleDisk Documentation**
       * Copyright (C) 2006-2014 Jean-Franois DEL NERO
         * [wteledsk.htm](lif/teledisk/wteledsk.htm)
           * *TeleDisk documentation*
         * See his github project
             * https://github.com/jfdelnero/libhxcfe

## FatFS low level disk IO
  * [fatfs](fatfs)
    * R0.14 FatFS code from (C) **ChaN**, 2019 - With very minimal changes 

## Optiboot
  * [optiboot](optiboot)
    * Optiboot Bootloader for Arduino and Atmel AVR
    * See: https://github.com/Optiboot/optiboot
       * [GPLv2 WRT](https://github.com/Optiboot/optiboot/blob/master/LICENSE)
       * [README](https://github.com/Optiboot/optiboot/blob/master/README.md)

## STK500v1 uploader for Optiboot
  * [uploader/flasher.py](uploader/flasher.py)
    * Optiboot uploader by **Mathieu Virbel** <mat@meltingrocks.com> 
      * Original repository https://github.com/tito/stk500
        * Authors main github page https://github.com/tito/stk500
          * https://meltingrocks.com/
  * Changes Jay and I made
   * Changed to atmega1284p
   * Jay converted code to Python 3
   * Added Baudrate argument
   * Added code to send "reset" command to hp85disk firmware to drop into optiboot
   * Fixed Intel 02 segment record calculation

___


## Resource Links 
  * 3D printed cases
    * https://github.com/MartinHepperle/HP-ReproParts

___


## Initial Setup of HP85 disk V2 PCB by Jay Hamlin
### Setup checklist
  * **You must an HP85 82937A GPIB card installed** [See document](Documents/82937AHP-IBInterface-InstallationAndTheoryOfOperationManual-82937-90007-56pages-Oct80)
    * **You must also have the required HP 82937A option roms installed**
      * **Mass Storage ROM** [Mass Storage ROM Manual](Documents/HP85-MassStorage.pdf)
      * **NOTE** I use the **PRM-85** with the HP85A super ROM - in my unit as it has all of the option ROMs included
  * Connect the GPIB plug from your HP85 GPIB card to the emulator - for initial testing make sure it is the only GPIB device attached
  * **NOTE:** The Emulator must be powered up and attached **before your HP85 is turned on** (the HP85 only detects disks after power-on or reset)
  * Attach the Power Supply with 2.1mm jack - suggest 7.5V to 9V DC regulated adapter with negative outside, positive center
    * You will see LED1 flash several times while the system is reading the configuration file and detecting disk images
    * If LED2 turns on there is an error reading the SD Card - see **SD Card problems** section


### Host computer requirements for serial debugging and firmware updating
  * You have either
    * **Ubuntu Linux desktop**
    * **Windows 10** running Windows Subsystem for Linux (Called WSL ) and the Ubuntu App from the Microsoft App store
    * **Mac OS**
  * All Text and configuration files used with the emulator MUST plain text format only (8 bit ASCII) 
     * Please NO Unicode - both file names and file formats!
  * You have a serial terminal program installed - for example minicom 
  * You need a FAT32 format blank SD Card
    * Copy of the hp85disk, github master branch, sdcard folder contents onto the SD Card
      * Make sure you only copy the contents and NOT folder AND contents
        * The emulator assumes the SD card home directory contains the images and configuration files

### Testing the emulator with the HP85
  * Turn on your HP85 - once the cursor is visable type the command **CAT**
    * You should see a directory listing, otherwise go to the bext section **initial troubleshooting**
  * If you have a directory listing see section called **HP85A and HP85B examples** for more examples


### Initial troubleshooting - if CAT gives a time-out error 
  * Make sure your device is the ONLY DEVICE attached to the HP 82937A controller of your HP85
  * You might not have the required option **ROMs** installed in your **HP85** for the HP85 GPIB interface card
  * The SD Card is missing the configuration file or disk images
  * The SD Card filesystem is corrupt.
  * See **SD Card problems** section
  * **NOTE** I would also suggest connecting your computer to the USB connector for more detailed disagnostics
  * USB Connection Summary
    * Attach USB cable from emulator to your computer using a serial terminal program
    * Read starting with section 
      * [Firmware updating and connecting to the hp85disk emulator with MINIMAL software install](#firmware-updating-and-connecting-to-the-hp85disk-emulator-with-minimal-software-install)
      * Read down to section [Connecting to hp85disk interactive serial port](#connecting-to-hp85disk-interactive-serial-port)
      * Read section [Command interface notes}(#command-interface-notes)
    * See section [hp85disk troubleshooting with serial terminal and DEBUG option](#hp85disk-troubleshooting-with-serial-terminal-and-debug-option) to verify your HP85 is communicating with your emulator
  * With the serial terminal connected type **help** you should see a help menu
    * To make sure there are files on the SD Card type one of these commands
      * **fatfs ls** for windows like director listing [example result](fatfs-ls.txt)  
      * **ls** for a Linux like directory listing [example result](trace/ls.txt)
  * Enter the serail terminal command **cat hpdisk.cfg** to see if you can read the main configuration file
  * Turn on debugging with the **DEBUG =** command
  * References
    * [hp85disk troubleshooting with serial terminal and DEBUG option](#hp85disk-troubleshooting-with-serial-terminal-and-debug-option)
    * [hp85disk DEBUG truth table](#hp85disk-debug-truth-table)
    * hp85disk setting debug examples](#hp85disk-setting-debug-examples)

### Status DEBUG LEDs
  * **LED2** goes on for error conditions - this is the LED nearest the GPIB connector
    * If this turns on you have an SDCARD problem - either missing card or filesystem issue
    * This LED will only go off after a reset and if no SD Card problems are detected
  * **LED1** is on when the SD Card is being read or written to

## Detailed information about tools and features 

## Built in command processor with many tools
### Accessing the hp85disk command interface with a serial terminal
  * Used to access the hp85disk command interface
  
### Command interface notes
  * When typing **any character** to the hp85disk emulator will stop disk emulation and display:
    * \<INTERRUPT>
  * After you type **Enter** the command processor will execute the command and return to disk emulation automatically

### Summary Getting files into and out of the emulator
  * The emulator uses a standard FAT32 formatted SD card accessable by standard Windows/Mac/Linux desktops.
    * This means you can removed the SD card and attach it to your desttop PC with a card reader
      * **ALWAYS POWER OFF the EMULATOR when inserting or removing the SD card**
  * Emulator disk images are single FAT32 files that are internally fomatted as LIF images - the format used by the HP85
  * This design allows you to copy files to/from the SD Card with your desktop PC
  * The hp85disk project includes tools for your desktop PC that can add/extract files from LIF disk images
    * These file and LIF image tools are also built into the emulator firmware - see references
      * [File manipulation commands built into the emulator](README.md#sd-card-file-manipulation-commands)
      * [LIF tools refrence](README.md#lif-tools-are-built-into-emulator-firmware)
      * [LIF - Importing TEXT files into LIF images](README.md#importing-ascii-and-plain-text-as-hp85-basic-programs)
      * [LIF - Exporting TEXT files from LIF images](README.md#exporting-hp85-basic-programs-to-ascii-plain-text)
      * [LIF - command](README.md#for-lif-help-type-lif-help)
    
___ 


## Important notes about SD Card requirements for the emulator

### Removing the SD Card from the emulator
  * **ALWAYS POWER OFF THE EMULATOR BEFORE REMOVING -or - INSERTING THE SD CARD**

### Formatting a new SD Card 
  * Must be formatted **FAT32** not FAT32x

### Using the SD Card images on your HP85
  * We provide the emulator with an SD card with images already installed
    * If you wish to create a new one follow these steps
  * There are working copies of disk images under the github sdcard folder
  * Open a web browser to https://github.com/magore/hp85disk
    * Click on the Green **Clone or Download** button
    * Pick download as **ZIP**
    * You can extract all of the folders and files to a folder of your choice
    * locate the **sdcard** folder
  * Copy the **contents** of the **sdcard** folder to the root/home folder of a a FAT32 formatted SD card
    * You should not see the **sdcard** folder on the SD Card itself just the contents of that folder and its subfolders

#### HP85A users SD card setup
  * Normally you will not be able to access **SS80** drives unless you have versions of the **HP85B** ROMS installed
    * See the notes about using a **PRM-85** to add the required ROMS
  * Copy the [amigo.cfg](sdcard/amigo.cfg) to the SD card and rename it to [hpdisk.cfg](sdcard/hpdisk.cfg)
    * This is an **AMIGO** drive only configuration
    * The default amigo.cfg file defines four **AMIGO** drives ready to go

#### HP85B users SD card setup
  * You needs the **EMS** ROM for **SS80** drives
    * If you do not have them see the **HP85A** notes above
  * You do not need to make any changes
    * The default [hpdisk.cfg](sdcard/hpdisk.cfg) uses two **AMIGO** drives and two **SS80** drives ready to go

___ 


###  Choosing an SD Card for the HP85 emulator PLEASE READ
  * Summary: When the hp85disk emulator writes to the SD Card the Card internally must modify a much much larger internal flash page - this can take too long
    * The **HP85** is sensitive to long read/write delays that some cards can cause problems with
    * You want SD Cards with fast random writes
    * I have found that the SanDisk Extreme and SanDisk Extreme Pro cards work best.
      * There is a huge difference in various cards on the market. 
      * Look for the cards with the best 4K random write times
      * A good source of benchmark information is looking for recent Raspberry Pi SD card benchmarks because they use SD Cards
        * Specifically look at best 4k random write - faster is better.
  * Details:
    * First step: hp85disk emulator writes to the SD Card
    * Internally the SD card finds an internal page where our data will go
    * Next the SD Card reads the page into internal RAM (recall it can be much over a megabyte)
    * Next the SD Card modifies the internal RAM with our data
    * Next the SD Card erases the page
    * Lastly the page is written to the SD Card and this takes time
      * Trivia - SD cards must to erase a page before updating because of the flash memory cell design
  * Why is this delay critical to the hp85disk emulator?
    * Too long of a delay can cause a timeout when writing to disk
    * SD cards internal hardware is mostly optimized for sequential writing 
      * Their buffers and timing are designed primarily for writing consecutive blocks one after another
        * When writing in consecutive order they can queue up many requests and combine them into one operation - a huge savings in time - done all in hardware
        * Writing to blocks in random locations break this optimization very badly
          * Therefore some SD Cards can take so long the **HP85** can timeout
      * The hp85disk emulator does not have enough memory to work around this issue
        * If we could load the entire disk image into ram AND we had more for many write buffers
          * Then we could optimize the SD Card writing process to avoid the problems
        * The AVR we use has only 20K of ram for everything
          * Perhaps some day we will port this project to a Raspberry PI with add on hardware
            * The protocol is already solved for the emulator so this would not be that hard - only a battle with the time I have free

### SD Card problems 
  * The the file system on the SD card might get corrupted if it was powered on while removing the card
    * **Symptoms - the hp85disk will not detect any drives**
      * This is because the small FatFS file system code I used can not self repair any filesystem errors
  * If you suspect this the easy way to check is to connect the emulator to a desktop/laptop computer
    * See [Firmware updating and connecting to the hp85disk emulator with MINIMAL software install](#firmware-updating-and-connecting-to-the-hp85disk-emulator-with-minimal-software-install)
  * When powering on the emulator it will report a details about the SD Card - you will see very obvious errors if it can not read the SD Card
  * If the emulator says it can not open the card then go to the **SD Card filesystem repair sections**

### SD Card filesystem repair
  * **Windows** insert the Micro SD card into you desktop/laptop Card Reader
    * Windows might detect errors and offer to fix them - answer **yes**
    * You can make Windows Check the drive using the following steps
      * Open *File Explorer* **Right Click** on the SD card drive and pick **properties**
      * Open the Tools tab
      * Click on Error Checking 
  * **Ubuntu Desktop with GUI** insert the Micro SD card into you desktop/laptop Card Reader
    * Under **Activities** search for **Disks**
      * Open **Disks** You will see a list of all of your drives on the left panel
        * Not - your SD Card may look like a USB device if you use a USB card reader
      * Click on the correct drive in the left panel
        * You will notice it give the size and partition type
      * Click on the Volume in the right panel
      * Click on the settings gear icon below the Volume
      * Click Repair filesystem
  * **Ubuntu Command line - without GUI** 
    * **sudo bash**
	* Method 1
      * Insert the Micro SD card into you desktop/laptop Card Reader
      * **dmesg | tail -25**
        * You will see the device partition listed in the last 10 or 20 lines
        * We will use: **/dev/sdc1** in the example **MAKE SURE YOU USE THE ONE YOU DETECTED**
      * **umount /dev/sdc1**
      * **fsck -f /dev/sdc1**
	* Method 2
    * **udevadm monitor --udev**
      * Insert the Micro SD card into you desktop/laptop Card Reader
      * The "add" lines will have the card partition listed on the far right
        * We will use: **/dev/sdc1** in the example **MAKE SURE YOU USE THE ONE YOU DETECTED**
      * Press **Ctrl c** to exit udevadm monitor
      * **umount /dev/sdc1**
      * **fsck -f /dev/sdc1**

___ 


## SD Card file manipulation commands 
  * **posix help** displays a list of these SD cards related commands
    * **NOTE: you do NOT have to put the "posix" keyword prior to any of these command**
  * These are very useful for backing up and copying disk images and configuration files
  * Path names may optionally be fully qualified file names or relative names
  * Directory seperators may be / or \
  * Relative names can include . or ..
    * Examples
       * .. is previous directory
       * .  is current directory
       * / or \ is a directory separator
       * /configs/../amigo0.lif is the same as /amigo0.lif
  * Note: Wild card pattern matching is NOT implemented on any of these commands.

### SD Card file command summary with examples
  * **cd dir**
    * Change to new working, default,  directory
<pre>
    cd /configs
</pre>

  * **pwd**
    * Display current working, default,  directory
<pre>
    pwd
    /configs
</pre>

  * **ls files and or directories**
    * You can specify any number of files or directory names or files as arguments
      * This is limited to a total length of 250 characters total
    * File information displayed includes type size date and name 
      * The file and directory information displayed is similar to the Linux **ls -la** command
    * Directory listing includes the file count for each directory
    * Example: 
<pre>
    ls /notes /backups /amigo0.lifg
    Listing:[/notes]
    -rwxrwxrwx none none         4151 Mon Mar  2 00:19:16 2020 drive-parameters.txt
    -rwxrwxrwx none none          563 Sun Mar  1 22:19:48 2020 GETSAVE-Readme.txt
    -rwxrwxrwx none none       367277 Sun Mar  1 22:08:44 2020 GETSAV Series80 Group Data Communications Pac for the HP 83_85.pdf
    -rwxrwxrwx none none       312128 Sun Mar  1 22:09:48 2020 GETSAV Series80 Group HP85 I_O rom Image specifier error 52.pdf
    -rwxrwxrwx none none         2560 Sun Mar  1 03:57:42 2020 GETSAVE.#e008
    -rwxrwxrwx none none         3512 Sat Feb 29 07:19:34 2020 drives_parameters.txt
    -rwxrwxrwx none none          102 Sun Mar  1 07:20:12 2020 hpdir-notes-examples.txt
    Files: 7
    Listing:[/backups]
    -rwxrwxrwx none none         3416 Fri Dec 31 19:17:44 1999 amigo.cfg
    -rwxrwxrwx none none         5690 Fri Dec 31 19:18:00 1999 hpdisk.cfg
    Files: 2
    Listing:[/amigo0.lif]
    -rwxrwxrwx none none       286720 Mon Apr 13 00:03:22 2020 amigo0.lif
    Files: 1
</pre>
  * **mkdir dir**
    * Make a new directory called dir
  * **rmdir dir**
    * Delete a directory - it must be empty
  * **rm file**
    * Remove/Delete a file
  * **copy file1 file2**
    * Copy an existing file to a new location and name
<pre>
	mkdir /backups
	copy /amigo.cfg /backups/amigo.cfg
    Opening /amigo.cfg
    Creating /backups/amigo.cfg
    
    Copying...
    3416 bytes copied.
    
    copy /hpdisk.cfg /backups/hpdisk.cfg
    Opening /hpdisk.cfg
    Creating /backups/hpdisk.cfg
    
    Copying...
    5690 bytes copied.
</pre>
  * **rename old new**
    * Rename a file - this is similar to a move command source and destination paths and names can differ
  * **page NN**
   * **NN** is the number of lines to display per page with the **cat** with the page **-p** option
   * Default is 25 lines
  * **cat file [-p]**
    * List a file to the display with optional pausing
    * **-p** page option tells **cat** to pause after the number of lines specified by the **page** command
    * When pausing, typing Space advances another page, Enter another line, q exits

___ 


## LIF tools are built into emulator firmware 
  * Built in help
    * **lif help**
      * Gives lif commands
  * Summary - List, Create, import, export, translate, copy, rename, delete, etc
  * NOTE: Each disk image is a single file, encoded in **LIF** format, saved on the SD Card
    * **LIF** format is a common the filesystem on series 80 computers.
    * **LIF** format is also a vary common file interchange format for series 80 computers
      * **LIF** format includes file date, size permissions and other important meta data
  * Translate between DTA8x (type E010) and plain text files
      * You can add a plain text file, and translate it, into a **LIF** image with file format DTA8x (type E010)
      * You can extract and translate DTA8x (type E010) into a plain text files
  * **Key LIF manipulation features**
    * **dir** display a directory of a **LIF** image
      * Directory listing of **LIF** images and SD Card files
      * If you have an RTC the listing can display file and **LIF** volume date and time
        * Display time stamps if they were set
          * But only if they were created or added with the built in tools
    * **add** an ascii file to **LIF** image
      * This function permits renaming of the translated file
      * They get translated between **HP85** DTA8x (type E010) format and plain text files!!!
    * **extract** ASCII files from **LIF** image
      * This function permits renaming of the translated file
      * They get translated between **HP85** DTA8x (type E010) format and plain text files!!!
    * **addbin** add binary programs from one **LIF** image to another *LIF* image
      * This function permits renaming of the translated file
    * **extractbin** a single binary file or program into a new **LIF** image
      * This function permits renaming of the translated file
      * Extracted **LIF** images contain a single file a 256 byte volume header, 256 byte directory followed by a file.
    * **del** delete file in **LIF** image
    * **rename** file in **LIF** image
    * **create** create a LIF image specifying a label, directory size and overall disk size
    * **createdisk** create a LIF image specifying a label and drive model name
  * [For more **LIF** documentation](lif/README.md)

## TeleDisk to LIF conversion tool (updated) - see [LIF README.md](lif/README.md)
  * [td02lif](lif/t202lif) [85-SS80.TD0](lif/85-SS80.TD0) [85-SS80.LIF](lif/85-SS80.LIF)
  * NOTE: There are a stand alone version of the tool that run on Linux - making it work on Windows should be easy
      * You can extract a **DTA8x** (type E010) file from a **LIF** image and translate it into plain text
  * TeleDisk to **LIF** image conversion - a very common disk interchange format
    * See the top of [lifutils.c](lif/lifutils.c) for full documentation and examples.
    * create **LIF** image with options
    * NOTE: the emulator automatically creates missing images if defined in [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg)
      * Type "lif help" in the emulator for a full list of commands
      * See the top of [lifutils.c](lif/lifutils.c) for full documentation and examples.

## Disk images and default configuration file for the hp85disk project
  * [sdcard folder has premade **LIF** disk images](sdcard)
    * [sdcard/create_images.sh](sdcard/create_images.sh)  creates the default **LIF** images and creates a matching default configuration files
  * [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) contains the default disk definitions that correspond to the **LIF** images - disk hardware definition
    * NOTE: The file paths can include subdirectory paths in the file name. This permits using multiple folders

## Note about LIF images and [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) disk definitions
  * To create/modify or update **LIF** images see the section on the LIF utilities supplied with the emulator
  * It is important that the **LIF** image size match the disk definitions
    * The emulator gets the hard limits for  disk using [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg)
      * The attached computer requests these disk details from the emulator 
    * Then the attached computer reads the disk **LIF** headers for the *LIF* layout information. 
      * So as long as the **LIF** headers and hardware information match things should work fine.
        * IF the do not match you may get errors when
          * If the **LIF** image is BIGGER then specified disk AND if the computer attempts to read outside the defined limits.
  * The emulator does not look at the **LIF** data when serving and image - that is up to the attached computer.
   * The computer also gets the disk description from the emulator when it scans for disks

___ 


## Specific details for Translating between plain text and HP85 BASIC programs using hp85disk
  * Requirements
    * hp85disk emulator 
      * There is a solution documented below in you do not have the hp85emulator
    * A HP utility commonly called "GETSAV" translates between **HP85** BASIC and DTA8x (type E010) files
      * The utility adds functions "GET" and "SAVE" to your **HP85** computer
        * This HP utility was part of a larger software package from HP for the **HP85**

## GETSAV is called GETSAVE inside my LIF disk images
  * Here is a direct link to *GETSAVE.LIF* - which is encoded as a **LIF** file: [sdcard/LIF-files/GETSAVE.LIF](sdcard/LIF-files/GETSAVE.LIF)
    * Can be installed on the **HP85** with using **LOADBIN "GETSAVE"**
  * My hp85disk tools built in the firmware can tools translate between DT8x (type E010 ) and plain text ASCII files
    * I provide stand alone **LIF** tools in the lif subfolder of the github project that do the same thing
  * Used together with the **HP85** these tools can translate between ASCII plain text and *HP85* BASIC programs!!!
    * Specific details steps are documented later in the README

## Translating between plain text and HP85 BASIC programs WITHOUT hp85disk
  * You must compile and install the stand alone lif tools found under the project [lif](lif) folder
  * You will also need a way of transfer binary files to/from your **HP85** 
    * You need to copy the **GETSAVE** program to your **HP85** some how
  * Specific details steps are documented later in the README


___ 



## FULL example step by step translating bewteen plain text files and HP85 BASIC programs
  * You can do these examples without out my emulator but requires an extra tools and steps

## Importing ASCII and plain text as HP85 BASIC programs
  * Lets import a text file with BASIC statements into one of the emulator images **amigo1.lif**
  * Turn off both the **HP85** and emulator - if it is attached
    * Remove the SD Card
  * Create a plain text file with BASIC statements in it **on your Desktop - NOT on HP85**
      * **10 DISP "HELLO WORLD"**
      * **20 END**
    * Save this file as **TEST.txt** onto the SD Card and exit your editor 
      * umount the SD Card "eject it" in windows jargon
      * Reinstall the SD Card in the emulator
  * Turn on the hp85disk emulator FIRST - THEN turn on the **HP85**
    * Trivia - the **HP85 only detects disk at power on or after a RESET** 
      * **Therefore the emulator MUST be running first**
    * Open your serial program with the documented settings

  * Lets add **TEST.txt** from the SD Card into the **amigo1.lif** emulator disk image
    * Type:
      * **lif add amigo1.lif TEST TEST.txt**
        * **TEST** is the internal **LIF** name, **TEST.txt** is you source file

  * On your **HP85** we will load a binary program called **GETSAVE**
    * **LOADBIN "GETSAVE"**
      * Note: quotes are always required on **HP85** BASIC file names
      * This installs **GETSAVE** into program memory on your **HP85** until reset or power cycle
        * **GETSAVE** adds new functions **"GET"** and **"SAVE"** to your **HP85** computer
  * On the **HP85** type
    * **GET "TEST"**
      * Wait until you see **DONE**
        * On BIG programs GET can take a very long time 
          * The slow speed is a **GETSAVE** limitation and not due to my emulator speed
    * Lets save it as a normal **HP85** basic program
      * **STORE "TESTB"**
        * Saving in this format makes a totally HUGE difference in speed for BIG programs
        * In the future you can use **LOAD "TESTB"**

## Exporting HP85 BASIC programs to ASCII plain text
  * You must have the hp85disk emulator power ON and attached to you **HP85**
    * Turn on the **HP85**
  * On your **HP85** we will load a binary program called **GETSAVE**
    * **LOADBIN "GETSAVE"**
      * Note: quotes are always required on **HP85** BASIC file names
      * This installs **GETSAVE** into program memory on your **HP85** until reset or power cycle
        * **GETSAVE** adds new functions **"GET"** and **"SAVE"** to your **HP85** computer
  * Lets load a normal **HP85** BASIC program
    * **LOAD "RWTESTB"**
  * First stage conversions
    * **SAVE "RWTESTA"**
      * Wait until you see **DONE**
        * On BIG programs SAVE can take a very long time 
          * The slow speed is a **GETSAVE** limitation and not due to my emulator speed
  * Lets export and convert **"RWTESTA"** to plain text using the emulator command prompts
    * Type:
      * **lif extract amigo1.lif RWTESTA RWTEST.txt**
        * **TEST** is the internal **LIF** name, **TEST.txt** is you source file
      * This saves the file **RWTEST.txt** to the SD Card and plain text ASCII
    * Later you can copy the **RWTEST.txt** file to your desktop
      * Turn off the **HP85** and the emulator
      * Remove the SD Card
      * Attach to your desktop and copy it off

___ 

## Tested Operating systems
### Linux
  * I used **Ubuntu 18.04,16.04LTS and 14.04LTS** when developing the code
  * I have instructions for flashing the firmware below
### Windows
  * **Windows 10** running Windows Subsystem for Linux (Called WSL ) and the Ubuntu App from the Microsoft App store
-OR-
  * **Windows 10** with the Python 3.7 Microsoft App store


___ 


## Firmware updating and connecting to the hp85disk emulator with MINIMAL software install
  * **NOTE: The examples below refer to files that are part of this hp85disk project**
  * You need the following files **listports.py, flasher.py and gpib.hex** - here are the links
    * https://raw.githubusercontent.com/magore/hp85disk/master/uploader/listports.py
    * https://raw.githubusercontent.com/magore/hp85disk/master/uploader/flasher.py
    * https://raw.githubusercontent.com/magore/hp85disk/master/release/build/gpib.hex
  * **Please update any paths to suit where you download them to**

## Software requirments
### Linux
  * **apt-get install python3**
    * Most modern Linux systems have Python3
  * **pip3 install pySerial**

### Windows
  * Windows - Install Python 3.7 from Windows App Store
  * Note: if the python install fails to run and you get an error **VCRUNTIME140.dll**
    * You have a problem with the Visual C++ runtime files 
    * For a solution: https://www.winhelponline.com/blog/fix-vcruntime140-dll-missing-error-windows/
  * Open Power Shell window - always use Power Shell under Windows for running Python3
    * **pip3 install pySerial**

### Windows users that have an external ISP
  * You can also follow the notes under [release/build/windows/README.md](release/windows/README.md)
  * [release/windows](release/windows) includes avrdude and the required scripts for minimal flashing of the firmware
   
## Discover your serial port name
  * Make sure you have a mini usb cable handy
  * **Make sure the emulator mini-usb cable is NOT connected to your PC/Mac yet**
  * **Make sure the hp85disk is powered on**

### Linux
  * Note where you have saved [listports.py](uploader/listports.py) and update the path below to suit
  * Open a terminal window
    * Run the following command 
    * **python3 uploader/listports.py**
    * Attach the emulator and rerun the command 

### Windows
  * Note where you have saved [listports.py](uploader/listports.py) and update the path below to suit
  * Open a Power Shell window
    * Run the following command 
    * **python3 uploader\listports.py**

### Connect the emulator  to discover the port name
  * Note where you have saved [listports.py](uploader/listports.py) and update the path below to suit
  * **Make sure the hp85disk is powered on**
  * Attach the hp85disk miniusb cable to your computer and rerun the [listports.py](uploader/listports.py)
  * **Note: The new port that appeared is the emulator port**
    * Linux example:   **/dev/ttyUSB0**
    * Windows Example: **COM3**
  * Note: The emulator uses the following port settings
    * **BAUD rate 115200**
    * **Data bits: 8 Data bits NO parity**
    * **Flow control NONE**

___ 

## Connecting to hp85disk interactive serial port
  * Note: Use the port discovered in the previos section
  * **Note: serial.tools.miniterm is a library installed by pySerial - you do not need to specify a path to it**
  * You will need the Python **pySerial** package documented in the previous steps
  * Replace the **/dev/ttyUSB0** port with the one you discovered previously
  * **python3  -m serial.tools.miniterm --parity N --rts 0 --dtr 0 /dev/ttyUSB0 115200**
    **Ctrl+]** - will exit **hold down Ctrl and press ]**
    * **serial.tools.miniterm** does not need a beginning directory path - when the package is installed Python knows where to look
    * Windows users can use the the standalone Python 3.7 from the Microsoft App store with Python package pySerial installed
    * Note: **serial.tools.miniterm** does **NOT work on Windows WSL Ubuntu** - yet so use the term script found in the hp85disk github main folder
  * For a help menu type **help**
    * Most of the commands listed by help also have help of there own 
      * Example: **lif help**
    * There is a full list of the commands later in this README
  * References
    * [Firmware updating and connecting to the hp85disk emulator with MINIMAL software install](#firmware-updating-and-connecting-to-the-hp85disk-emulator-with-minimal-software-install)
    * [Command interface notes}(#command-interface-notes)
    * [hp85disk troubleshooting with serial terminal and DEBUG option](#hp85disk-troubleshooting-with-serial-terminal-and-debug-option) to verify your HP85 is communicating with your emulator

___ 


## Updating hp85disk firmware with built in bootloader
  * Note where you have saved [flasher.py](uploader/flasher.py) and [gpib.hex](release/build/gpib.hex) and update the path below to suit
    * You can download them using the direct links in the previous section 
    * **MAKE sure you download these files in github RAW formwat**
  * Only do this if recommended
  * Note: The github hp85disk master branch project includes disk images and precompiled firmware
    * Compiled Firmware hex file [release/build](release/build) 
    * SD Card Disk Images        [release/sdcard](release/sdcard)
  * [I have provided compiled HEX files under the folder release/build](release/build)
  * References:
    * [Firmware updating and connecting to the hp85disk emulator with MINIMAL software install](#firmware-updating-and-connecting-to-the-hp85disk-emulator-with-minimal-software-install)
    * [Connecting to hp85disk interactive serial port](#connecting-to-hp85disk-interactive-serial-port)

### Flashing notes
  * In the following update example you should see messages similar to message below 
  * **It is not that uncommon that you will have to try more then once**
    * **If you have problems please see the "Firmware update problems" sections below**
  * Here is what you should expect to see as flashing starts
  * Example flashing messages: python3 flasher.py 115200 /dev/ttyUSB0 gpib.hex
   * Update your port name to suit your OS, and update the paths to the flasher.py and gpib.hex if needed
<pre>
[STK500] Get sync
Connected to bootloader
[STK500] Get parameter 81
Major: 8
[STK500] Get parameter 82
Minor: 1
[STK500] Read signature
Signature: 0x1e9705
MAXADDR: 0x010000, OFFSETADDR: 0x000000, BASEADDR: 0x010000

data[0]: 0x10, data[1]: 0x00

MAXADDR: 0x01e148, OFFSETADDR: 0x000000, BASEADDR: 0x010000

program size: 123208

[STK500] Load address 000000
...
And so on until the end
...
[STK500] Load address 01e100
[STK500] Load address 01e200
[STK500] Leaving programming mode

</pre>

### Linux firmware update example
  * Note where you have saved [flasher.py](uploader/flasher.py) and [gpib.hex](release/build/gpib.hex) and update the path below to suit
  * Run the following command 
    * **python3 uploader/flasher.py 115200 /dev/ttyUSB0 release/build/gpib.hex**

### Windows firmware update example
  * Note where you have saved [flasher.py](uploader/flasher.py) and [gpib.hex](release/build/gpib.hex) and update the path below to suit
  * Run the following command 
    * **python3 uploader/flasher.py 115200 COM3 release/build/gpib.hex**

### Mac firmware update example
  * Note where you have saved [flasher.py](uploader/flasher.py) and [gpib.hex](release/build/gpib.hex) and update the path below to suit
  * Run the following command 
    * **python3 flasher.py /dev/tty.usbserial-AB0KMQCH gpib.hex**

### Firmware update problems - if you get a failure during updating
  * **ALways try the flasher.py script at least twice before attempting this proccedure**
  * Type in the flashing command, see  above, but *without* pressing Enter yet
    * **Now hold down RESET on the hp85disk board - now release RESET and press Enter quickly**
      * **You have a short window (about a second) after releasing RESET to Press Enter**
      * ** You may have to try this a few times to get the timing right**

___ 


## Requirements for compiling and flashing the FULL hp85disk project
  * These steps, below, are required ONLY if:
   * You plan to build the standalone **lif** and **mkcfg** utilities
   * You plan on making code changes
   * You wish to use an **In System Programmer (ISP)** that requires **avrdude** to work
  * [I have provided compiled HEX files under the folder release/build](release/build)
    * You just need to flash the files - see MINIMAL notes above


## Full installation Ubuntu and Windows software for building hp85disk and building related applications
Note: If you only plan on updating firmware and would rather not compile skip to Firmware updating below

### Windows 10 install Ubuntu Subsystem for Linux and Ubuntu App
Note: I recommend this as the easiest way to compile and build hp85disk under Windows</br>
  * Following these steps take less then 10 minutes including the software and hp85disk download
  * Install **WSL - Windows Subsystem for Linux**
    * See: https://docs.microsoft.com/en-us/windows/wsl/install-win10
    * See: https://wiki.ubuntu.com/WSL
    * Using the GUI for enabling Windows features
      * Open the Start Menu and search **Turn Windows features on or off**
      * Select **Windows Subsystem for Linux**
      * Click **OK**
      * Restart your computer when prompted

### Installing Ubuntu 18.04 LTS App
Note: **Please use the 18.04 Ubuntu App - NOT the 20.04 - as it is issues**
  * Open **Windows Store**
    * Install **Ubuntu 18.04 App**
      * Say NO use use App accross all of your devices
        * Note: you DO NOT have to sign in to the Windows Store to install this, although that may not be obvious
        * If you just **X** out of the sign in prompt the software will still install
  * Configure Ubuntu 18.04 App: Search **Ubuntu** 
    * **Launch Ubuntu 18.04 App**
      * **Note: When launching the Ubuntu App for the very first time**
        * It will take a few minutes the first time depending on your network speed 
        * It will ask for a userid and password for a new user
    * Configure Ubuntu App
      * **Right Click on the very top of the Ubuntu App window**
        * **Click on Properties -> Open the Options Tab**
        * **Enable Ctrl+Shift+C/V Copy Paste**
        * **Close and Reopen Ubuntu App for the new settings to take effect**


### Automated software install and hp85disk github project download
Note: Depends on Windows WSL install and and Ubuntu 18.04 App install - see previous section
  * Open a Ubuntu 18.04 App terminal window
    * **Ubuntu App** under Windows
  * You can download an install script I create to do everything for you called **install_hp85disk,sh**
    * It will install all of the required software tools and download a copy of the github hp85disk project
    * Copy and Paste this command into your open Ubuntu App Terminal WIndows
    *  **wget https://raw.githubusercontent.com/magore/hp85disk/master/install_hp85disk.sh**
  * Running the **install_hp85disk.sh** script
  * **bash ./install_hp85disk.sh**
    * **Note: The install script will ask you for your login password**
      * This is the same password you just create when installing the Ubuntu App
      * **WHy? This is permits the script to install required software inside the Ubuntu App as the Administrator/root user**
    * **Note: This script may take 5 to 15 minutes depending on how slow your network and computer are**
  * You now have a directory called **hp85disk**
    * This directory is a full copy of the **hp85disk github project**

### Updating hp85disk github project at any time using git
#### Linux
  * Open a **terminal window**
  * **cd hp85disk**
  * **git pull**

#### Windows
  * Open a **Ubuntu App terminal window**
  * **cd hp85disk**
  * **git pull**

___ 


## Compile Firmware
Note: Change into the hp85disk folder created by the install.sh script</br>
  * **make clean**
  * **make**
  * **make install**
    * Installs **lif** and **mkcfg** tools

### Compile and updating Firmware - assumes you have all tools installed
  * Note: in the steps below you can override defaults
    * You just tack on settings like this to the end of the make command line
      * **AVRDUDE_ISP=avrisp  ISP_PORT=/dev/ttyS4 HP85_PORT=/dev/ttyS3**
      * **AVRDUDE_ISP=arduino ISP_PORT=/dev/ttyS3 HP85_PORT=/dev/ttyS3**
  * Now attached the emulator miniusb port to your computer 
    * Make sure you have installed the software in the previous steps and discovered the hp85disk serial port

### Flash with Internal optiboot
  * Note: in this example assume the hp85disk serial port is /dev/ttyS3
    * You can override the [Makefile](Makefile)  default without editing it
  * **make clean**
  * **make**
  * **make install**
    * This installs utilities like lif and mkcfg
  * **make flash HP85_PORT=/dev/ttyS3**
    * This overrides the two settings in the Makefile
      * Alternatively you can edit the Makefile and change the two settings for your setup
  * Note: If You want to install the release firmware that I verified and uploaded to github
    * **make flash-release  HP85_PORT=/dev/ttyS3**
      * This overrides the two settings in the Makefile
        * Alternatively you can edit the Makefile and change the two settings for your setup

#### If you have a firmware update problems - if you get a failure during updating
  * Type in the flashing command, see  above, but **without pressing Enter yet**
    * Now hold down **RESET** on the hp85disk board - release RESET and press Enter quickly
      * You have a short Window after releasing RESET to Press Enter

### External programmers
  * **avrisp** is a very low cost arduino based *In System programer* **ISP** 
    * Also called **Arduino as ISP** 
    * This uses a low cost arduino atmega328P and has source code provided with the Arduino platform itself
      * See: https://www.arduino.cc/en/tutorial/arduinoISP
        * Example: Search for **arduino nano 3.0** cost about $10 and has small formfactor.
        * You do not want the USB version as the software is different
      * Program your arduino using the **Arduino as ISP example**
    * Connections to hp85disk ISP header:
<pre>
       Arduino Pin  hp85disk ISP header Pin
       D12          1
       5V           NC = Not Connected! see note
       D13          3
       D11          4
       D10          5
       GND          6
Note 5V connection - DO NOT CONNECT 
     Given the hp85disk and arduino are both powered with their own supplies it would be bad to connect them
</pre>
  * **avrispv2** is another very low cost *In System programer* **ISP** about $10
    * Low cost Pololu USB AVR Programmer V2.1 
    * https://www.robotshop.com/ca/en/pololu-usb-avr-programmer-v21.html

### Update Firmware with external programmer
  * You will need and AVR programmer supported by **avrdude** (part of avrtools)
    * See [Makefile](Makefile) keywords **AVRDUDE_ISP** and **ISP_PORT** in next [Makefile](Makefile) section
    * Lets assume 
      * External programmer is attached as **/dev/ttyS4**
      * hp85disk is attached as **/dev/ttyS3**
  * **make clean**
  * **make**
  * **make install**
  * **make flash-isp-release**
  * Examples with other programmers
    * **make flash-isp-release AVRDUDE_ISP=avrisp 	HP85_PORT=/dev/ttyS3   ISP_PORT=/dev/ttyS4**
    * **make flash-isp-release AVRDUDE_ISP=avrispv2	HP85_PORT=/dev/ttyUSB0 ISP_PORT=/dev/ttyACM0**
    * OR
  * **make flash-isp** 
    * This will use **avrdude** and your ISP (In System Programmer) to flash the firmware

### Flashing AND connecting to hp85disk emulator terminal just after firmware update 
  * **Note: You can add *term* after *ANY* make flash commands**
  * Examples:
    * **make flash-release     term**
    * **make flash-isp-release term AVRDUDE_ISP=avrispv2     HP85_PORT=/dev/ttyUSB0 ISP_PORT=/dev/ttyACM0**
  * **ISP_PORT** must be set to the programmer port name
  * **HP85_PORT** must be set to the hp85disk emulator serial port name

___ 


## Makefile configuration options for hp85disk emulator 
  * These options apply to original V1 and new V2 boards
  * **make config** will list all of these - example listing in next section
  * Update **BAUD, HP85_PORT, BOARD, PPR_REVERSE_BITS and LCD_SUPPORT** for your platform
    * **AVRDUDE_DEVICE** is the name of AVR as it is known by **avrdude**
      * **m1284**
    * **ISP_SPEED**  is the ISP programming clock speed supplied by **avrdude**
      * **5**
        * My device works with 0.25 but 5 is safe
    * **AVRDUDE_ISP**
      * You will need and AVR programmer supported by **avrdude** (part of avrtools)
        * The built in **optiboot** loader is called **arduino**
        * You can list all of the supported programmers using the command **avrdude -c list**
          * Note: **ISP** = *In System Programmer*
          * **avrdude device programmer name as known by avrdude**
            * **avrdude -c list**  # for a list of devices
          * See [Makefile](Makefile) keywords **AVRDUDE_ISP** and **ISP_PORT**
            * This is the ISP programmer and port names
            * FYI: I am using **atmelice_isp** [Makefile](Makefile) as the default
          * The cheapest ISP is the Arduino as ISP 
            * **AVRDUDE_ISP** = *avrisp*
              * There are many sources that sell this low cost programmer - Amazon has them for about $10 
              * FYI: The Arduino installer package includes the source for the Arduino ISP in the Example Programs if you want to make one
          * **Note: JTAG is disabled for this project so you can not use a JTAG programmer**
    * **ISP_PORT**
      * Depends on the ISP and operating system used
        * Example: on Linux it might be **/dev/ttyUSB1** for a **avrisp** and **arduino**
        * **usb** for **atmelice_isp** on Linux - the emulator hardware will typically have instructions
    * **HP85_PORT** is the hp85disk emulator serial HP85_PORT name as detected by your operating system
      * **/dev/ttyUSB0** on my system - Linux
    * **BAUD**  is the hp85disk emulator serial baud rate 
      * **115200** = a safe default that most systems can manage
        * NOTE: My development environment works with 500000 baud but I use 115200 for distribution and this project
        * NOTE: Faster is better when enabling more debug messages 
          * Too many messages can cause the **HP85** to timeout waiting for IO
    * **DEVICE**
      * **Target AVR device as known by GCC** for this project
      * **atmega1284p**
        * **DO NOT CHANGE THIS - there are too main dependencies**
    * **BOARD** is the version of the hardware currently V2
      * **1** = **V1 hardware** without **GPIB** BUS transceivers
      * **2** = **V2 hardware** with **GPIB** BUS transceivers (DEFAULT)
    * **PPR_REVERSE_BITS**
      * **Note: This is now automatically set by board revision so this is not normally needed**
        * **0** = **V1 hardware** without the **GPIB** buffers 
        * **1** = **V2 hardware** with **GPIB** buffers 
    * **LCD_SUPPORT**
      * **1** = **LCD support** for displaying messages
    * **F_CPU**  
    * **RTC_SUPPORT** for Real Time Clock is included with V2 boards
      * **1** = **RTC** support for a **DS1307** command compatible **RTC** chip - the **DS3231** is the 3.3V version
        * This will time stamp plot files and add time stamps inside lif images
        * The emulator will still work if this is set but not attache to an RTC
    * **F_CPU**  
      * CPU frequency - the firmware and V1/V2 boards use this so do not change it
        * **20000000**

___ 


## make configuration display 
  * **make config**
    * List current configuration defaults

<pre>
    Current Configuration Defaults
        You can override settings by adding assignments at the end of any make command
        Example:
                 make flash-isp ISP_PORT=/dev/ttyUSB0 AVRDUDE_ISP=avrisp HP85_PORT=/dev/ttyUSB0
    
        DEVICE                 = atmega1284p
        F_CPU                  = 20000000 
        BAUD                   = 115200
        HP85_PORT              = 
    
        AVRDUDE_DEVICE         = m1284
        AVRDUDE_ISP            = atmelice_isp
        ISP_PORT               = usb
        ISP_SPEED              = 5
    
        OPTIBOOT               = 1
        BOARD                  = 2
        PPR_REVERSE_BITS       = 1
        I2C_SUPPORT            = 1
        RTC_SUPPORT            = 1
        LCD_SUPPORT            = 1
    
        AMIGO                  = 1
        FATFS_SUPPORT          = 1
        FATFS_TESTS            = 1
        GPIB_EXTENDED_TESTS    = 0
        POSIX_TESTS            = 1
        POSIX_EXTENDED_TESTS   = 0
        LIF_SUPPORT            = 1
    
     GCC Verbose Options - show more detail while compiling
        VERBOSE                = 0
</pre>

___ 


## make help documentation
  * **make help**
    * List the common commands to compile/install/flash the code
    * See also **make config** section below
<pre>
Building Commands
    make install           - builds and installs all command line utilities
    make sdcard            - builds all sdcard images and creates default hpdisk.cfg and amigo.cfg files
    make release           - builds all code and copies files to the release folder
    make clean             - cleans all generated files
    make                   - builds all code

Listing current cunfiguration settings
    make config

Overriding any configuration settings
    You can add configuration values at the end of your make commands like this
    make flash-isp term         ISP_PORT=/dev/ttyUSB0 AVRDUDE_ISP=avrisp   HP85_PORT=/dev/ttyUSB0
    make flash-isp-release term ISP_PORT=/dev/ttyACM0 AVRDUDE_ISP=avrispv2 HP85_PORT=/dev/ttyUSB0
    make flash term             HP85_PORT=/dev/ttyUSB0
    make flash-release term     HP85_PORT=/dev/ttyUSB0

Note: Adding "term" after a make command will start a terminal to the hp85disk when finished

Programming using an 6 wire ISP - installs optiboot
    make install_optiboot  - install optiboot boot loaded using an ISP
    make flash-isp         - build and flash the code using an ISP
    make flash-isp-release - flash the release code using an ISP

ISP Programmer suggestions
    arduino                - This ISP is built into all recent hp85disk firmware using optiboot
    avrisp                 - Low cost arduino as ISP - see README.md
    avrispv2               - Low cost USB AVR ISP - such as Pololu USB AVR Programmer V2.1 - see README.md

Verify code using an 6 wire ISP - Just does a code verify against the gpib.hex file
    make verify-isp        - verify code using an ISP
    make verify-isp-release- verify release code using an ISP

Programming using the built in optiboot programmer
    make flash             - build and flash the code using built in optiboot programmer
    make flash-release     - flash the release code using built in optiboot programmer
    IMPORTANT - if flashing fails to start try these steps
        On your computer type in the "make flash" command without pressing Enter afterwards
        Next press RESET on the hp85disk board,  then release RESET, then quickly press Enter

Programming using an 6 wire ISP - WITHOUT installing optiboot
    IMPORTANT - you will not be able to use non isp flashing modes later on
       Makes booting and flashing process slightly faster
    make flash-isp-noboot         - build and flash the code using an ISP
    make flash-isp-noboot-release - flash the release code using an ISP

 GCC Verbose Options - show more detail while compiling
    VERBOSE                = 0
</pre>


## Example building with Makefile overrides
  * **make flash      AVRDUDE_ISP=arduino      ISP_PORT=/dev/ttyS3**
  * **make flash-isp  AVRDUDE_ISP=avrisp       ISP_PORT=/dev/ttyUSB0**
  * **make flash-isp  AVRDUDE_ISP=atmelice_isp ISP_PORT=usb**

___


## Building Doxygen documentation for the project - OPTIONAL
  * **aptitude install --with-recommends doxygen doxygen-doc doxygen-gui doxygen-latex**
  * *If you omit this you will have to update the [Makefile](Makefile) to omit the steps*

___ 


## Using the hp85disk emulator
  * Here we focus just on **HP85** BASIC commands 
  * See [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) for configuration settings and setting and documentation.
     * Printer capture is configured currently for my HP54645D scope
       * The following example works for an **HP85** attached to the emulator via **GPIB** bus.
         * **PRINTER IS 705**
         * **PLIST**
     * Disk images in [sdcard](sdcard) folder drive and configuration settings
       * **First  Amigo**   9121D disk at 710 for my **HP85A** (with 85A ROM's)
       * **Second Amigo**   9121D disk at 710 for my **HP85A** (with 85A ROM's)
       * **First   SS80** HP9134L disk at 720 for my **HP85B** (with 85B ROM's)
       * **Second  SS80** HP9134L disk at 730 for my **HP85B** (with 85B ROM's)

     * How to use the examples with your **HP85**
       * Copy the files inside the project [sdcard](sdcard) folder to the home folder of a fat32 formatted drive
         * All image files and configuration must be in the home folder only - not in a subdirectory.
         * You may store other user files in sub folders of your choosing.
       * Verify [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) configuration settings for your computer
       * Insert card into emulator
       * Attract **GPIB** cables
       * Power on emulator
       * Power on your computer last!
          * The emulator MUST be running and attached to your computer first!
          * The **HP85** ONLY checks for drives at POWER UP or RESET

## Testing examples on your HP85
  * Testing was done with an **HP85A** (with extended EMS ROM) 
    * Using the Hewlett-Packard Series 80 - **PRM-85** by Bill Kotaska
    * This makes my **HP85A** look like and **HP85B** 
      * I can also use the normal mass storage ROM if I limit to **AMIGO** drives.
      * http://vintagecomputers.sdfeu.org/hp85/prm85.htm
       * old site http://vintagecomputers.site90.net/hp85/prm85.htm

## Initializing a disk images
## HP85B only feature or HP88A with **PRM-85** board
  * IMPORTANT NOTE: formatting is done automatically by the hp85disk lif image creation commands
  * The **HP85B** and EMS ROM has extended INITIALIZE attributes
    * If you use the hP85 INITIALIZE commands it erases everything on the emulated image
    * You can however backup up copy existing **LIF** images to another folder on the SD Card for safe keeping
      * There is a built in copy command for this 
<pre>
  INITIALIZE "AMIGO1",":D700",14,1
  INITIALIZE "AMIGO2",":D710",14,1
  INITIALIZE "SS80-1",":D720",128,1
  INITIALIZE "SS80-2",":D730",128,1
</pre>
  
## HP85A and HP85B examples
  * Note lines with a "#' as the first non blank character are just my comments 
    * A bad habit from writing too many bash scripts 

<pre>
  # Listing files:
  # first AMIGO
  CAT ":D700"
  # second AMIGO
  CAT ":D710"
  # first SS80
  CAT ":D720"
  # second SS80
  CAT ":D730"
  
  # Loading file from first SS80:
  LOAD "HELLO:D720"
  # Copying file between devices: fist AMIGO to second AMIGO
  COPY "HELLO:D700" TO "HELLO:D710"
  # Copying ALL files between devices: FIRST SS80 to Second SS80
  COPY ":D720" TO ":D730"
  # LOAD the GETSAVE binary program very short example
  LOADBIN "GETSAVE"
  # This program stays in memory until the HP85 is reset
  # See all of the detailed notes earlier in the README 
</pre>
  * Now on the emulator itself type
  * **lif add amigo1.lif MYTEST test.txt**
     * See all of the detailed notes earlier in the README 
  * We just added the **TEST.txt** file to the image file called **amigo1.lif** and named it MYTEST
<pre>
  # Lets assume amigo1.lif is defined as device :D700 in the hpdisk.cfg file
  GET "HELLO:D700"
  # Save as a HP85 BASIC file in DTA8x (type E010) file
  PUT "HELLOA:D700"
  # Store it as as HP85 BASIC BAS8x (type E020) file
  STORE "HELLO2B:D700"
  # Now in the future we can LOAD it 
  LOAD "HELLO2B:D700"
  # How to Delete the file 
  PURGE "HELLO2B:D700"
  # List the BASIC file
  LIST
  # Clear memory
  SCRATCH
</pre>

___ 


## Understanding Drive GPIB BUS addressing and Parallel Poll Response (PPR) - HP85A vs HP85B
  * While **GPIB** devices can have address between 0 and 31 you can have no more than 8 disk drives.
  * ALL disk drives are required to respond to a **PPR** query by the (HP85) controller.
    * **PPR** query is done when the controller in charge (HP85) pulls **ATN** and **EOI** low.
    * **PPR** response occurs when a disk drive pulls one **GPIB** bus data line low in response.
       * You can only have 8 of these because there are only 8 **GPIB** data bus lines.
         * **GPIB** data bus bits are numbered from 1 to 8
         * **PPR** response bits are **assigned in reverse order starting from 8, bit 8 for device 0**
           * **This is a GPIB specification - not my idea**
             * The **HP85** uses these assumptions
  * **IMPORTANT! On power up the HP85 issues a PPR query for disk drives**
    * **The emulator must be running BEFORE this happens**
    * **PPR** query = both **ATN** and **EOI** being pulled low by the computer.
    * **PPR** response is when each drive pulls a single **GPIB** data bus bit **LOW** - while **ATN** and **EOI** are **LOW**.
       * *ONLY* those that are detected in this way are then next scanned
    * Next for all detected drives the **HP85** issues **Request Identify** to each in turn.
      * This is done one drive at a time in order
      * The **PPR** keyword in the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) is the **PPR** bit the drive uses
        * **PPR** of **0 = PPR** response on **GPIB** data bus **bit number 8** - as per **GPIB BUS specifications**.
      * The ID keyword in [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) is the 16 bit reply to "Request Identify Reply"
        * **IMPORTANT! **AMIGO** drives cannot be queried for detailed drive layout information**
          * The **HP85A** can only use its **hard-coded firmware tables** to map **ID** to disk layout parameters
          * This implies that the **HP85A** can only use **AMIGO** disks it has defined in firmware.
        * The **HP85B** can however; query newer **SS80** drives for detailed drive layout information instead.
        * The **HP85A** cannot use **SS80** drives unless it uses copies of the **HP85B** EMS and EDISK ROMS.
          * One way this can be done with the **PRM-85** expansion board offered by Bill Kotaska 
            * (The **PRM-85** is great product giving you access to all of the useful ROMS)
___

## Technical Limitations
 * Multiple drive support is implemented but multiple UNIT support is NOT
 * While most **AMIGO** and **SS80** feature have been implemented my primary focus was on the **HP85A** and **HP85B**.
   * (I do not have access to other computers to test for full compatibility)
   * This means that a few **AMIGO** and **SS80** **GPIB** commands are not yet implemented!
     * Some of these are extended reporting modes - many of which are optional.
   * Note: The **HP85A** can only use **AMIGO** drives - unless you have the **HP85B** EMS ROM installed in your **HP85A**
      * This can be done with the **PRM-85** expansion board offered by Bill Kotaska (a great product!)
 * To attach a drive to our computer, real or otherwise, you must know:
   * The correct **GPIB BUS address** and **parallel pool response (PPR)** bit number your computer expects.
     * See **ADDRESS**, **PPR** and **ID** values in [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg)
   * Older computers may only support **AMIGO** drives.
     * Such computers will have a hard coded in firmware list of drive its supports.
       * These computers will issue a *GPIB BUS* **Request Identify** command and only detect those it knows about.
       * *If these assumptions do NOT match the layout defined in the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) no drives will be detected.*
   * Newer computers with **SS80** support can request fully detailed disk layout instead of the "request identify"
   * My emulator supports both reporting methods - but your computer may not use them both!
     * For supported values consult your computer manuals or corresponding drive manual for your computer.
       * See gpib/drives_parameters.txt for a list on some known value (CREDITS; these are from the HPDir project)
     * In all cases the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) parameters MUST match these expectations.
   * The [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) file tells the emulator how the emulated disk is defined.
     * **GPIB BUS address**, **PPR bit number** and **AMIGO** **Request Identify** response values.
     * Additional detail for **SS80** drives that newer computers can use.
     * In ALL cases the file informs the code what parameters to emulate and report.
       * ALL of these values MUST match your computers expectations for drives it knows about.
   * Debugging
     * You can enable reporting of all unimplemented **GPIB** commands (see *TODO* debug option in [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg)
       * Useful if you are trying this on a non **HP85** device
       * See the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) for documentation on the full list of debugging options
     * The emulator can passively log all transactions between real hardware on the **GPIB** bus 
       * Use the **gpib trace logfile** command - pressing any key exits - no emulation is done in this mode.
         * **NOTE: on V2 boards , due to the GPIB buffers, we can not trace NDAC, NRFD and SRQ currently**
           * The GPIB buffers prevent looking at all signals at once
       * You can use this to help understand what is sent to and from your real disks.
       * I use this feature to help prioritize which commands I first implemented.
___


## hp85disk Terminal Commands
 * Pressing any key will break out of the gpib task loop until a command is entered
   * **help**
     * List all available commands 
     * Some commands also have their own help like *lif help*
     * **Note: all command options must be seperated with spaces between each item**

### For main help menu type *help*
 * Any word that has **help** after it will give help for that command
 * **help**
<pre>
	>help
	HP85 Disk and Device Emulator with built in LIF utilities
	 (c) 2014-2020 by Mike Gore
	 GNU version 3
	-> https://github.com/magore/hp85disk
	   GIT last pushed:   2020-06-17 19:52:32.792206282 -0400
	   Last updated file: 2020-06-18 12:13:25.589523676 -0400

	help     - displays this help menu
	fatfs     help
	posix     help
	lif       help
	gpib      help
	drives    help
	backlight 0xRRGGBB
	dir       directories list
	input   - toggle input debugging
	mem     - display free memory
	reset   - reset emulator
	setdate - set date
	time    - display current time
</pre>

### For lif help type *lif help*
 * **lif help**
<pre>
	lif add lifimage lifname from_ascii_file
	lif addbin lifimage lifname from_lif_file
	lif create lifimage label directory_sectors sectors
	lif createdisk lifimage label model
	lif del lifimage name
	lif dir lifimage
	lif extract lifimage lifname to_ascii_file
	lif extractbin lifimage lifname to_lif_file
		extracts a file into a sigle file LIF image
	lif rename lifimage oldlifname newlifname
	lif renamevol lifimage name
	Use -d  after 'lif' keyword to enable LIF filesystem debugging
</pre>


### For td02lif help type *td02lif help*
  * **NOTE: td02lif support is only available on the command line utility - NOT in firmware - too big**
<pre>
    td02lif help
    Usage: td02lif [options] file.td0 file.lif
           td02lif help
    tdo2lif options:
    Notes: for any option that is NOT specified it is automatically detected
             -s256|512 | -s 256|512 - force sector size
             -h1|2 | -h 1|2 - force heads/surfaces
             -tNN | -t NN  - force tracks
</pre>

### For posix help type *posix help*
 * **posix help**
<pre>
    Note: posix prefix is optional
    posix cat file [-p]
    posix cd dir
    posix copy file1 file2
    posix ls dir [-l]
    posix mkdir dir
    posix page NN
    posix pwd
    posix rm file
    posix rmdir dir
    posix rename old new
</pre>

### For drives help type *drives help*
 * **drives help**
<pre>
	>drives help
	mount
		 displays a list of mounted drives one per line
	mount AMIGO|SS80 model address file
		 Example: mount 9121  6 /amigo6.lif
		 Example: mount 9134D 2 /amigo2.lif
		 Note: drive model must exist in hpdir.ini [driveinfo] section
	mount PRINTER address
		 Example: mount PRINTER 5
	umount address     
         Example: umount 6

	addresses
	   Display all device GPIB bus addresses and PPR values
	config [-v]
	   Display current drives configuration
	   -v Verbose - show full detail
</pre>


### For gpib help type *gpib help*
 * **gpib help**
<pre>
	>gpib help
	gpib      help
	Note: gpib prefix is optional
	gpib debug N
	   debug message reporting see hpdisk.cfg for details
	gpib elapsed
	gpib elapsed_reset
	gpib ifc
	gpib task
	gpib trace filename.txt [BUS]
	   Display activity of GPIB bus and log it
	   BUS - include handshake states
OK
</pre>

### For fatfs help type *fatfs help*
 * **fatfs help**
<pre>
	>fatfs help
	Note: fatfs tests MUST start with "fatfs" keyword
	fatfs help
	fatfs ls directory
	fatfs status

	OK
</pre>
    
### For setting the time type *setdate* it will prompt for the date as shown below
 * This also sets the RTC
  * **NOTE: you may have to issue this command TWICE on the very first initialization**
 * **setdate**
<pre>
    Enter date YYYY MM DD HH:MM:SS >2020 04 09 16:54:00
    rtc seconds: 1586451240
    rtc time:    Thu Apr  9 16:54:00 2020
    clk seconds: 1586451240
    clk time:    Thu Apr  9 16:54:00 2020
</pre>

### For displaying the time and the rtc time type *time*
 * **time**
<pre>
    rtc seconds: 1586451317
    rtc time:    Thu Apr  9 16:55:17 2020
    clk seconds: 1586451317
    clk time:    Thu Apr  9 16:55:17 2020
</pre>

##  hp85disk setting debug options
  * Debugging is can be controlled in two ways
    * The **DEBUG** statement in [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg)
    * Typing the command in interactively

###  hp85disk DEBUG truth table
<pre>
  You can OR the following values together to add debug processing
  Values in the table are in HEX (base 16)
  Warning: Setting too many can cause HP85 timeouts while displaying messages
      1 ERRORS - all GPIB and device related error message
        # Note: Will not suppress Startup and configuration errors
      2 GPIB PPR Parallel Poll Response states
      4 GPIB BUS level single command byte decoded messages
      8 GPIB main loop - top level data and controll lines decoded
     10 GPIB TODO DEVICE missing support code
     20 GPIB Device Level states such as AMIGO,SS80,PRINTER
     40 GPIB Disk I/O read/write times
     80 GPIB read/write string timing
    100 GPIB read/write string byte decode
    200 GPIB try to detect Parallel Poll BUS state
    400 LIF utitilites debugging
</pre>

###  hp85disk setting debug examples
  * Interactively - **NOTE the spaces between the keyword, assignment and value**
    * **gpib debug = 0x11**
  * In the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) file
<pre>
    # Errors and TODO messages only
    # (1+10)
    DEBUG = 0x11
</pre>

  * Interactively
    * **gpib debug = 0x33**
  * In the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) file
<pre>
    # Main device states and errors only
    # (1+2+10+20)
    DEBUG = 0x33
</pre>

  * Interactively
    * **gpib debug = 0x3D**
  * In the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) file
<pre>
    # Most useful debugging messages
    # (1+2+8+10+20)
    DEBUG = 0x3D
</pre>

  * Interactively
    * **gpib debug = 0x51**
  * In the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) file
<pre>
    # Errors, TODO and SD Card Read/Write times
    # (1+10+40)
    DEBUG = 0x51
</pre>


### hp85disk troubleshooting with serial terminal and DEBUG option
  * Power on your HP85 - you should see
<pre>
	gpib_read_byte: IFC state=4
	<IFC>
	gpib_read_byte: IFC state=1
	<IFC>
</pre>
  * Enable GPIB BUS and Command debugging 
  * Enter the command **debug 0x3f** and press enter. 
    * You should see something like this - if you have more then 4 disks configured you will see a few extra DPPR messages
<pre>
	<INTERRUPT>
	>debug 0x3f
	debug=003FH
	[GPIB BUS_INIT]
	[GPIB STATE INIT]
	[PPR DISABLE ALL]
	[SS80 03H INIT]
	[DPPR bit:3, mask:00H]
	[SS80 02H INIT]
	[DPPR bit:2, mask:00H]
	[DPPR bit:1, mask:00H]
	[DPPR bit:0, mask:00H]
	OK
	[GPIB BUS_INIT]
	[GPIB STATE INIT]
	[PPR DISABLE ALL]
	[SS80 03H INIT]
	[DPPR bit:3, mask:00H]
	[SS80 02H INIT]
	[DPPR bit:2, mask:00H]
	[DPPR bit:1, mask:00H]
	[DPPR bit:0, mask:00H]
</pre>
  * Turn on your HP85 - you should see something like this
    [HP85 Power On GPIB BUS Debug Trace](trace/poweron-trace.txt)
  * Type **CAT** followed by RETURN on your HP85 you should see something like this
    [CAT GPIB BUS Debug Trace](trace/cat-trace.txt)



## Desktop Utility Commands
  * There are three standalone desktop utilities
  * **lif** is the same as the **lif** command in the hp85disk firmware
  * **td2lif** is the teledisk conversion utility
  * **mkcfg** displays a disk configuration in hpdisk.cfg format

### mkcf - display a disk configuration section in hpdisk.cfg format
  * Display disk configuration that you can use in the hpdisk.cfg file
<pre>
	mkcfg [-list]| [-m model [-b]|[-d]] [-a address]
	   -list lists all of the drives in the hpdir.ini file
	   -a hpdir address 0..7
	   -m model only, list hphpdir.cfg format hpdir configuration
	   -s short hphpdir.cfg format
	   -b only display block count, you can can use this with -m
	   -d only display computed directory block count, you can use this with -m
	   -f NAME specifies the LIF image name for this drive
</pre>
  * Example: Say you wanted to add a 9121 disk with address 0 and parallel poll bit 0
  * Then run: **mkcfg -s -m 9121 -f /amigo$D.lif -a 0 -p 0**
  * Copy the result into your hpdisk.cfg file - just make sure you have no conflicts
<pre>
	# HP9121 dual 270K AMIGO floppy disc
	# HP85 BASIC ADDRESS :D700
	AMIGO 9121
		HEADER
				# GPIB Address
			ADDRESS                 = 0
				# Parallel Poll Reponse Bit
			PPR                     = 0
				# LIF image file name
			FILE                    = /amigo.lif
		END
	END
</pre>


### lif utilities
  * **lif** is the same as the **lif** command in the hp85disk firmware
  * **lif help**
<pre>
	lif       help
	lif add lifimage lifname from_ascii_file
	lif addbin lifimage lifname from_lif_file
	lif create lifimage label directory_sectors sectors
	lif createdisk lifimage label model
	lif del lifimage name
	lif dir lifimage
	lif extract lifimage lifname to_ascii_file
	lif extractbin lifimage lifname to_lif_file
		extracts a file into a sigle file LIF image
	lif rename lifimage oldlifname newlifname
	Use -d  after 'lif' keyword to enable LIF filesystem debugging
</pre>
  * Note: the td02lif commands can be run inside the lif command or standalone

### td02lif  - Teledisk to LIF translator
  * **td2lif** is the teledisk conversion utility
  * **td02lif help**
<pre>
	td02lif help
	Usage: td02lif [options] file.td0 file.lif
		   td02lif help
	tdo2lif options:
	Notes: for any option that is NOT specified it is automatically detected
			 -s256|512 | -s 256|512 - force sector size
			 -h1|2 | -h 1|2 - force heads/surfaces
			 -tNN | -t NN  - force tracks

</pre>

___ 


# Abbreviations
Within this project I have attempted to provide detailed references to manuals, listed below.  I have included short quotes and section and page# reference to these works.
 * **SS80**
 * **CS80**
 * **A or Amigo**
 * **HP-IP**
 * **HP-IP Tutorial**

___

## Documentation References and related sources of information
 * Web Resources
   * <http://www.hp9845.net>
   * <http://www.hpmuseum.net>
   * <http://www.hpmusuem.org>
   * <http://bitsavers.trailing-edge.com>
   * <http://en.wikipedia.org/wiki/IEEE-488>
   * See [Documents folder](documents)


___


## Enhanced version of Tony Duell's lif_utils by Joachim
   * <https://github.com/bug400/lifutils>
   * Create/Modify **LIF** images


___


## CS80 References: ("CS80" is the short form used in the project)
   * "CS/80 Instruction Set Programming Manual"
   * Printed: APR 1983
   * HP Part# 5955-3442
   * See [Documents folder](documents)


___

## Amigo References: ("A" or "Amigo") is the short form used in the project
   * "Appendix A of 9895A Flexible Disc Memory Service Manual"
   * HP Part# 09895-90030
   * See [Documents folder](documents)

___


## HP-IB
   * ("HP-IB" is the short form used in the project)
   * "Condensed Description of the Hewlett Packard Interface Bus"
   * Printed March 1975
   * HP Part# 59401-90030
   * See [Documents folder](documents)


___


## Tutorial Description of the Hewlett Packard Interface Bus
   * ("HP-IB Tutorial" is the short form used in the project)
   * <http://www.hpmemory.org/an/pdf/hp-ib_tutorial_1980.pdf>
   * Printed January 1983
   * <http://www.ko4bb.com/Manuals/HP_Agilent/HPIB_tutorial_HP.pdf>
   * Printed 1987
   * See [Documents folder](documents)

___


## GPIB / IEEE 488 Tutorial by Ian Poole
   * <http://www.radio-electronics.com/info/t_and_m/gpib/ieee488-basics-tutorial.php>
   * See [Documents folder](documents)

___


## HP 9133/9134 D/H/L References
   * "HP 9133/9134 D/H/L Service Manual"
   * HP Part# 5957-6560
   * Printed: APRIL 1985, Edition 2
   * See [Documents folder](documents)

___


## *LIF* File system Format
   * <http://www.hp9845.net/9845/projects/hpdir/#lif_filesystem>
   * See [Documents folder](documents)

___


## Useful Utilities
   * [HPDir HP Drive - Disk Image Manipulation](http://www.hp9845.net/9845/projects/hpdir)
     * Copyright � 2010 A. K�ckes
   * [HPDrive Drive Emulators for Windows Platform](http://www.hp9845.net/9845/projects/hpdrive)
     * Copyright � 2010 A. K�ckes


## *GPIB* Connector pin-out by Anders Gustafsson in his hpdisk project
  * http://www.dalton.ax/hpdisk/


<pre>
    Pin Name   Signal Description       Pin Name   Signal Description 
    1   DIO1   Data Input/Output Bit 1  13  DIO5   Data Input/Output Bit 5 
    2   DIO2   Data Input/Output Bit 2  14  DIO6   Data Input/Output Bit 6 
    3   DIO3   Data Input/Output Bit 3  15  DIO7   Data Input/Output Bit 7 
    4   DIO4   Data Input/Output Bit 4  16  DIO8   Data Input/Output Bit 8 
    5   EIO    End-Or-Identify          17  REN    Remote Enable 
    6   DAV    Data Valid               18  Shield Ground (DAV) 
    7   NRFD   Not Ready For Data       19  Shield Ground (NRFD) 
    8   NDAC   Not Data Accepted        20  Shield Ground (NDAC) 
    9   IFC    Interface Clear          21  Shield Ground (IFC) 
    10  SRQ    Service Request          22  Shield Ground (SRQ) 
    11  ATN    Attention                23  Shield Ground (ATN) 
    12  Shield Chassis Ground           24  Single GND Single Ground
</pre>

___


## Main project files for hp85disk Project 
  * Project Main Home Directory
    * [main.c](main.c)
    * [main.h](main.h)
      * Main start-up code
    * [Makefile](Makefile)
      * Main Project Makefile
  * Terminal scripts
    * [miniterm.sh](miniterm.sh)
      * wrapper for miniterm.py part of the python package pySerial
    * [term](term)
     * Wrapper for minicom terminal emulator
  * Doxygen files
    * [Doxyfile](Doxyfile)
      * Doxygen Configuration files for project
    * [doxyinc](doxyinc)
      * Determines which files are included in the project Doxygen documents
    * [DoxygenLayout.xml](DoxygenLayout.xml)
      * Doxygen Layout file
  * Project Readme
    * [README.md](README.md)
      * Project README 
  * Project Copyright
    * [COPYRIGHT.md](COPYRIGHT.md)
      * Project Copyrights 

## Compiled firmware release files
  * [release](release)

## Board design file for version 1 and 2 hardware information
  * [board](board)
    * [V1](board/V1)
      * V1 Board documentation and Release files
      * [board design and pinouts of this project and a schematic PDF ](board/V1/HP85Disk.pdf)
      * [board design and pinouts of this project and a schematic DOC ](board/V1//HP85Disk.doc)
      * [board README.md](board/V1/HP85Disk.doc)
    * [V2/releases](V2/releases)
      * **Jay Hamlin version 2 circuit board design using GPIB buffers**

## Documents
  * [Documents](Documents)
  * **GPIB** BUS, HP device, **LIF** and chips documentation for this project
    * [Documents/README.md](Documents/README.md) 

## hp85disk software files
  * Most of the software in the project was written by me except where noted
  **NOTE - if you download any files from these links MAKE SURE YOU USE the github RAW viewing tab**
    * **If you do NOT use the RAW viewing tab yuou will download an anotated/markup version of the files**

## Compiled firmware release files
  * Has compiled firmware files and release SD Card images
    * [release firmware](release/build)
      * **gpib.hex** firmware
    * [release sdcard](release/sdcard)
      * A snapshot of the [sdcard](sdcard) folder at time of firmware release

## Uploader
  * Used for detecting serial ports and flashing firmware
  * [uploader](uploader)
    * [flasher.py](uploader/flasher.py)
      * Firmware flasher Python code
    * [flasher.sh](uploader/flasher.sh)
      * Firmware flasher shell wrapper - verifies parameters and HEX file exeistance
    * [listports.py](uploader/listports.py)
      * List serial ports - used for detecting the hp85disk emulator USB port
  
## FatFs
  * [fatfs](fatfs)
    * R0.14 FatFS code from (C) **ChaN**, 2019 - With very minimal changes 
    * [00history.txt](fatfs/00history.txt)
    * [00readme.txt](fatfs/00readme.txt)
    * [ff.c](fatfs/ff.c)
    * [ffconf.h](fatfs/ffconf.h)
    * [ff.h](fatfs/ff.h)
    * [integer.h](fatfs/integer.h)

  * [fatfs.hal](fatfs.hal)
    * R0.14 FatFS code from (C) **ChaN**, 2019 with changes
      * Hardware abstraction layer based on example AVR project
    * [diskio.c](fatfs.hal/diskio.c)
      * Low level disk I/O module glue functions (fatfs.hal/C)ChaN, 2016 
    * [diskio.h](fatfs.hal/diskio.h)
      * Low level disk I/O module glue functions (fatfs.hal/C)ChaN, 2016 
    * [mmc.c](fatfs.hal/mmc.c)
      * Low level MMC I/O by (fatfs.hal/C)ChaN, 2016 with modifications
    * [mmc.h](fatfs.hal/mmc.h)
      * Low level MMC I/O by (fatfs.hal/C)ChaN, 2016 with modifications
    * [mmc_hal.c](fatfs.hal/mmc_hal.c)
      * My Hardware abstraction layer code
    * [mmc_hal.h](fatfs.hal/mmc_hal.h)
      * My Hardware abstraction layer code
  
  * [fatfs.sup](fatfs.sup)
    * My fatfs support utility and POSIX wrapper test functions
    * [fatfs.h](fatfs.sup/fatfs.h)
      * FatFS header files
    * [fatfs_sup.c](fatfs.sup/fatfs_sup.c)
    * [fatfs_sup.h](fatfs.sup/fatfs_sup.h)
      * FatFS file listing and display functions
    * [fatfs_tests.c](fatfs.sup/fatfs_tests.c)
    * [fatfs_tests.h](fatfs.sup/fatfs_tests.h)
      * FatFS user test functions

## **GPIB** related
  * [gpib](gpib)
    * My **GPIB** code for **AMIGO**, **SS80** and **PRINTER** support
    * [amigo.c](gpib/amigo.c)
      * **AMIGO** parser
    * [amigo.h](gpib/amigo.h)
      * **AMIGO** parser
    * [defines.h](gpib/defines.h)
      * Main **GPIB** header and configuration options
    * [debug.txt](debug.txt)
      * List of debug flags
    * [drives.c](gpib/drives.c)
      * Supported Drive Parameters 
    * [drive_references.txt](gpib/drive_references.txt)
      * General Drive Parameters Documentation for all known drive types
    * [format.c](gpib/format.c)
      * **LIF** format and file utilities
    * [gpib.c](gpib/gpib.c)
      * All low level **GPIB** bus code
    * [gpib.h](gpib/gpib.h)
      * **GPIB** I/O code
    * [gpib_hal.c](gpib/gpib_hal.c)
      * **GPIB** hardware abstraction code
    * [gpib_hal.h](gpib/gpib_hal.h)
      * **GPIB** hardware abstraction code
    * [gpib_task.c](gpib/gpib_task.c)
      * **GPIB** command handler , initialization and tracing code
    * [gpib_task.h](gpib/gpib_task.h)
      * **GPIB** command handler , initialization and tracing code
    * [gpib_tests.c](gpib/gpib_tests.c)
      * **GPIB** user tests
    * [gpib_tests.h](gpib/gpib_tests.h)
      * **GPIB** user tests
    * [printer.c](gpib/printer.c)
      * **GPIB** printer capture code
    * [printer.h](gpib/printer.h)
      * **GPIB** printer capture code
    * [references.txt](gpib/references.txt)
      * Main **SS80**, **AMIGO** and **GPIB** references part numbers and web links
    * [ss80.c](gpib/ss80.c)
      * **SS80** parser
    * [ss80.h](gpib/ss80.h)
      * **SS80** parser
    * [notes.txt](gpib/notes.txt)
      * My notes on **GPIB** bus states as it relates to the project

## Hardware CPU specific 
  * [hardware](hardware)
    * My CPU hardware specific code
    * [baudrate.c](hardware/baudrate.c)
      * Baud rate calculation tool. Given CPU clock and desired baud rate, will list the actual baud rate and registers
    * [bits.h](hardware/bits.h)
      * BIT set and clear functions
    * [cpu.h](hardware/cpu.h)
      * CPU specific include files
    * [delay.c](hardware/delay.c)
    * [delay.h](hardware/delay.h)
      * Delay code
    * [hal.c](hardware/hal.c)
    * [hal.h](hardware/hal.h)
      * GPIO functions, spi hardware abstraction layer and chip select logic
    * [iom1284p.h](hardware/iom1284p.h)
      * GPIO map for ATEMEGA 1284p
    * [mkdef.c](hardware/mkdef.c)
      * Not used
    * [pins.txt](hardware/pins.txt)
      * AVR function to GPIO pin map
    * [ram.c](hardware/ram.c)
    * [ram.h](hardware/ram.h)
      * Memory functions
    * [rs232.c](hardware/rs232.c)
    * [rs232.h](hardware/rs232.h)
      * RS232 IO
    * [rtc.c](hardware/rtc.c)
    * [rtc.h](hardware/rtc.h)
      * DS1307 I2C RTC code
    * [spi.c](hardware/spi.c)
    * [spi.h](hardware/spi.h)
      * SPI BUS code
      * Unused
    * [i2c.c](hardware/i2c.c)
    * [i2c.h](hardware/i2c.h)
      * I2C code for display updates
    * [user_config.h](hardware/user_config.h)
      * Main include file MMC SLOW and FATS frequency and CPU frequency settings

## Common libraries
  * [lib](lib)
    * My Library functions
    * [bcpp.cfg](lib/bcpp.cfg)
      * BCPP C code formatting tool config
    * [matrix.c](lib/matrix.c)
    * [matrix.h](lib/matrix.h)
      * Matrix code - not used
    * [matrix.txt](lib/matrix.txt)
      *  A few notes about matrix operations
    * [queue.c](lib/queue.c)
    * [queue.h](lib/queue.h)
      * Queue functions
    * [sort.c](lib/sort.c)
    * [sort.h](lib/sort.h)
      * Sort functions - not used
    * [stringsup.c](lib/stringsup.c)
    * [stringsup.h](lib/stringsup.h)
      * Various string processing functions
    * [time.c](lib/time.c)
    * [time.h](lib/time.h)
      * POSIX time functions
    * [timer.c](lib/timer.c)
    * [timer.h](lib/timer.h)
      * Timer task functions
    * [timer_hal.c](lib/timer_hal.c)
      * Timer task hardware abstraction layer
    * [timetests.c](lib/timetests.c)
      * Time and timer test code


## *LIF* files
  * [lif](lif)
    * My **LIF** disk image utilities 
    * [lif/lifutils.c](lif/lifutils.c)
    * [lif/lifutils.c](lif/lifutils.c)
      * Functions that allow the emulator to import and export files from **LIF** images 
    * [Makefile](lif/Makefile)
      * Permits creating a standalone Linux version of the **LIF** emulator tools
    * Code by Mike Gore
      * [Makefile](lif/Makefile)
        * Make stand alone Linux versions of **LIF** utility and optionaly TeleDisk to *LIF* converter
      * [lifsup.c](lif/lifsup.c)
      * [lifsup.h](lif/lifsup.h)
        * Stand alone libraries for **LIF** and TELEDISK utility 
          * These functions are also part of various hp85disk libraries
      * [lifutils.c](lif/lifutils.c)
      * [lifutils.h](lif/lifutils.h)
        * **LIF** image functions, directory listing and file adding. extracting, renaming,deleting
      * [td02lif.c](lif/td02lif.c)
      * [td02lif.h](lif/td02lif.h)
        * My TeleDisk to **LIF** translator
      * [lif-notes.txt](lif/lif-notes.txt)       
        * My notes on decoding E010 format **LIF** images for HP-85
      * [README.txt](lif/README.txt)
        * Notes on each file under **LIF** and lif/teledisk
      * [85-SS80.TD0](lif/85-SS80.TD0) from hpmuseum.org
        * Damaged **SS80** Exerciser from HP Museum
      * [85-SS80.LIF](lif/85-SS80.LIF)
        * The current converter automatically did these repairs
          *  cyl 11, side 0 sector 116 mapped to 8
          *  cyl 13, side 0 sector 116 mapped to 11
          *  cyl 15, side 0 sector 10  missing - zero filled

## *LIF* teledisk files
   * [lif/teledisk](lif/teledisk)
     * My TELEDISK **LIF** extractor 
       * Note: The TeleDisk image MUST contain a **LIF** image  - we do NOT translate it
     * [README.txt](lif/teledisk/README.txt)
       * Credits
     * Important Contributions (My converter would not have been possible without these)
       * Dave Dunfield, LZSS Code and TeleDisk documentation
         * Copyright 2007-2008 Dave Dunfield All rights reserved.
         * [td0_lzss.h](lif/teledisk/td0_lzss.h)
         * [td0_lzss.c](lif/teledisk/td0_lzss.c)
           * LZSS decoder
         * [td0notes.txt](lif/teledisk/td0notes.txt)
           * Teledisk Documentation
       * Jean-Franois DEL NERO, TeleDisk Documentation 
         * Copyright (lif/teledisk/C) 2006-2014 Jean-Franois DEL NERO
           * [wteledsk.htm](lif/teledisk/wteledsk.htm)
             * TeleDisk documentation
           * See his github project
             * https://github.com/jfdelnero/libhxcfe

## Posix wrapper - provides Linux file IO functions for Fatfs
  * [posix](posix)
    * My POSIX wrappers provide many UNIX POSIX compatible functions by translating fatfs functions 
    * [buffer.c](buffer.c)
    * [buffer.h](buffer.h)
      * Currently unused in this project
    * [posix.c](posix/posix.c)
    * [posix.h](posix/posix.h)
      * POSIX wrappers for fatfs - Unix file IO function call wrappers
    * [posix_tests.c](posix/posix_tests.c)
    * [posix_tests.h](posix/posix_tests.h)
      * POSIX user tests

## Printf display functions
  * [printf](printf)
    * My Printf and math IO functions
    * [mathio.c](printf/mathio.c)
      * Number conversions 
    * [mathio.h](printf/mathio.h)
      * Number conversions 
    * [n2a.c](printf/n2a.c)
      * Binary to ASCII converter number size only limited by memory
    * [printf.c](printf/printf.c)
      * My small printf code - with floating point support and user defined low character level IO
    * [sscanf.c](printf/sscanf.c)
      * My small scanf code - work in progress
    * [test_printf.c](printf/test_printf.c)
      * Test my printf against glibs 1,000,000 tests per data type

## SD Card files for project
  * [sdcard](sdcard)
  * My **HP85** **AMIGO** and **SS80** disk images
    * Linux bash script to build ALL the disk images
      * [sdcard/create_images.sh](sdcard/create_images.sh)
        * Files from **ASCII-files**, **LIF-files** are added to all of the created images
    * All Disk definitions, address,**PPR**, **DEBUG** level for **SS80** and **AMIGO** drives
      * [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg)
        * PRINTER address
    * Alternate configuration for using only **AMIGO** drives
      * [amigo.cfg](sdcard/amigo.cfg)
        * Use this if your system does not support **SS80** drives 
           * Copy this file over the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) file after renaming the [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) file
        * PRINTER address

### *AMIGO* disk images
  * [amigo1.lif](sdcard/amigo0.lif)
    * **AMIGO** disk image
    * Has some demo basic programs in it
  * [amigo2.lif](sdcard/amigo1.lif)
    * **AMIGO** disk image
    * Has some demo basic programs in it
  * [amigo3.lif](sdcard/amigo2.lif)
    * **AMIGO** disk image 
    * Has some demo basic programs in it
  * [amigo4.lif](sdcard/amigo3.lif)
    * **AMIGO** disk image 
    * Has some demo basic programs in it

### *SS80* disk images
  * [ss80-1.lif](sdcard/ss80-0.lif)
    * **SS80** disk image 
    * Has some demo basic programs in it
  * [ss80-2.lif](sdcard/ss80-1.lif)
    * **SS80** disk image 
    * Has some demo basic programs in it
  * [ss80-3.lif](sdcard/ss80-2.lif)
    * **SS80** disk image 
    * Has some demo basic programs in it
  * [ss80-4.lif](sdcard/ss80-3.lif)
    * **SS80** disk image 
    * Has some demo basic programs in it

### SD Card emulator configuration file backups
  * [sdcard/configs](sdcard/configs)
    * Backup copies of the hp85disk config files

### Build Drive Configuration
  * [sdcard/mkcfg](sdcard/mkcfg)
  * Build a [sdcard/hpdisk.cfg](sdcard/hpdisk.cfg) disk record 
    * Using [hpdir.ini](sdcard/notes/hpdir.ini) database
       * We can get drive block count *mkcfg -m DRIVE -b*
<pre>
      mkcfg [-list]| [-m model [-b]|[-d]] [-a address]
         -list lists all of the drives in the hpdir.ini file
         -a disk address 0..7
         -m model only, list hpdisk.cfg format disk configuration
         -s short hpdisk.cfg format
         -b only display block count, you can can use this with -m
         -d only display computed directory block count, you can use this with -m
         -f NAME specifies the LIF image name for this drive
</pre>

### BUILD SCRIPTS
  * [sdcard/scripts](sdcard/scripts)
    * Scripts that help creating **LIF** images from multiple files
    * Used by [create_images.sh](sdcard/create_images.sh)
### My *HP85* bus trace files
  * [sdcard/traces](sdcard/traces)
    * [amigo_trace.txt](sdcard/traces/amigo_trace.txt)
    * **AMIGO** trace file when connected to **HP85** showing odd out of order command issue
  * [gpib_reset.txt](sdcard/traces/gpib_reset.txt)
    * **GPIB** reset trace when connected to **HP85**
  * [gpib_trace.txt](sdcard/traces/gpib_trace.txt)
    * **GPIB** transaction trace when connected to **HP85**
### My *HP85* plot capture files
  * [plots](sdcard/plots]
    * [plot1.plt](sdcard/plots/plot1.plt)
    * [plot2.plt](sdcard/plots/plot2.plt)
### ASCII Basic files - in text format for easy editing
  * [ASCII-files](sdcard/ASCII-files)
    * [CIRCLE.TXT](sdcard/ASCII-files/CIRCLE.TXT)
    * [DRIVES.TXT](sdcard/ASCII-files/DRIVES.TXT)
    * [GPIB-TA.txt](sdcard/ASCII-files/GPIB-TA.txt)
    * [HELLO.TXT](sdcard/ASCII-files/HELLO.TXT)
    * [RWTEST.TXT](sdcard/ASCII-files/RWTEST.TXT)
    * [TREK85A.TXT](sdcard/ASCII-files/TREK85A.TXT)
    * [ASCII-files/TREK85](sdcard/ASCII-files/TREK85)
      * TREK85 by Martin Hepperle, December 2015
        * https://groups.io/g/hpseries80/topic/star_trek_game_for_hp_85/4845241
      * [author.txt](sdcard/TREK85/author.txt)  
      * [readme.txt](sdcrad/TREK85/readme.txt)    
      * [Star Trek.pdf](sdcard/TREK85/Start Trek.pdf)
      * [TREK85.BAS](sdcard/TREK85/TREK85.BAS)
      * [trek.lif](sdcard/TREK85/trek.lif)
### *LIF* images with a single program in them
  * [LIF-files](sdcard/LIF-files)
    * Internal names are the same as the **LIF** name without extension
  * [GETSAVE.LIF](sdcard/ASCII-files/GETSAVE.LIF)
    * Adds **GET** and **SAVE** commands to an **HP85**
  * [GPIB-T.lif](sdcard/ASCII-files/GPIB-T.lif)
    * Simple **GPIB** test
  * [RWTESTB.lif](sdcard/ASCII-files/RWTESTB.lif)
    * Reads,Writes and Purge tests
  * [TREK85B.lif](sdcard/ASCII-files/TREK85B.lif)
    * TREK85 by Martin Hepperle, December 2015
      * https://groups.io/g/hpseries80/topic/star_trek_game_for_hp_85/4845241
  * **LIF** images with multiple programs in them
  * [LIF-volumes](sdcard/LIF-volumes)
    * [85-SS80.LIF](sdcard/ASCII-files/85-SS80.LIF)
### *GETSAV* documentation
  * [notes](sdcard/notes)
    * **GETSAVE** can be loaded on an **HP85** to GET and SAVE Basic text files
	  * This can be installed on your **HP85** with this command LOADBIN "GETSAVE"
      * NOTE: my LIF utilities can translate between ASCII files and files in GET/SAVE format 
  * Various notes 



## Unused code
  * [unused](unused)
    * Everything under this directory is currently unused
    * [LCD.c](unused/LCD.c)
    * [LCD.h](unused/LCD.h)
      * SparkFun LCD-14072,LCD-14073,LCD-14074 support code
      * https://github.com/sparkfun/SparkFun_SerLCD_Arduino_Library
        * Modified for this project
    * [lcd_printf.c](unused/lcd_printf.c)
    * [lcd_printf.h](unused/lcd_printf.h)
      * My LCD printf wrapper for SparkFun LCD functions
    * [TWI_AVR8.c](unused/TWI_AVR8.c)
    * [TWI_AVR8.h](unused/TWI_AVR8.h)
      * I2C code LUFA Library Copyright (hardware/C) Dean Camera, 2011.


___
