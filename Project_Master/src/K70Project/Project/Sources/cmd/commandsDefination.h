/*
 * This header file consist of commands definition which is
 * available for the shell
 *
 * It also defines the data structure for the command and their function
 * signature.
 */

#ifndef _COMMANDDEFINATION_H
#define _COMMANDDEFINATION_H

#include "commandsPrototypes.h"

/* Data structure for commands */
struct commandEntry
{
    char *name;
    int (*functionp)(int argc, char *argv[]);
    char *helpDisplayText;
} commands[] = {
    {"date", 			cmd_date, 			"date"},
    {"echo", 			cmd_echo,			"echo <string>"},
    {"exit", 			cmd_exit,   		"exit"},
    {"help", 			cmd_help,			"help OR help <command>"},
    {"malloc", 			cmd_malloc,			"malloc <size>"},
    {"free", 			cmd_free,			"free <address>"},
    {"freeerrorcode", 	cmd_freeerrorcode,  "freeerrorcode <address>"},
    {"memorymap", 		cmd_memorymap,  	"memorymap"},
    {"memtest", 		tmemtest,			"memtest"},
    {"fgetc", 			cmd_fgetc,			"fgetc <fd>"},
    {"fputc", 			cmd_fputc,			"fputc <fd> a"},
    {"fopen", 			cmd_fopen,  		"fopen <stream>"},
    {"fclose", 			cmd_fclose, 		"fclose <fd>"},
    {"create", 			cmd_create,			"create <filename>"},
    {"delete", 			cmd_delete,			"delete <filename"}, 
    {"ls", 				cmd_ls,				"ls"},
    {"fputs", 			cmd_fputs,			"fputs <fd> <string>"},
    {"fgets", 			cmd_fgets, 			"fgets <fd>"},
    {"ledloop", 		loopLEDDemo,		"ledloop"},
    {"touch2led", 		cmd_touch2led,		"touch2led"},
    {"pot2ser",			cmd_pot2ser,		"pot2ser"},
    {"therm2ser", 		cmd_therm2ser,		"therm2ser"},
    {"pb2led", 			cmd_pb2led,			"pb2led"},
    {"ser2lcd", 		cmd_ser2lcd, 		"ser2lcd"},
    {"examine",    		cmd_examine,  		"examine"},
    {"deposit",    		cmd_deposit,   		"deposit"},
    {"flashled", 		cmd_flashled,       "flashled"},
    {"whoami",          cmd_whoami,         "whoami"},
    {"multitask",       cmd_multitask,      "multitask"},
    {"pb2ser",		    cmd_pb2ser,         "pb2ser"},
    {"tasklist", 		cmd_tasklist,       "tasklist"}
};

/* Have NUMBER_OF_COMMANDS to be defined variables based on number of commands
 * which grows as we add new commands */
#define NUMBER_OF_COMMANDS sizeof(commands)/sizeof(commands[1])
/* Size of entire struct / size of one struct element */

#endif /* ifndef _COMMANDDEFINATION_H */
