============================================================
Harvard Extension School CSCI E-92:
Principles of Operating Systems - Spring 2017

Project Documentation
Link to the final term project presentation : https://www.youtube.com/watch?v=2worKeER13A
============================================================

shell.c is the main file for simple shell implementation.After displaying 
the "$ " sign, we scan the string of 256 characters using readInputString 
function and compute the argc value. Then to further parse the string we 
use parseArguments function. which returns array of argv.ignoreInitialWhiteSpaceIfAny
function igonres any initial spaces in the input string and if user have entered 
enter then it displays the prompt.

A declaration of argc and argv follows:

   int argc;
   char *argv[];

Here is an example.  If the input were to contain:

   echo     this  is some     input   string

   then argc would contain 6 and argv would be an array of seven
   pointers; the first six would point to null terminated strings and
   the seventh pointer would be NULL.  The first pointer would point
   to the string "echo", the second pointer would point to the string
   "this", and so forth.  Note that the argv array has been dynamically
   allocated and that each of the strings pointed to by each entry in argv
   needs to be dynamically allocated.  The strings pointed to by argv will 
   never contain any spaces or tabs.  The space allocated for argv will 
   be exactly the required size (i.e., it should occupy (argc+1)*sizeof(char *) 
   bytes).  Similarly, the space allocated for each string will be exactly 
   the required size (i.e., each string should occupy the number of bytes 
   in the string + 1 for a null terminating byte).  Storage should be allocated
   using the malloc system call.

The error codes are defined as following
    SUCCESS = 0,
    INVALID_INPUT = -1,
    FILE_NOT_FOUND = -2,
    MEMORY_ALLOCATION_FAILED = -3,
    FAILED_TO_EXECUTE_GETTIMEOFDAY = -4,
    INVALID_COMMAND = -5,
    MAX_ERRORCODE_INDEX = -6

4)executeCommand is the main routine which will execute the command, which 
is implemented in cmd_*.c files

So far Shell supports following programs, an updated list can be seen in 
commandsDefination.h file.

/* Available Commands */
int cmd_date(int, char **);
int cmd_echo(int, char **);
int cmd_exit(int, char **);
int cmd_help(int, char **);
int cmd_malloc(int, char **);
int cmd_free(int, char **);
int cmd_memorymap(int, char **);

The structure to hold this commands is following, it is designed in such a way that 
in future any new commands can be added easily.

/* Data structure for commands */
struct commandEntry
{
    char *name;
    int (*functionp)(int argc, char *argv[]);
} commands[] = {
    {"date", cmd_date},
    {"echo", cmd_echo},
    {"exit", cmd_exit},
    {"help", cmd_help},
    {"malloc", cmd_malloc},
    {"free", cmd_free},
    {"memorymap", cmd_memorymap}
};

stringMatch Utility is for checking which command was supplied as argv[0]. 
After a command returns to the shell, all storage allocated for argv and 
the strings that argv points to will be freed.  After the storage is freed,
the shell will loop back to prompt the user for another input line using a while loop.

NOTE: 
   Each command checks that the appropriate number of arguments
   are specified on the command line.  If no arguments are appropriate
   for a command, then command guarantees that no arguments have
   been specified. 

============================== 
Memory Management
==============================

initializeMainMemory Function initializes the memory by the required amount and sets 
the free block's PID to 255 as default pattern. The initialize function looks like 

void initialize()
{
    /* Initializing the block of memory */
    freeList->size = MAX_MEMORY_SIZE-sizeof(struct block);
    freeList->free=1;
    freeList->pid=0xFF;
    freeList->next=NULL;
}

Moreover, when the block is freed we reset the PID to 255. 

Externally there are two visible interfaces: "myMalloc" and
"myFree." Which are implemented in memory.c and supporting header file memory.h
The myMalloc function is declared as taking an unsigned int as its only
parameter and returning a pointer to void, as follows:

	void *myMalloc(unsigned int size);

The "size" parameter is the size in bytes of the storage needed by the
caller.  The myMalloc function allocates an appropriately sized
region of memory and it returns a pointer to (i.e., the address
of) the first byte of that region.  If the storage cannot be
successfully allocated for any reason, the function returns a
NULL pointer (i.e., the value 0).  If a request is made to allocate 0
bytes of memory, the function returns a NULL pointer.  The
allocated memory is not initialized.

