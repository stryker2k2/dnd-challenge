#include <stdio.h>
#include <string.h>

int logToFile(char *logTxt)
{ 
    char tmpLog[64];
    printf("[+] logging to file\n");
    sprintf(tmpLog, "[+] %s", logTxt);
    FILE *logFile = fopen("log.txt", "w");
    if (logFile == NULL)
    {
        return 1;
    }

    fprintf(logFile, "%s\n", tmpLog);
    fclose(logFile);

    return 0;
}

int main(int argc, char *argv[])
{
    logToFile(argv[1]);
}



// How to Compile:
// gcc -m32 -no-pie -O0 -Wno-format-truncation -fno-stack-protector bronze.c -o bronze