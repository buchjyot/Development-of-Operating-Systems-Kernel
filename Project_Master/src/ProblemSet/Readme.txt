============================================================
Harvard Extension School CSCI E-92:
Principles of Operating Systems - Spring 2017

Project Documentation
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