The pointer returned by myMalloc will always point to a region of
memory that starts on an 8-byte (double-word) boundary. The roundUp function 
does this required rounding. This allows a user to place any data at the 
beginning of a myMalloc region --including data that requires a double-word
boundary.  

The myFree function is declared as taking a pointer to void as its
only parameter and not returning anything, as follows:

	void myFree(void *ptr);

The "ptr" parameter is a pointer to a region of memory previously
allocated by the myMalloc function.  The myFree function deallocates
the entire region of memory pointed to by the parameter.  If the value
of the parameter is NULL, the call has no effect.  If the "ptr"
parameter does not point to a previously allocated region of memory,
the call does not have any effect.  A region of memory allocated by the
myMalloc function is being only deallocated once.  Once a region of
memory has been deallocated by calling myFree, the storage can not
be accessed in any way by the calling program.  The myFree function makes
the freed storage be available for a future call of myMalloc.

memtest.c file contains the test for myMalloc and myFree. Each testpoint prints 
the memory map by calling myMemoryMap funciton. 

shell commands malloc, free and memorymap are implented which internally calls 
myMalloc, myFree and myMemoryMap.

============================
Memory Management Algorithms
============================

I have used the First-fit-algorithm to find a free block to allocate memory. 
Assume that we get a request to allocate a block of memory of size 500 bytes. 
Starting from the first metadata block we can go on searching for the first block
which has enough space to allocate. If we get the free block sizes to be 250,
130 and 750 in order, we will allocate the block of size 750.

If we find a free block which exactly fits the required size, we don't need to do
the splitting. So this split() function is only required if we have what is more than required.
The arguments accepted by this function are --- A pointer to the metadata block which refers to 
the block of memory whose size is more than required.(fitting_slot) and the required size of
the memory chunk to be allocated. Here we create a new metadata block pointer called "new". 
And it should point to the location provided by passing(setting aside) a block of memory which
is equal to the_size_of_the_metadata_block_we_considered + the_size_requested_to_be_allocated

Then this new pointer points to the metadata block referring to the next free chunk. As you can 
see from the code, I have set the attributes of both the new and fitting_slot metadata blocks
accordingly.

First fit || Next fit || Best fit || Worst fit || Quick fit

=================================================================================================
First Fit
In the first fit approach is to allocate the first free partition or hole large
enough which can accommodate the process. It finishes after finding the first suitable
free partition.

Advantage
Fastest algorithm because it searches as little as possible.

Disadvantage
The remaining unused memory areas left after allocation become waste if it is too smaller.
Thus request for larger memory requirement cannot be accomplished.

=================================================================================================
Next fit
Next fit is a modified version of first fit. It begins as first fit to find a free partition. 
When called next time it starts searching from where it left off, not from the beginning.

=================================================================================================
Best Fit
The best fit deals with allocating the smallest free partition which meets the requirement
of the requesting process. This algorithm first searches the entire list of free partitions
and considers the smallest hole that is adequate. It then tries to find a hole which is close
to actual process size needed.

Advantage
Memory utilization is much better than first fit as it searches the smallest free partition first available.

Disadvantage
It is slower and may even tend to fill up memory with tiny useless holes.
=================================================================================================
Worst fit
In worst fit approach is to locate largest available free portion so that the portion left will 
be big enough to be useful. It is the reverse of best fit.

Advantage
Reduces the rate of production of small gaps.

Disadvantage
If a process requiring larger memory arrives at a later stage then it cannot be accommodated 
as the largest hole is already split and occupied.
=================================================================================================
Quick fit
Quick fit has stunning performance. The idea here is that you keep a "cache" of free blocks of storage
rooted in their sizes.  Sizes at this level are always multiples of some basic allocation granularity,
such as DWORD.  Like most algorithms that work well, it is based on patterns of observed behavior.
The L1 and L2 caches rely on what is called "locality of reference" to in effect prefetch and cache 
data that is very likely to be used in the near future.  

Advantage
Fast memory access and allocation 

