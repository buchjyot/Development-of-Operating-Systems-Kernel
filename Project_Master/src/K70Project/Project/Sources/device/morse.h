/*
 * morse.h
 *
 *  Created on: Apr 15, 2017
 *      Author: welcome
 */

#ifndef MORSE_H_
#define MORSE_H_

/* The dot duration is the basic unit of time measurement in code transmission */

/* DOT_TIME = 500 milliseconds = 0.5 seconds*/
const int DOT_TIME = 500;

/* DASH_TIME = DOT_TIME*3 */
const int DASH_TIME = 500*3;

/* Each dot or dash is followed by a short silence, equal to the dot duration.*/
const int TIME_BETWEEN_DOTS_OR_DASHES_OR_BOTH = 500;

/* The letters of a word are separated by a space equal to three dots (one dash) */
const int TIME_BETWEEN_LETTERS_OF_A_WORD = 500*3;

/* The words are separated by a space equal to seven dots*/
const int TIME_BETWEEN_WORDS = 500*7;

void myMorseToggleFcn(void);
void doNothing(void);
int findAlphabetIndex(char);
int findDigitIndex(char);
int isSupportedSpecialChars(char,int*);

/* NOTE: The difference between uppercase A and lowercase a ascii value is 32 */

struct
{
	char* morseCode;
	char asciiChar;
} morseCharTable[] = {		
		{".-", 	  'A'},
		{"-...",  'B'},
		{"-.-.",  'C'},
		{"-..",   'D'},
		{".",     'E'},
		{"..-.",  'F'},
		{"--.",   'G'},
		{"....",  'H'},
		{"..",    'I'},
		{".---",  'J'},
		{"-.-",   'K'},
		{".-..",  'L'},
		{"--",    'M'},
		{"-.",    'N'},
		{"---",   'O'},
		{".--.",  'P'},
		{"--.-",  'Q'},
		{".-.",   'R'},
		{"...",   'S'},
		{"-",     'T'},
		{"..-",   'U'},
		{"...-",  'V'},
		{".--",   'W'},
		{"-..-",  'X'},
		{"-.--",  'Y'},
		{"--..",  'Z'}		
};

/* ASCII values from 48 to 57 */
struct 
{
	char* morseCode;
	char asciiNumbers;

}morseNumberTable[] ={
		{"-----", '0'},
		{".----", '1'},
		{"..---", '2'},
		{"...--", '3'},
		{"....-", '4'},
		{".....", '5'},
		{"-....", '6'},
		{"--...", '7'},
		{"---..", '8'},
		{"----.", '9'},
};


/* Supported Special Chars */
struct 
{
	char* morseCode;
	char asciiSpecialChars;

}morseSpecialCharsTable[] ={
		{".-.-.-", '.'}, //Period //46
		{"--..--", ','}, //Comma //44
		{"..--..", '?'}, //Query //63
		{"-...-",  '='}, //Equals //61
		{"-.-.--", '!'}, //Exclamation Point 
		{"-..-.",  '/'}, //Slash
		{"-.--.",  '('}, //Parenthesis 
		{"-.--.",  ')'}, //Close Parenthesis 
		{".-...",  '&'}, //Ampersand
		{"---...", ':'}, //colon
		{"-.-.-.", ';'}, //Semi-colon
		{".-.-.",  '+'}, //Plus Sign
		{"-....-", '-'}, //Minus Sign
		{"..--.-", '_'}, //underscore
		{".-..-.", '"'}, //Quatation Mark
		{"...-..-",'$'},//Dollar Sign
		{".--.-.", '@'}, //At Sign
		{".----.", '\''}, //Apostrophe
};

#define TOTAL_NUM_OF_SPECIAL_CHARS 18

#endif /* MORSE_H_ */
