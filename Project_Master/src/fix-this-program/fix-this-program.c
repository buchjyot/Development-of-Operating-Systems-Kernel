#include <stdio.h>

/* Return type of main function should not be void */
int main(void)
{   
    /* For loop declerations are only allowed in c99, hence declaring variable in the begening of the program*/
    int i;

    printf("hello, world\n");

    for(i=0;i<10;i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}