Disadvantage
Chache management is nessessary
==================================================================================================
Looking at all the performance and ease of implementation, I have implemented First Fit algorithm
which is esay to implement. 

Why the algorithm "First-Fit" is a good compromise between efficient storage utilization and speed?
=> For our K70 project we will be attaching external RAM and hence, we will have an enough memory 
to implement and do computiation. Moreover, first fit algorithm is easy to implement. It may be some
time consuming to travase through the linked list, but since we will run processor at 120MHz. It will 
be just fine to use First-Fit algorithm.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Device-independent system contains: 

Supported Devices : LEDS, PUSHBUTTONS

In Memory File System: 65535 files

typedef enum 
{
	LED = 0,
	PUSHBUTTON = 1,
	FILESYSTEM = 2,
	UART_COMM = 3,
	LCDC = 4,
	ADC = 5,
	TOUCH = 6,
}allMajorDeviceList;

typedef enum 
{
	ORANGE = 0,
	YELLOW = 1,
	BLUE = 2,
	GREEN = 3,
}allLEDs;

typedef enum 
{
	PB1 = 0,
	PB2 = 1,
}allPBs;

typedef enum 
{
	UART_2 = 2,
}allUARTs;

typedef enum
{
	RGB = 0,
}allLCDC;

typedef enum
{
	POTENTIOMETER = 0,
	TEMPRATURE_SENSOR = 1,
}allADC;

typedef enum
{
	TS1 = 0,
	TS2 = 1,
	TS3 = 2,
	TS4 = 3,
}allTouchSensors;

enum file_type
{
	FILE_REGULAR = 0,
	EMPTY = 1,
}allFilesType;

typedef enum
{
	STDIN = 0,
	STDOUT = 1,
	STDERR = 2,
}streamID;


struct deviceEntry
{
	char *name; 
	int majorDeviceID;
	int minorDeviceID;
} allDevices[] = {
		{"/dev/led/orange", 		 LED,		 ORANGE},
		{"/dev/led/yellow", 		 LED, 		 YELLOW},
		{"/dev/led/blue",   		 LED, 		 BLUE},
		{"/dev/led/green",  		 LED, 		 GREEN},
		{"/dev/pushbutton/pb1",  	 PUSHBUTTON, PB1},
		{"/dev/pushbutton/pb2", 	 PUSHBUTTON, PB2},
		{"",                         FILESYSTEM, FILE_REGULAR},
		{"/dev/uart/2", 	         UART_COMM,  UART_2},
		{"/dev/lcdc/rgb",            LCDC,       RGB},
		{"/dev/adc/pot",             ADC,        POTENTIOMETER},
		{"/dev/adc/temp",            ADC,        TEMPRATURE_SENSOR},
		{"/dev/touch/ts1",           TOUCH,      TS1},
		{"/dev/touch/ts2",           TOUCH,      TS2},
		{"/dev/touch/ts3",           TOUCH,      TS3},
		{"/dev/touch/ts4",           TOUCH,      TS4},
};

NOTE: 

All the enums are defined in device.h

All the LEDs have major device ID to be 0
Pushbuttons have major device ID to be 1

If we want to know which LED, we need to refer to the minor device.

=====================================================================
File is implemented as below.

struct file
{
	bool isFree;
	struct file_node *head; /* Head of the file node - some memory location for the file */
	uint32_t noOfFileNodes; /* Number of file nodes */
	char name[256]; /* Name of the file which may be up to 256 characters */
	/* uint8_t permissions; */ /* May be in future to provide the permissions for file access */
	enum file_type type; /* File or Directory */	
	uint32_t file_size; /* Size of the file which is sum of all the file_node sizes */
};

struct file *allFiles;

initalizeDevicesAndMemory function initializes a memory for the file system, it turns on the clock for
the LED and Push Button and starts the default 3 streams named as stdin(read only), stdout(write only), stderr(write only).

	fopen_main("stdin","r");
	fopen_main("stdout","w");
	fopen_main("stderr","w");

Following are some implementation NOTEs:

  Filename length: At least 255 characters in a filename.

  Number of files/directories in a directory: No limit (limited by
    available memory)

  Size of a file: No limit (limited by available memory)

  Number of files in your file system: 64K
	
