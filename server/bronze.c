#include <stdio.h>
#include <string.h>

int logToFile()
{ 
    char tmpLog[64];

    printf("Enter Text: ");

    fgets(tmpLog, sizeof(tmpLog) * 10, stdin);

    printf("[+] logging to file\n");
    FILE *logFile = fopen("log.txt", "w");
    if (logFile == NULL)
    {
        return 1;
    }

    fprintf(logFile, "%s", tmpLog);
    fclose(logFile);

    fflush(stdout);

    return 0;
}

void printSecret() {
    int encoded_secret[] = {0x134C, 0x1375, 0x135B, 0x1342, 0x1304, 0x131A, 0x1303, 0x1359, 0x1353, 0x131A, 0x1307, 0x1345, 0x1356, 0x1359, 0x1350, 0x1304, 0x134A, 0x1337};

    int key = 0x1337;
    int i = 0;

    // The array has 18 elements. We need a buffer of size 19 (18 chars + 1 null byte).
    char decoded_buffer[19]; 

    // --- Decoding and Storing ---
    while ((encoded_secret[i] ^ key) != 0) {
        
        // Decode the character using XOR and cast it to a char
        char decoded_char = (char)(encoded_secret[i] ^ key);
        
        // Store the decoded character in the buffer
        decoded_buffer[i] = decoded_char;
        
        i++;
    }

    decoded_buffer[i] = '\0';

    printf("%s %s", "CTF-KEY", decoded_buffer);

    fflush(stdout);
}

int main(int argc, char *argv[])
{
    printf("Starting\n");
    logToFile();
}


// How to Compile:
// gcc -m32 -no-pie -O0 -Wno-format-truncation -w -fno-stack-protector bronze.c -o bronze