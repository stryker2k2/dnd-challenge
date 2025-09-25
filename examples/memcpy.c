#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char characterName[2048] = { "johynnyboy" };

    char *ptr = (char *)malloc(sizeof(characterName));
    memcpy(ptr, &characterName, sizeof(characterName));

    printf("%s\n", characterName);
    printf("%s\n", ptr);
}