==================================================================================

(1) Can any device be opened simultaneously
more than once?, 

The PCB stores the numbers of open streams and we allow only the 10 streams to be opened at once. 
We do allow any device to be opened simultaneously. In this case, we show the message corosponding to DEVICE_ALREADY_OPEN
and simply retrun its file identifier.

(2) Can any file be opened simultaneously more than
once?, 

We do allow any files to be opened simultaneously, In this case we show the message corrosponsding to FILE_ALREADY_OPEN, and 
simply return its file identifier.

(3) What happens if a file already contains some bytes and an
fputc is issued to that file?  If in the beginning or middle, is the
file truncated at that point or does the fputc simply overwrite bytes?
If at the end, is the file extended?

Most of the time when file is opened with the mode specified, it provides behavior based on the specified mode.

It is required to provide the following mode to get the desired behavior with fopen.

		r      Open text file for reading.  The stream is positioned at the
              beginning of the file.

       r+     Open for reading and writing.  The stream is positioned at the
              beginning of the file.

       w      Truncate file to zero length or create text file for writing.
              The stream is positioned at the beginning of the file.

       w+     Open for reading and writing.  The file is created if it does
              not exist, otherwise it is truncated.  The stream is
              positioned at the beginning of the file.

       a      Open for appending (writing at end of file).  The file is
              created if it does not exist.  The stream is positioned at the
              end of the file.
 
==================================================================================

The function signatures are as below:

int fgetc_main(int );
int fputc_main(int,char);
int fopen_main(char *);
int fclose_main(int);

int led_fgetc(void *);
int led_fputc(char,void *);
int led_fopen(void *);
int led_fclose(void *);

int pushbutton_fgetc(void *);
int pushbutton_fputc(char,void *);
int pushbutton_fopen(void *);
int pushbutton_fclose(void *);

int file_fgetc(void *);
int file_fputc(char,void *);
int file_fopen(void *);
int file_fclose(void *);

int uart_fgetc(void *);
int uart_fputc(char,void *);
int uart_fopen(void *);
int uart_fclose(void *);

NOTE that for LEDs and PushButtons if the logical value is 1 or 0, we return 49 or 48 respectively, 
so that when we display the return character it shows either 1 or 0. 
(This behavior is only for better user experiance)
======================================================================
If fopen is issued at shell level, then cmd_fopen gets issued at OS level.
which calls fopen_main(). 

fopen_main looks following things:
1) is this named device/file already opened ? if yes then return opened file identifier
2) if not already opened 
	- and a device name then open that device. 
	- and a file name then open that file.
	- and nothing matched then create a file 
3) We register the device in the openStreams of PCB, Based on the minor device we find the minor device 
struct using the getMinorDeviceStruct function.
4) Based on the major device we call myfopen of that device and pass the minordevice struct.

fclose simply makes the openStream to be free pcb->openStreams[fd]->isFree = true;
so it can be used by some other device or file.
========================================================================

Supervisor call architecture for all of the hardware interactive system calls
is implemented in svc.c.   

Note that all the user facing systems calls are now modified with their version of 
svc_*. folloing are some of the examples. 

svc_myMalloc
svc_myFree
svc_myFreeErrorCode
svc_myMemoryMap
svc_fgetc
svc_fputc
svc_create
svc_delete
svc_fputs
svc_fgets

Notice that when user calls fgetc in the shell, eventually cmd_fgetc will get called. cmd_fgetc will call 
svc_fgetc, Which will be a Supervisor call. svcHandler Supervisor call (SVC) routine is implemented in C with
function name svcHandlerinC.

svcHandlerinC uses bunch of switch case statement which then calls Implementation of the function. i.e. 
SVCfgetc_mainImpl(); (Please refer to code for more detail)
============================================================================
To allow login capabilites for any user, the username and password pairs are implemented in this OS.
For Grading purposes: use any username password pair. (guest would be ideal)

