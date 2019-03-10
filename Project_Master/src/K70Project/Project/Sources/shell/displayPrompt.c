#include "../device/deviceEnums.h"
#include "../svc/svc.h"

void displayPrompt(void);

void displayPrompt()
{	
    /* Display a "$ " Prompot */
	svc_fputs_main(STDOUT, "$ ");
}
