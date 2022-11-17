#include <stdio.h>
#include <string.h>

int main()
{
    char str[6] = "hello";
    char overflow[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    strcpy(str, overflow);

    printf("str: \"%s\"", str);

    return 0;
}