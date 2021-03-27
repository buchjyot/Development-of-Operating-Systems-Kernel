/*
 * deviceFcnDeclaration.h
 *
 *  Created on: Apr 8, 2017
 *      Author: welcome
 */

#ifndef DEVICEFUNCTIONDECLARATION_H_
#define DEVICEFUNCTIONDECLARATION_H_

int SVCfgetc_mainImpl(int);
int SVCfputc_mainImpl(int,char);
int SVCfopen_mainImpl(char *,char *);
int SVCfclose_mainImpl(int);
int SVCfputs_mainImpl(int, char *);
int SVCfgets_mainImpl(int);
int SVCcreateImpl(char *);
int SVCdeleteImpl(char *);

/* Following functions are core device specific functions which will be called only in privilege mode*/
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
int uart_fputc(char, void *);
int uart_fopen(void *);
int uart_fclose(void *);

int lcdc_fgetc(void *);
int lcdc_fputc(char, void *);
int lcdc_fopen(void *);
int lcdc_fclose(void *);

int adc_fgetc(void *);
int adc_fputc(char, void *);
int adc_fopen(void *);
int adc_fclose(void *);

int ts_fgetc(void *);
int ts_fputc(char, void *);
int ts_fopen(void *);
int ts_fclose(void *);

int morse_led_fgetc(void *);
int morse_led_fputc(char, void *);
int morse_led_fopen(void *);
int morse_led_fclose(void *);

#endif /* DEVICEFUNCTIONDECLARATION_H_ */
