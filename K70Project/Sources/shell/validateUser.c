/*
 * validateUser.c
 *
 *  Created on: Apr 11, 2017
 *      Author: Jyot Buch
 *      
 * NOTE: Currently all the users are privilege 
 */

#include "../util/util.h"
#include "../device/deviceEnums.h"
#include "../svc/svc.h"
#include "../util/reportError.h"
int validateUser(bool);

struct userTable
{
	char* username;
	char* password;
}userTable1[] = {
		{"admin","password"},
		{"jyotbuch","myos"},
		{"guest","guest"}
};

#define TOTAL_NUMBER_OF_USERS sizeof(userTable1)/sizeof(userTable1[0])
#define CHAR_LOGOUT 4

int validateUser(bool flag)
{
	/* To control the behaviour, if you want this to validate username and passward, set it to true. 
	 * currently disabled for testing purpose*/	
	bool usernameMatched = false,passwordMatched = false;
	char ustring[256],pstring[256];
	int i, userID;

	/* Make default login to be guest if flag is set to false */
	if(!flag) strcpy_fcn(currentUser,"guest");

	if(flag) svc_fputs_main(STDOUT, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n");

	while(flag)
	{
		/**********************************************************************/
		USERNAMELOOP:
		/* Ask for USERNAME */
		svc_fputs_main(STDOUT, "username: ");

		/* read the username string */
		for(i = 0; i < 256; i++)
		{
			ustring[i] = (char)svc_fgetc_main(STDIN);

			if((ustring[i] == '\r') || (ustring[i] == '\n'))
			{
				ustring[i] = '\0'; /* Overwrite the char and terminate string by NULL */
				svc_fputs_main(STDOUT, "\r\n");
				break;
			}

			if((ustring[i] == CHAR_LOGOUT))
			{
				goto LOGOUT;
			}

			svc_fputc_main(STDOUT,ustring[i]); /* Echo back the character */
		}		

		if(ustring[0]=='\0')goto USERNAMELOOP;

		/**********************************************************************/
		PASSWORDLOOP:
		/* Ask for PASSWORD */
		svc_fputs_main(STDOUT, "password: ");

		/* Read the password string */
		for(i = 0; i < 256; i++)
		{
			pstring[i] = (char)svc_fgetc_main(STDIN);

			if((pstring[i] == '\r') || (pstring[i] == '\n'))
			{
				pstring[i] = '\0';  /* Overwrite the char and terminate string by NULL */
				svc_fputs_main(STDOUT, "\r\n");
				break;
			}

			if((pstring[i] == CHAR_LOGOUT))
			{
				goto LOGOUT;
			}

			svc_fputc_main(STDOUT,'*');
		}

		if(pstring[0]=='\0')goto PASSWORDLOOP;
		/**********************************************************************/
		/* Compare username string with database */
		for(i=0; i<TOTAL_NUMBER_OF_USERS; i++)
		{
			if(strcmp_fcn(userTable1[i].username,&ustring[0]))
			{
				usernameMatched = true;
				userID = i;
				break;
			}
		}

		if(i!=TOTAL_NUMBER_OF_USERS)
		{
			if(strcmp_fcn(userTable1[userID].password,&pstring[0]))
				passwordMatched = true;
		}

		/**********************************************************************/
		/* if both username and passward matches then exit the loop */
		if(usernameMatched && passwordMatched)
		{
			/* he only exit point for this loop is here by setting the flag to false.*/
			flag = false;
			strcpy_fcn(currentUser,ustring);
		}
		else
		{
			svc_reportError(INVALID_CREDENTIALS);
		}

		LOGOUT:
		if(!usernameMatched && ((ustring[0] == CHAR_LOGOUT)||(pstring[0] == CHAR_LOGOUT)))
		{
			svc_fputs_main(STDOUT, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n");
			return(TERMINATE);
		}
		/**********************************************************************/		
	}

	if(flag) svc_fputs_main(STDOUT, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n");

	/* If user is not admin then set the time of day clock to 0*/
	if(!isPrivilegedUsrLogin()) 
		svc_settimeofday(0);

	return(LOGIN_SUCCESSFULL);
}

