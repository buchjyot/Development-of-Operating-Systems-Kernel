/*
 * device.c
 *
 *  Created on: Mar 18, 2017
 *      Author: Jyot Buch
 */

#include "device.h"
#include "../mem/memory.h"
#include "../util/reportError.h"
#include "../process/processControlUtils.h"
#include "../util/util.h"
#include "led.h"

/* For Morse Code drivers */
struct LED *GlobalMorseLed;
int ledState = OFF;
int doNothingState = OFF;

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

/* Global Function that user/process will call */

int SVCfgetc_mainImpl(int fd) /* fd is a file descriptor which user will pass*/
{
	/* Get the current PCB*/
	struct PCB *pcb = (struct PCB *)SVCgetCurrentPCBImpl();

	/* If the fd is not something which has a device/file mapped to it, throw error INVALID_FILE_DISCRIPTOR */
	if(pcb->openStreams[fd].isFree)
		return(INVALID_FILE_DISCRIPTOR);

	/* If user try to get char from stdout\stderr throw an error message */
	if(fd==STDOUT || fd == STDERR)
		return(OUTPUT_ONLY_STREAMS);

	/* Based on the file descriptor */
	return (majorDevice[pcb->openStreams[fd].majorDeviceID].myfgetc(pcb->openStreams[fd].minorStruct));
}

int SVCfputc_mainImpl(int fd,char c)
{
	errorCode status;

	/* Get the current PCB*/
	struct PCB *pcb = (struct PCB *) SVCgetCurrentPCBImpl();

	/* If the fd is not something which has a device/file mapped to it, throw error INVALID_FILE_DISCRIPTOR */
	if(pcb->openStreams[fd].isFree)
		return(INVALID_FILE_DISCRIPTOR);

	/* If user try to put char in stdin throw an error message */
	if(fd==STDIN)
		return(INPUT_ONLY_STREAMS);

	/* Call myfputc based on the file descriptor */
	status = majorDevice[pcb->openStreams[fd].majorDeviceID].myfputc(c,pcb->openStreams[fd].minorStruct);

	return(status);
}


/* Global Function that user/process will call */
int SVCfopen_mainImpl(char *name,char *mode)
{
	/* Find the correct device that user is trying to access */
	int id,fd;
	struct PCB *pcb = (struct PCB *)SVCgetCurrentPCBImpl();

	/* Make sure that mode is one of the value supported by fopen */
	if(!(strcmp_fcn(mode,"r")||strcmp_fcn(mode,"r+")||strcmp_fcn(mode,"w")||strcmp_fcn(mode,"w+")||strcmp_fcn(mode,"a")))
		return(INVALID_MODE);

	/* look in to the PCB if there exists same named file or device which is currently open*/
	fd = getOpenStreamIDifAlreadyOpen(pcb,name);
	if(fd!=NOTHING_MATCHED) 
	{
		displayAlreadyOpenMessage(name);	
		return(fd);
	}

	/* get stream Id to get allDevices ID */
	id = getStreamID(name,mode);
	if(id<0)return(id);

	/* Find the free FILE descriptor */
	fd = SVCgetFreeFileDiscriptorImpl();
	if(fd==MAX_OPEN_STREMS_LIMIT_REACHED)return(MAX_OPEN_STREMS_LIMIT_REACHED);

	/* Create a dev struct */
	struct deviceEntry dev = allDevices[id];

	/* Make a Record in the PCB that now this device is being used */
	pcb->openStreams[fd].isFree = false;
	pcb->openStreams[fd].majorDeviceID = dev.majorDeviceID;
	pcb->openStreams[fd].minorDeviceID = dev.minorDeviceID;
	pcb->openStreams[fd].minorStruct = getMinorDeviceStruct(name,dev.majorDeviceID,dev.minorDeviceID);

	if(strcpy_fcn(pcb->openStreams[fd].name,name)!=true)
		SVCreportErrorImpl(COPY_FAILURE);

	if(id==FILE_ID || isaDefaultStream(name))/* That means its a file or default stream*/
	{
		if(strcpy_fcn(pcb->openStreams[fd].mode,mode)!=true)
			SVCreportErrorImpl(COPY_FAILURE);
	}

	/* Call the myfopen function */
	majorDevice[dev.majorDeviceID].myfopen(pcb->openStreams[fd].minorStruct);

	/* Return the File Descriptor so that user can interact with the device */
	return fd;
}

