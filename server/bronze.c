#include <stdio.h>
#include <string.h>

int logToFile(char *logTxt)
{ 
    char tmpLog[64];
    printf("[+] logging to file");
    sprintf(tmpLog, "[+] %s", logTxt);
    FILE *logFile = fopen("log.txt", "w");
    if (logFile == NULL)
    {
        return 1;
    }

    fprintf(logFile, "%s", tmpLog);
    fclose(logFile);

    return 0;
}

void printSecret() {
    int encoded_secret[] = {0x134C, 0x1375, 0x135B, 0x1342, 0x1304, 0x131A, 0x1303, 0x1359, 0x1353, 0x131A, 0x1307, 0x1345, 0x1356, 0x1359, 0x1350, 0x1304, 0x134A, 0x1337};

    int key = 0x1337;
    int i = 0;

    printf("CTF-KEY ");
    while ((encoded_secret[i] ^ key) != 0) {
        printf("%c", (char)(encoded_secret[i] ^ key));
        i++;
    }
}

int main(int argc, char *argv[])
{
    logToFile(argv[1]);
}


// How to Compile:
// gcc -m32 -no-pie -O0 -Wno-format-truncation -fno-stack-protector bronze.c -o bronze