following commands are implemented for user interaction.

	 1. ser2lcd: Continuously copy characters from serial input to
        LCD.  End on a ^D (control-D) input character.  The behavior
        of the LCD device should mirror the behavior of our terminal
        emulator.  That is, it should: (1) return to the beginning of
        the current line when it is sent a carriage-return (\r, ^M, or
        control-M) and (2) go to the next line in the same column when
        it is sent a line-feed (\n, ^J, or control-J).
     2. touch2led: Continuously copy from each touch sensor to the
        corresponding LED.  End when all four touch sensors are
        "depressed."
     3. pot2ser: Continuously output the value of the analog
        potentiomemter to the serial device as a decimal or
        hexadecimal number followed by a newline.  End when SW1 is
        depressed.
     4. therm2ser: Continuously output the value of the thermistor to
        the serial device as a decimal or hexadecimal number followed
        by a newline.  End when SW1 is depressed.
     5. pb2led: Continuously copy from SW1 to orange LED and SW2 to
        yellow LED.  End when both SW1 and SW2 are depressed.

=====================================================================================

1) Flex Timer to maintain the clock :

Timer routines that uses FlexTimer interrupts (from FTM0) to maintain the current time and date.  
A supervisor call svc_settimeofday can be called to allow the time and date to be set and a second
supervisor call svc_gettimeofday return the current time and date.  

The time is maintained as the number of milliseconds since an epoch of midnight (zero hours) on January
1, 1970. The routines maintain this count of milliseconds in a double word (64 bit) value. The initial time
and date will not be set automatically; a user must issue the supervisor call to set the time and date.

Notice that framepointer union is now changed to accomodate unsigned long long int values that is composed of 
r0 and r1 both i.e. 64 bit value.

2) UART Communication :

All the input and ouput to UART is now interrupt driven instead of polling driven. 

Interrupt-driven I/O from/to the user RS232 UART (UART2):
In order to implement this, two queues are maintained: one queue for input
characters (from the port to the K70) and one queue for output characters 
(from the K70 to the port). When a UART receiver interrupt ("receiver full interrupt")
occurs, read the character from the UART data register and enqueue the character to
the input queue. 
 
When the character input SVC assigned to this serial I/O device is called, we dequeue a 
character from the input queue and return it to the user.  When the character output SVC
assigned to this serial I/O device is called, enqueue the character to the output queue 
and make sure that the UART transmitter interrupt is enabled.  When the UART transmitter
interrupt occurs, dequeue a character from the output queue and send it to the UART data
register.  If there are no more characters in the output queue, we disable transmitter interrupts.

In cases where the SVC action cannot be accomplished (for example, an input SVC with no characters
in the buffer or an output SVC with the buffer full), busy wait for the condition to clear.  
If an interrupt routine cannot complete its action (for example, if a UART receiver interrupt
occurs and the input queue is full or if a UART transmitter interrupt occurs and the output queue is empty),
either discard the character that caused the interrupt and possibly echo a bell character (control-G) 
back to the serial port or don't output a character, respectively.
 
3)PDB0 Timer: 

	/* 
	 * fpheripheral = 60 MHz
	 * 
	 * fprescalar = fpheripheral / Prescalor = 60 MHz / 128 = 468750 Hz
	 * 
	 * fpdb = fprescalar / Mult = 468750 / 40 = 11718.75 Hz
	 * 
	 * So PDB0 interrupt occurs at every 1/11718.75 seconds
	 * 
	 * So providing count of 11718 should give interrupt at every one second.
	 */
	 
Reference :page 1181 (PDF page 1188) of the K70 Sub-Family Reference Manual, Rev. 4, Oct-2015

4) Date command: 

Please look at the help for the date command.

=============================================================================================================
Process Control and Multi Programming
*Currently Not working as expected. 
*For the API design please look at the in code comments
*Currently for createFirstProcessFunction I have defined #defines for modes, such as SINGLE_PROCESS_MODE 
and MULTI_TASKING_MODE
*Current testing and demo SINGLE_PROCESS_MODE is recommanded.
=============================================================================================================
MORSE code devices: 

morse.h defines the morse code table. PDB0 timer is used to intearct with the devices

For example: 
$ fopen /dev/morse/led/orange
Returned File Descriptor is 3
$ fputc 3 a
$ fputs 3 Hello World

Please look at the term project presentation for more details on working of morse code translation.

==============================================================================================================
For mutual execlusion in the malloc and free infrastructure I have used di() and ei()