int SVCfclose_mainImpl(int fd)
{
	struct PCB *pcb = (struct PCB *)SVCgetCurrentPCBImpl();

	/* If the fd is not something which has a device/file mapped to it, throw error INVALID_FILE_DISCRIPTOR */
	/* Moreover, we do not want user to close the default streams that OS opened while booting */
	if(pcb->openStreams[fd].isFree)
		return(INVALID_FILE_DISCRIPTOR);
	else if(isaDefaultStream(pcb->openStreams[fd].name))
		return(DO_NOT_CLOSE_DEFAULT_STREAMS);

	/* Call the myclose function */
	majorDevice[pcb->openStreams[fd].majorDeviceID].myfclose(pcb->openStreams[fd].minorStruct);

	/* Free the stream by setting isFree to true so that this file descriptor can be used elsewhere. */
	pcb->openStreams[fd].isFree = true;

	return(SUCCESS);
}

/* This function puts the string to UART or file */
int SVCfputs_mainImpl(int fd,char* string)
{
	struct PCB *pcb = SVCgetCurrentPCBImpl();

	int retCode = SUCCESS;

	if(pcb->openStreams[fd].majorDeviceID == MORSE_LED && (fd!=STDOUT) && (fd!=STDERR))
	{
		while(*string)
		{
			/* Deliver one by one char to fputc_main */
			retCode = SVCfputc_mainImpl(fd,*string++);
			if(retCode!= SUCCESS)break;

			/* Wait for the time between 2 dots or 2 dashes or both, if next is not a space, if space anyway when 
			 * loop will reach above sentence then it will wait for time duration of 7 dots.*/
			if(*string && *string !=' ')
			{
				doNothingState = ON;
				SVCsetPDBTimerImpl(TIME_BETWEEN_LETTERS_OF_A_WORD, &doNothing, PDBTimerOneShot);			
				while(doNothingState);				
			}

		}
	}
	else
	{
		while(*string)
		{
			/* Deliver one by one char to fputc_main */
			retCode = SVCfputc_mainImpl(fd,*string++);
			if(retCode!= SUCCESS)break;
		}
	}

	return(retCode);
}

/* This function reads string from files and displays them to the UART*/
int SVCfgets_mainImpl(int fd)
{
	int returnArg = SUCCESS;

	while(true)
	{
		returnArg = SVCfgetc_mainImpl(fd);
		if(returnArg == OUTPUT_ONLY_STREAMS || returnArg == INVALID_FILE_DISCRIPTOR) break;

		if(returnArg == EOF_REACHED)
			return(SUCCESS);

		/* Display the file content using stdout */
		SVCfputc_mainImpl(STDOUT,(char)returnArg);
	}

	return(returnArg);
}

int SVCcreateImpl(char* name)
{
	/* See if that named file/device/stream already exists. */
	if(isaFile(name)||isaDevice(name)||isaDefaultStream(name))
	{
		/* Report the message that file exists. */
		return(FILE_ALREADY_EXISTS_WITH_SAME_NAME);
	}

	/* create a file with a given name if mode is specified as w+ or a or a+ */
	create_file(name);
	return(SUCCESS);
}

