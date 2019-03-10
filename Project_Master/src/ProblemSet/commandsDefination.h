/*
 * This header file consist of commands definition which is
 * available for the shell
 *
 * It also defines the data structure for the command and their function
 * signature.
 */

#ifndef _COMMANDDEFINATION_H
#define _COMMANDDEFINATION_H

/* Available Commands */
int cmd_date(int, char **);
int cmd_echo(int, char **);
int cmd_exit(int, char **);
int cmd_help(int, char **);
int cmd_malloc(int, char **);
int cmd_free(int, char **);
int cmd_memorymap(int, char **);
int cmd_freeerrorcode(int, char **);

/* Utility to find the exact match for the command specified */
int stringMatch(char**);

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
    {"freeerrorcode", cmd_freeerrorcode},
    {"memorymap", cmd_memorymap}
};

/* Have NUMBER_OF_COMMANDS to be defined variables based on number of commands
 * which grows as we add new commands */
#define NUMBER_OF_COMMANDS sizeof(commands)/sizeof(commands[1])
/* Size of entire struct / size of one struct element */

#endif /* ifndef _COMMANDDEFINATION_H */
