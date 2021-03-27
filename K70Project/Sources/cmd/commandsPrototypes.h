/*
 * cmdPrototypes.h
 *
 *  Created on: May 7, 2017
 *      Author: Jyot Buch
 */

#ifndef CMDPROTOTYPES_H_
#define CMDPROTOTYPES_H_

/* Available Commands */
int cmd_date(int, char **);
int cmd_echo(int, char **);
int cmd_exit(int, char **);
int cmd_help(int, char **);
int cmd_malloc(int, char **);
int cmd_free(int, char **);
int cmd_memorymap(int, char **);
int cmd_freeerrorcode(int, char **);
int cmd_fgetc(int, char **);
int cmd_fputc(int, char **);
int cmd_fclose(int, char **);
int cmd_fopen(int, char **);
int cmd_create(int, char **);
int cmd_delete(int, char **);
int tmemtest(int, char **);
int cmd_ls(int, char **);
int cmd_fputs(int argc, char **);
int cmd_fgets(int argc, char **);
int loopLEDDemo(int, char **);
int cmd_touch2led(int, char **);
int cmd_pot2ser(int, char **);
int cmd_therm2ser(int, char **);
int cmd_pb2led(int, char **);
int cmd_ser2lcd(int, char **);
int cmd_examine(int, char **);
int cmd_deposit(int, char **);
int cmd_flashled(int, char **);
int cmd_whoami(int, char **);
int cmd_multitask(int, char **);
int cmd_pb2ser(int, char **);
int cmd_tasklist(int, char **);

#endif /* CMDPROTOTYPES_H_ */