int SVCdeleteImpl(char* name)
{
	int status = delete_file(name);
	return(status);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

void displayAlreadyOpenMessage(char* name)
{
	if(isaDevice(name))
	{
		/* Report the message that Device is already open */
		SVCreportErrorImpl(DEVICE_ALREADY_OPEN);
	}
	else if(isaFile(name))
	{
		/* Report the message that file is already open */
		SVCreportErrorImpl(FILE_ALREADY_OPEN);
	}
	else if(isaDefaultStream(name))
	{
		SVCreportErrorImpl(DEFAULT_STREAMS_ALREADY_OPEN);
	}
}


int getStreamID(char *name,char* mode)
{
	int id;

	/* Identify whether name is a device or a file or default stream */
	if(isaDefaultStream(name))
	{
		return(UART2_ID); /* Indexes in the device/file list entry */
	}
	else if(isaDevice(name)) /* If the name starts from /dev then it is a peripheral device */
	{
		id = getDeviceListID(name);
		if(id == DEVICE_NAME_NOT_FOUND)
			return(DEVICE_NAME_NOT_FOUND);
		return(id);
	}
	else if(isaFile(name))  /* If the name do not match in the file list, then you need to create that file before you can open */
	{
		return(FILE_ID); /* Indexes in the device/file list entry */
	}
	else if(strcmp_fcn(mode,"w+") || strcmp_fcn(mode,"a"))
	{
		/* create a file with a given name if mode is specified as w+ or a or a+ */
		create_file(name);
		return(FILE_ID); 
	}
	else
	{
		/* Return NOTHING_MATCHED because the name do not match with either device, file or stream */
		return(NOTHING_MATCHED);
	}

}

bool isaDefaultStream(char* name)
{
	return(strcmp_fcn(name,"stdin") || strcmp_fcn(name,"stdout") || strcmp_fcn(name,"stderr"));
}

int getDeviceListID(char *dName)
{
	int i = 0;
	for(i = 0; i < TOTAL_NUMBER_OF_DEVICES; i++)
	{
		if((strcmp_fcn(allDevices[i].name,dName)))
			break;
	}

	if(i<TOTAL_NUMBER_OF_DEVICES)
		return(i);
	else
		return(DEVICE_NAME_NOT_FOUND);
}

int getFileListID(char *fName)
{
	/* Traverse through the file names */
	int i = 0;
	for(i = 0; i < numOfFiles; i++)
	{
		if((strcmp_fcn(allFiles[i].name,fName)))
			break;
	}

	if(i<numOfFiles)
		return(i);
	else
		return(FILE_NAME_NOT_FOUND);
}

bool isaDevice(char *string)
{
	char *pattern = "/dev";
	int i = 0;
	while(*pattern && *string)
	{
		if(pattern[i] != string[i])
			break;
		i+=1;
	}
	if(i==mystrlen(pattern))
		return true;
	else
		return false;
}

int isaFile(char *string)
{
	/* Look for file in a list and if the file exists then return 1, otherwise 0 */
	int i;

	if(numOfFiles !=0)
	{
		for(i=0;i<numOfFiles;i++)
		{
			if(!allFiles[i].isFree)
			{
				if(strcmp_fcn(allFiles[i].name,string))
					return(true);
			}
		}

		if(i==numOfFiles)
			return(false);
	}
	return(false);
}

int getOpenStreamIDifAlreadyOpen(void* input,char *name)
{
	int i;
	struct PCB* myPCB = (struct PCB*)input;

	for(i = 0; i < TOTAL_NUMBER_OF_OPEN_STREAMS; i++)
	{
		if(!(myPCB->openStreams[i].isFree))
		{
			if(strcmp_fcn(myPCB->openStreams[i].name,name))
				return(i);
		}
	}
	return(NOTHING_MATCHED);
}

void* getMinorDeviceStruct(char* myName,int majorDeviceID,int minorDeviceID)
{
	int id,returnCode;

	switch(majorDeviceID)
	{
	case LED: /* LED Struct */
		return(&led_color[minorDeviceID]);

	case PUSHBUTTON: /* PUSHBUTTON Struct */
		return(&pb_switch[minorDeviceID]);

	case FILESYSTEM: /* File Struct */
		returnCode = getFileListID(myName);
		if(returnCode==FILE_NAME_NOT_FOUND || returnCode==NOTHING_MATCHED)
			SVCreportErrorImpl(returnCode);
		else
		{
			id = returnCode;
			return(&allFiles[id]);
		}

	case UART_COMM: /* UART do not use any minor device struct for now it is assumed to be uart2*/
		return(NULL);

	case LCDC_DEV: /* LCDC do not use any minor device struct for now*/
		return(NULL);

	case ADC: /* ADC will return the channel based on the minor device ID.*/
		return(&channel[minorDeviceID]);

	case TOUCH: /* For touch sensors use electrodeHW and minordeviceID for indexing*/
		return(&electrodeHW[minorDeviceID]);

	case MORSE_LED:
		return(&led_color[minorDeviceID]);

	default:
		return(NULL);
	}

	return(NULL);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

/* Note that MASK changes for all the LEDs but the port is same as PORT-A. */

/* When User calls led_fputc for doing byte operation on any led
 * that means he wants to either make a led on or off.
 */

int led_fputc(char c, void* myLED)
{
	struct LED *led = (struct LED *)myLED;

	if(c=='0' || c==0)
	{
		/* Turn the LED off */
		PTA_BASE_PTR->PSOR = led->MASK;
	}
	else
	{
		/* Turn the LED on */
		PTA_BASE_PTR->PCOR = led->MASK;
	}

	return(SUCCESS);
}

/* When User calls led_fgetc for doing byte operation on any led
 * that means he wants to get the status of the led whether it is
 * on or off
 */
int led_fgetc(void* myLED)
{
	struct LED *led = (struct LED *)myLED;
	int ledState = PTA_BASE_PTR->PDOR & led->MASK;
	return(!ledState);
}

int led_fopen(void* myLED)
{
	/* This function does nothing because clock for LEDs are already ON when the user
	 * Boots the OS */
	return(SUCCESS);
}

int led_fclose(void* myLED)
{
	/* This function does nothing because clock for LEDs are already ON when the user
	 * Boots the OS and we do not want them to be off */
	return(SUCCESS);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

int pushbutton_fputc(char c,void* myPB)
{
	/* We can not push the button from the processor so this function does nothing.*/
	return(INPUT_ONLY_DEVICE);
}

int pushbutton_fgetc(void* myPB)
{
	int pushbuttonState;
	struct PUSHBUTTON *pb = (struct PUSHBUTTON *)myPB;

	if(strcmp_fcn(pb->port,"PTD"))
	{
		pushbuttonState = PTD_BASE_PTR->PDIR & (1 << PUSHBUTTON_SW1_PORTD_BIT);
	}
	else if(strcmp_fcn(pb->port,"PTE"))
		pushbuttonState = PTE_BASE_PTR->PDIR & (1 << PUSHBUTTON_SW2_PORTE_BIT);

	/* If push button is up return 0; if push button is down return 1 */
	return(!pushbuttonState);
}

int pushbutton_fopen(void* myPB)
{
	/* This function does nothing because clock for PBs are already ON. */
	return(SUCCESS);
}

int pushbutton_fclose(void* myPB)
{
	/* This function does nothing because clock for PBs are already ON when the user
	 * Boots the OS and we do not want them to be off. */
	return(SUCCESS);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

int fileSystemInit()
{
	int i;
	allFiles = (struct file*)svc_myMalloc(TOTAL_NUMBER_OF_FILES*sizeof(struct file));
	if(!allFiles)return(MEMORY_ALLOCATION_FAILED);

	for(i=0; i<TOTAL_NUMBER_OF_FILES ;i++)
	{
		/* Let all files be free in the begining */
		allFiles[i].isFree = true;
	}

	return(SUCCESS);
}

void create_file(char *name)
{
	int i;
	struct file_node *currentNode;

	/* Traverse through file list until you find the free file block */
	for(i=0; i<TOTAL_NUMBER_OF_FILES ;i++)
	{
		if(allFiles[i].isFree == true)
			break;
		/* i++ : look at the next file block */
	}

	/* files are already owned by the OS by init code, set MALLOC_FOR_OS_ENV to true so that all file nodes are also owned by the OS*/
	setMallocFileSystemEnv();
	currentNode = SVCmyMallocImpl(sizeof(struct file_node));
	unsetMallocFileSystemEnv();

	allFiles[i].isFree = false;

	if(strcpy_fcn(allFiles[i].name,name)!=true)
		SVCreportErrorImpl(COPY_FAILURE);

	allFiles[i].type = FILE_REGULAR;
	allFiles[i].head = currentNode;
	allFiles[i].head->next = NULL;
	allFiles[i].head->filled_size = 0;

	allFiles[i].noOfFileNodes = 1;
	allFiles[i].file_size = 0;
	numOfFiles++; /* Global variable which says how many total files are present in the file system */
}

int delete_file(char *name)
{
	int i,k,fd;
	struct file_node *currentNode;
    errorCode status;

	/* Traverse through file list until you find the file block which matches the name */
	for(i=0; i < TOTAL_NUMBER_OF_FILES ;i++)
	{
		if(strcmp_fcn(allFiles[i].name,name))
			break;
		/* i++ : look at the next file block */
	}

	allFiles[i].type = EMPTY;
	currentNode = allFiles[i].head;

	/* Traverse through all the file nodes and free them */
	for(k=0;k < allFiles[i].noOfFileNodes ;k++)
	{
		status = SVCmyFreeErrorCodeImpl(currentNode);
        if(status!=MEMORY_FREE_SUCCESS)return(status);
		currentNode = currentNode->next;
	}
	allFiles[i].file_size = 0;
	allFiles[i].isFree = true;
	allFiles[i].head = NULL;
	allFiles[i].noOfFileNodes = 0;
	numOfFiles--; /* Since you deleted one file, decrement the counter of number of files */

	/* Make sure there is no reference to that file in the PCB */
	struct PCB *pcb = (struct PCB *)SVCgetCurrentPCBImpl();

	/* look in to the PCB if there exists same named file which is currently open*/
	fd = getOpenStreamIDifAlreadyOpen(pcb,name);
	if(fd!=NOTHING_MATCHED)
	{
		pcb->openStreams[fd].isFree = true; /* Free the stream since the file no longer exists*/
		pcb->openStreams[fd].cursor = NULL;
		pcb->openStreams[fd].currentFileNode = NULL;
	}

	return(SUCCESS);
}

int file_fputc(char c, void* ptr)
{
	int fd;
	struct PCB *pcb = (struct PCB *)SVCgetCurrentPCBImpl();
	struct file *currfile = (struct file*) ptr;	 /* Interpret incoming structure as file Struct */
	struct file_node *currfileNode;

	/* Put the char */
	fd = getOpenStreamIDifAlreadyOpen(pcb,currfile->name);
	if(fd!=NOTHING_MATCHED)
	{
		/* check if the file is in write mode */
		if( strcmp_fcn(pcb->openStreams[fd].mode,"r+") || strcmp_fcn(pcb->openStreams[fd].mode,"w+") || strcmp_fcn(pcb->openStreams[fd].mode,"a"))
		{
			*(pcb->openStreams[fd].cursor) = c;

			currfileNode = pcb->openStreams[fd].currentFileNode;

			/* The moment we put one char, Increment the filled_size by one byte*/
			currfileNode->filled_size = currfileNode->filled_size + 1;

			/* Create a new file node if we have approached the limit for the file node data*/
			if(currfileNode->filled_size == SINGLE_FILE_NODE_DATA_SIZE)
			{
				setMallocFileSystemEnv();
				struct file_node *newFileNode = SVCmyMallocImpl(sizeof(struct file_node));
				unsetMallocFileSystemEnv();

				currfileNode->next = newFileNode;

				/* Initialize the new block */
				currfileNode = currfileNode->next;
				currfileNode->filled_size = 0;

				/* update file level parameters */
				currfile->noOfFileNodes = currfile->noOfFileNodes + 1 ;
				pcb->openStreams[fd].cursor = (char*) (&currfileNode->data[0]); /* Let the cursor now point to the next file node's location */
			}
			else
			{
				/* Increment the cursor within the same file_node*/
				(pcb->openStreams[fd].cursor) = (pcb->openStreams[fd].cursor) + 1;
			}

			/* add it to openStreams structure to keep track of currfileNode */
			pcb->openStreams[fd].currentFileNode = currfileNode;

			/* Compute the total size of the file */
			currfile->file_size = getAllfileNodesDataSize(currfile);
			return(SUCCESS);
		}
		else
			return(FILE_NOT_OPEN_FOR_WRITE);
	}
	return(NOTHING_MATCHED);
}

int file_fgetc(void* ptr)
{
	int c,i,fd;
	bool flag = false;
	struct file_node *currfileNode;
	struct PCB *pcb = (struct PCB *)SVCgetCurrentPCBImpl();

	/* Interpret incoming structure as file Struct */
	struct file *currfile = (struct file*) ptr;

	/* Put the char */
	fd = getOpenStreamIDifAlreadyOpen(pcb,currfile->name);

	if(eofReached(fd)) /* Logic based on size of the file or node */
		return(EOF_REACHED);

	if(fd!=NOTHING_MATCHED)
	{
		/* Start from head being your currentFileNode, if you have multiple file_nodes then traverse through them*/
		currfileNode = pcb->openStreams[fd].currentFileNode;

		/* check if the file is in the read mode */
		if( strcmp_fcn(pcb->openStreams[fd].mode,"r") || strcmp_fcn(pcb->openStreams[fd].mode,"r+") || strcmp_fcn(pcb->openStreams[fd].mode,"w+"))
		{
			c = *(pcb->openStreams[fd].cursor);
			/* Get next cursor position */
			if((pcb->openStreams[fd].cursor) == (char*) (&currfileNode->data[SINGLE_FILE_NODE_DATA_SIZE-1]))
			{
				(pcb->openStreams[fd].cursor) = (char*) (&currfileNode->next->data[0]);
				/* add it to openStreams structure to keep track of currfileNode */
				pcb->openStreams[fd].currentFileNode = currfileNode->next;
				flag = true;
			}
			else
			{
				for(i=0; i< (currfile->noOfFileNodes)-1 ; i++)
				{
					currfileNode = currfileNode->next;
					if((pcb->openStreams[fd].cursor)  == (char*)(&currfileNode->data[SINGLE_FILE_NODE_DATA_SIZE-1]))
					{
						(pcb->openStreams[fd].cursor) = (char*)(&currfileNode->next->data[0]);
						/* add it to openStreams structure to keep track of currfileNode */
						pcb->openStreams[fd].currentFileNode = currfileNode->next;
						flag = true;
						break;
					}
				}
			}

			if(!flag)
				(pcb->openStreams[fd].cursor)++; /* Increment the cursor within the same file_node*/

			return c;
		}
		else
			return(FILE_NOT_OPEN_FOR_READ);
	}
	else
		return(NOTHING_MATCHED);
}

int file_fopen(void* ptr)
{
	int fd;
	struct PCB *pcb = (struct PCB *)SVCgetCurrentPCBImpl();
	struct file *currFile = (struct file*) ptr;
	struct file_node *currFileNode;
	errorCode status;

	/* In which mode user is trying to open a file */
	fd = getOpenStreamIDifAlreadyOpen(pcb,currFile->name);

	if(fd!=NOTHING_MATCHED)
	{
		if(strcmp_fcn(pcb->openStreams[fd].mode,"r") || strcmp_fcn(pcb->openStreams[fd].mode,"r+"))
		{
			/* r Open text file for reading.  The stream is positioned at the
			 * beginning of the file. */

			/* r+ Open for reading and writing.  The stream is positioned at
			 * the beginning of the file.. */

			pcb->openStreams[fd].currentFileNode = currFile->head;
			pcb->openStreams[fd].cursor =(char*) (&currFile->head->data[0]);
		}

		else if(strcmp_fcn(pcb->openStreams[fd].mode,"w") || strcmp_fcn(pcb->openStreams[fd].mode,"w+"))
		{

			/* w Truncate file to zero length or create text file for writing.
			 *  The stream is positioned at the beginning of the file.. */

			/* w+ Open for reading and writing. The file is created if it does
			 *  not exist, otherwise it is truncated. The stream is positioned
			 *   at the beginning of the file.*/
			pcb->openStreams[fd].currentFileNode = currFile->head;
			pcb->openStreams[fd].cursor =(char*) (&currFile->head->data[0]);
			currFileNode = currFile->head;

			/* If more than 1 file nodes, then move to the next file node, hold it's next file node and free memory recursively*/
			if(currFile->noOfFileNodes > 1 && currFileNode->next->next != NULL)
			{
				int k;
				struct file_node *holdNode;
				currFileNode = currFileNode->next;

				/* Traverse through all the rest of the file nodes (i.e. 2nd, 3rd...) and free them */
				for(k=1;currFileNode->next != NULL;k++)
				{
					holdNode = currFileNode->next; /* hold the pointer for the next file_node in holdNode */

					setMallocFileSystemEnv();				
					status = SVCmyFreeErrorCodeImpl(currFileNode); /* free current node */
                    if(status!=MEMORY_FREE_SUCCESS)return(status);
					unsetMallocFileSystemEnv();

					currFileNode = holdNode; /* reload current node to whatever you hold for */
					currFile->noOfFileNodes--;
				}

				/* Free the last node out side of the loop */
				setMallocFileSystemEnv();
				status = SVCmyFreeErrorCodeImpl(currFileNode);
                if(status!=MEMORY_FREE_SUCCESS)return(status);
				unsetMallocFileSystemEnv();

				currFile->noOfFileNodes--;
			}

			else if (currFile->noOfFileNodes > 1 && currFileNode->next->next == NULL)
			{
				setMallocFileSystemEnv();
				status = SVCmyFreeErrorCodeImpl(currFileNode->next);
                if(status!=MEMORY_FREE_SUCCESS)return(status);
				unsetMallocFileSystemEnv();
			}

			currFileNode->filled_size = 0;
			/* Compute the total size of the file which should return 0 */
			currFile->file_size = getAllfileNodesDataSize(currFile);
		}

		else if(strcmp_fcn(pcb->openStreams[fd].mode,"a"))
		{
			/*  a Open for appending (writing at end of file).  The file is
			 *  created if it does not exist.  The stream is positioned at the
			 *  end of the file. */

			currFileNode = currFile->head;
			int i;

			for(i=1;i<currFile->noOfFileNodes;i++)
			{
				currFileNode = currFileNode->next;
			}

			for(i=0;i<currFileNode->filled_size;i++)
			{}

			pcb->openStreams[fd].currentFileNode = currFileNode;
			pcb->openStreams[fd].cursor =(char*) (&currFileNode->data[0+i]);
		}

		else
			return(INVALID_FOPEN_MODE);
	}
	else
	{
		return(NOTHING_MATCHED);
	}

	return(SUCCESS);
}

int file_fclose(void* ptr)
{
	return(SUCCESS);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

bool eofReached(int fd)
{
	struct PCB *pcb = (struct PCB *)SVCgetCurrentPCBImpl();

	if(pcb->openStreams[fd].currentFileNode->next==NULL)
	{
		if(pcb->openStreams[fd].cursor - (char*)&pcb->openStreams[fd].currentFileNode->data[0] == pcb->openStreams[fd].currentFileNode->filled_size)
			return(true);
		else if(pcb->openStreams[fd].cursor - (char*)&pcb->openStreams[fd].currentFileNode->data[0] < pcb->openStreams[fd].currentFileNode->filled_size)
			return(false);
	}
	else
		return(false);

	return(NO_ASSIGNMENT);
}

uint32_t getAllfileNodesDataSize(void* ptr)
{
	int i;
	uint32_t sum = 0;
	struct file *myFile = (struct file*) ptr;
	struct file_node *currFileNode = myFile->head;

	for(i=0; i<=(myFile->noOfFileNodes-1); i++)
	{
		sum = sum + currFileNode->filled_size;
		currFileNode = currFileNode->next;
	}
	return(sum);
}

int listFiles(void)
{
	int i,len;
	char buffer[256];

	for(i=0;i<TOTAL_NUMBER_OF_FILES;i++)
	{
		if(!allFiles[i].isFree)
		{
			len = snprintf(buffer,256,"%s \r\n",allFiles[i].name);
			if(len>=0) svc_fputs_main(STDOUT,buffer);
		}
	}

	return(SUCCESS);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

int adc_fgetc(void *ptr)
{
	struct ADCChannel *myChannel = (struct ADCChannel*) ptr;

	ADC1_SC1A = myChannel->CHANNEL;
	while(!(ADC1_SC1A & ADC_SC1_COCO_MASK)) {
	}

	return ADC1_RA;
}

int adc_fputc(char c, void *ptr)
{
	return(OUPUT_ONLY_DEVICE);
}

int adc_fopen(void *ptr)
{
	return(SUCCESS);
}

int adc_fclose(void *ptr)
{
	return(SUCCESS);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

/* Calibrate the capacitive touch sensors */
void TSI_Calibrate(void) {
	int i;
	uint16_t baseline;

	TSI0_GENCS |= TSI_GENCS_SWTS_MASK;	/* Software Trigger Start */
	while(!(TSI0_GENCS & TSI_GENCS_EOSF_MASK)) {
	}
	TSI0_GENCS |= TSI_GENCS_EOSF_MASK;	// Clear the EOSF (End of Scan) flag

	for(i = 0; i < ELECTRODE_COUNT; i++) {
		baseline = *(electrodeHW[i].counterRegister);
		electrodeHW[i].threshold = baseline + THRESHOLD_OFFSET;
	}
}

int ts_fgetc(void *ptr)
{
	struct electrodeHW *electrode = (struct electrodeHW*) ptr;
	uint16_t oscCount;

	TSI0_GENCS |= TSI_GENCS_SWTS_MASK;	/* Software Trigger Start */
	while(!(TSI0_GENCS & TSI_GENCS_EOSF_MASK)) {
	}
	TSI0_GENCS |= TSI_GENCS_EOSF_MASK;	// Clear the EOSF (End of Scan) flag

	oscCount = *(electrode->counterRegister);

	/* Returns 1 when touch sensor is depressed and 0 otherwise */

	return (oscCount > electrode->threshold);
}

int ts_fputc(char c, void *ptr)
{
	return(OUPUT_ONLY_DEVICE);
}

int ts_fopen(void *ptr)
{
	return(SUCCESS);
}

int ts_fclose(void *ptr)
{
	return(SUCCESS);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */

int morse_led_fgetc(void *ptr)
{
	return(OUPUT_ONLY_DEVICE);
}

int morse_led_fputc(char c, void *ptr)
{
	GlobalMorseLed = (struct LED *)ptr;
	char tempMorseCode[12]; 
	int id,i=0;

	/* Make sure the character is a supported character to output to morse led */	
	if(isAlphabet(c))
	{
		id = findAlphabetIndex(c);
		strcpy_fcn(tempMorseCode,morseCharTable[id].morseCode);
	}	
	else if(isDigit(c))
	{
		id = findDigitIndex(c);
		strcpy_fcn(tempMorseCode,morseNumberTable[id].morseCode);
	}
	else if(isSupportedSpecialChars(c,&id))
	{
		strcpy_fcn(tempMorseCode,morseSpecialCharsTable[id].morseCode);
	}
	else if(c == ' ')
	{
		doNothingState = ON;
		SVCsetPDBTimerImpl(TIME_BETWEEN_WORDS, &doNothing, PDBTimerOneShot);			
		while(doNothingState);
		/* Let user know if space was transmitted by breaking the line twice*/
		SVCfputs_mainImpl(STDOUT,"\r\n");
		return(SUCCESS);
	}
	else
	{
		return(INVALID_MORSE_INPUT_CHAR);
	}


	/* Transmit morse code over the LED for the incoming character */

	/* Start a timer to turn the LED OFF */
	while(tempMorseCode[i])
	{
		/* Turn on the LED */
		led_fputc(ON,GlobalMorseLed); 
		ledState = ON;
		doNothingState = ON;

		switch(tempMorseCode[i])
		{

		case '.':
			/* Let user know what will be transmitted */
			SVCfputc_mainImpl(STDOUT,'.');

			SVCsetPDBTimerImpl(DOT_TIME, &myMorseToggleFcn, PDBTimerOneShot);
			while(ledState){}; /* loop infinite until the LED state goes off */

			if(tempMorseCode[i+1] != NULL)
			{	
				SVCsetPDBTimerImpl(TIME_BETWEEN_DOTS_OR_DASHES_OR_BOTH, &doNothing, PDBTimerOneShot);			
				while(doNothingState){};
			}			
			break;

		case '-':
			/* Let user know what will be transmitted */
			SVCfputc_mainImpl(STDOUT,'-');

			/*The duration of a dash is three times the duration of a dot.*/
			SVCsetPDBTimerImpl(DASH_TIME, &myMorseToggleFcn, PDBTimerOneShot);
			while(ledState){}; /* loop infinite until the LED state goes off */

			if(tempMorseCode[i+1] != NULL)
			{
				/*Each dot or dash is followed by a short silence, equal to the dot duration.*/
				SVCsetPDBTimerImpl(TIME_BETWEEN_DOTS_OR_DASHES_OR_BOTH, &doNothing, PDBTimerOneShot);
				while(doNothingState){};
			}
			break;
		}

		i++;
	}

	/* Break the line when one character is transmitted for user to visulize on UART*/
	SVCfputs_mainImpl(STDOUT,"\r\n");

	return(SUCCESS);
}

int morse_led_fopen(void *ptr)
{
	return(SUCCESS);
}

int morse_led_fclose(void *ptr)
{
	return(SUCCESS);
}

void myMorseToggleFcn(void)
{

	if((ledState ^= 1) == ON)
	{
		/* Turn the LED on */
		PTA_BASE_PTR->PCOR = GlobalMorseLed->MASK;
	}
	else
	{
		/* Turn the LED off */
		PTA_BASE_PTR->PSOR = GlobalMorseLed->MASK;
	}
}

void doNothing(void)
{
	/* This function does nothing, it only toggles doNothingState*/
	doNothingState ^= 1;
}

int findAlphabetIndex(char c)
{
	int ch = (int) c;

	if((ch>=65 && ch<=90)) /* Capital letters*/
	{
		return(ch - 'A');
	}		
	else if(ch>=97 && ch<=122) /* Small letters*/
	{
		return(ch - 'a');
	}

	return(SUCCESS);
}

int findDigitIndex(char ch)
{
	return((int)ch - '0');
}

int isSupportedSpecialChars(char ch,int* idx)
{
	int i = 0;

	for(i=0;i<TOTAL_NUM_OF_SPECIAL_CHARS;i++)
	{
		if(ch == morseSpecialCharsTable[i].asciiSpecialChars)
		{
			*idx = i;
			return(true);
		}
	}	

	return(false);
}
