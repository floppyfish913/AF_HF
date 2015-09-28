#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main()
{
    char str1[]="kaka";
    char buff[100];
    sprintf(buff,"asdsadsadsad %f",1.43);
    sprintf(buff,"aaaaa %f",1.43);
    strncat (str1, buff,999);
    printf("%d\n",strlen(str1));
    printf("%s",str1);

    return 0;
}

