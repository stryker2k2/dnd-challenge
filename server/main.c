#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

int sockHandle;

int storyMode(int mySock);
int validateBytes(char *inputString);
int killSock(int mySock);

int printKey()
{
    FILE *keyFile;

    char *banner = "\r\n============= BLS CTF KEY ============\n\r";
    char *tryAgain = "\rPeform this same overflow on the server to find the key!\n";
    char output[128];
    char key[44];
    int counter = 0;
    char ch;

    send(sockHandle, banner, strlen(banner), 0);

    keyFile = fopen("C:\\key.txt", "r");
    if (keyFile == NULL)
    {
        send(sockHandle, tryAgain, strlen(tryAgain), 0);
    }

    fgets(key, sizeof(key), keyFile);
    sprintf(output, "\r%s\n\n", key);
    send(sockHandle, output, strlen(output), 0);

    Sleep(500);

    fclose(keyFile);

    killSock(sockHandle);

    return 0;
}

int killSock(int mySock)
{
    char *connectionTerminated = ("\nConnection has been terminated.\n"
                                "\rPress \"CTRL+C\" to free your terminal window.\n");

    send(mySock, connectionTerminated, strlen(connectionTerminated), 0);
    printf("[+] Connection Terminated\n");

    Sleep(1);
    shutdown(mySock, SD_BOTH);
    closesocket(mySock);
}

int emptyBuffer(int mySock)
{     
    char trash[1024];
    int bytesRcvd = 0;
    int isPuTTY = 0;

    do 
    {
        bytesRcvd = recv(mySock, trash, sizeof(trash), 0);
        isPuTTY = validateBytes(trash);
        
        printf("[~] Deleting the excess trash of %d bytes\n", bytesRcvd);

        if (bytesRcvd < sizeof(trash))
        {            
            memset(trash, 0, sizeof(trash));
            break;
        }
        
    } while (bytesRcvd);

    if (isPuTTY = TRUE)
    {
        return TRUE;
    }

    return 0;
}

int validateMultipleChoiceInput(int mySock)
{
    char *invalidChoice = "[!] Invalid Choice\n\r";
    u_long bytesAvailable = 0;
    char *end;
    char numChoice[2];
    int choice;
    int isPuTTY = 0;

    memset(numChoice, 0, sizeof(numChoice));
    
    do
    {
        ioctlsocket(mySock, FIONREAD, &bytesAvailable);            
    } while (!bytesAvailable);

    if (bytesAvailable > 2)
    {
        isPuTTY = emptyBuffer(mySock);
        if (isPuTTY)
        {
            bytesAvailable = 0;
            return -1;
        }
        send(mySock, invalidChoice, strlen(invalidChoice), 0);
        printf("[+] Sent \"Invalid Choice\"\n\r");

        return 0;
    }

    recv(mySock, numChoice, sizeof(numChoice), 0);
    isPuTTY = validateBytes(numChoice);
    if (isPuTTY) return -1;
    numChoice[1] = 0x00;
    
    choice = (int)strtol(numChoice, &end, 10);
    if (numChoice == end)
    {
        send(mySock, invalidChoice, strlen(invalidChoice), 0);
        printf("[+] Sent \"Invalid Choice\"\n\r");
        
        memset(numChoice, 0, sizeof(numChoice));
        return 0;
    }

    return choice;
}

int validateBytes(char *inputString)
{
    // printf("[~] ");
    for (int i = 0; i < strlen(inputString); i++)
    {
        if ((BYTE)inputString[i] == 0x0d)
        {
            if ((BYTE)inputString[i+1] == 0x0a)
            {
                // printf("PuTTY (0x0d, 0x0a)\n");
                return 1;
            }
        }

        else if ((BYTE)inputString[i] == 0xff)
        {
            if ((BYTE)inputString[i+1] == 0xfb)
            {
                // printf("PuTTY (0xff, 0xfb)\n");
                return 1;
            }
        }

        else
        {
            // printf("\n");
            // printf("\\x%02x ", inputString[i]);
            return 0;
        }
    }

    return 0;
}

/* TODO: Create logging function */
int logMe(char *logme)
{ 
    char tmpLog[64];

    strcpy(tmpLog, logme);

    return 0;
}

int orderAle(int mySock, char *characterName)
{
    char *proposeToast = ("\r\n\nThe Bartender slides a cold frosty mug of the finest \n\r"
                        "ale this side of Buldur's Bridge. He fills a mug up for \n\r"
                        "himself and raises it high. He suggests that you propose \n\r"
                        "a toast. To what cause shall we drink to?\n\n\r"
                        "[>] ");
    char *toastProposed = ("\r\n\nYou, the mighty %s, stand up proud with your frosty \n\r"
                            "mug of ale and make a toast to %s!\n\r"
                            "The entire tavern erupts into loud bolsterous cheer \n\r"
                            "and celebration! All hail %s!\n\r");
    char *returnMenu = ("What shall you do now?\n\r"
                        "[1] Return to Main Menu\n\r"
                        "[2] Terminate Connection\n\n\r"
                        "[>] ");
    char *invalidChoice = "[!] Invalid Choice\n\n\r";
    char playerToast[1024];
    char output[2048];
    int choice;

    send(mySock, proposeToast, strlen(proposeToast), 0);
    printf("[+] Sent \"proposeToast\"\n");

    recv(mySock, playerToast, sizeof(playerToast), 0);
    if(validateBytes(playerToast))
    {
        recv(mySock, playerToast, sizeof(playerToast), 0);
    }
    
    logMe(playerToast);

    for (int i = 0; i < strlen(playerToast); i++)
    {
        if (playerToast[i] == 0x0A)
        {
            playerToast[i] = 0x00;
        }
    }

    for (int i = 0; i < strlen(characterName); i++)
    {
        if (characterName[i] == 0x0A)
        {
            characterName[i] = 0x00;
        }
    }

    snprintf(output, sizeof(output), toastProposed, characterName, playerToast, characterName);
    send(mySock, output, strlen(output), 0);

    while (TRUE)
    {
        send(mySock, returnMenu, strlen(returnMenu), 0);
        printf("[+] Sent \"returnMenu\"\n");

        while (choice = validateMultipleChoiceInput(mySock))
        {          
            switch(choice)
            {
                case -1:
                    break;
                case 1:
                    printf("[+] Choice 1 Selected\n");
                    storyMode(mySock);
                    return 0;
                case 2:
                    printf("[+] Choice 2 Selected\n");
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    printf("[+] Sent \"Invalid Choice\"\n");
                    break;
            }
        }
    }

    return 0;
}

int playGame(int mySock)
{

    char *enterYourName = ("\r\n\nThe game is a Dungeons and Dragons type text-based game.\n\r"
                            "It wants you to pick a name. \"Champion, what name do you \n\r"
                            "hail by?\"\n\n\r"
                            "[>] ");
    char *drinkOrder = ("\r\n\nHail, Adventurer! You find yourself at a Tavern ordering \n\r"
                        "yourself a drink. What do you order?\n\r"
                        "[1] Ale, of course!\n\r"
                        "[2] Bartender's Choice\n\r"
                        "[3] Water, please.\n\r"
                        "[4] Return to Main Menu\n\r"
                        "[5] Terminate Connection\n\n\r"
                        "[>] ");
    char *invalidChoice = "[!] Invalid Choice\n\r";

    char characterName[2048] = { "" };
    int choice;

    send(mySock, enterYourName, strlen(enterYourName), 0);
    printf("[+] Sent \"Enter Your Name\"\n");

    recv(mySock, characterName, sizeof(characterName), 0);
    if(validateBytes(characterName))
    {
        recv(mySock, characterName, sizeof(characterName), 0);
    }

    // char *characterPtr = (char *)malloc(sizeof(characterName));
    // memcpy(characterPtr, characterName, sizeof(characterName));

    // memset(characterName, 0, sizeof(characterName));

    while (TRUE)
    {
        send(mySock, drinkOrder, strlen(drinkOrder), 0);
        printf("[+] Sent \"drinkOrder\"\n");

        while (choice = validateMultipleChoiceInput(mySock))
        {         
            switch(choice)
            {
                case -1:
                    break;
                case 1:
                    printf("[+] Choice 1 Selected\n");
                    orderAle(mySock, characterName);
                    // free(characterPtr);
                    return 0;
                case 2:
                    printf("[+] Choice 2 Selected\n");
                    return 0;
                case 3:
                    printf("[+] Choice 3 Selected\n");
                    return 0;
                case 4:
                    printf("[+] Choice 4 Selected\n");
                    return 0;
                case 5:
                    printf("[+] Choice 5 Selected\n");
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    printf("[+] Sent \"Invalid Choice\"\n");
                    break;
            }
        }
    }

    return 0;
}

int storyMode(int mySock)
{
    char numChoice[2] = { 0 };   
    char buffer[1024] = { 0 }; 
    int bytesRcvd = 0;
    int choice;

    /* Read from New Connection */
    char *welcome = ("\n\n*** Welcome to the Black Lantern Security Interview ***\n\n\r"
                    "You walk into the interview room and the interviewer gives you \n\r"
                    "a laptop with a text-based role playing game installed on it.\n\r"
                    "What do you do?\n\r");
    char *welcomeOptions = ("[1] Play the game\n\r"
                    "[2] Compliment the Interviewer on the attire\n\r"
                    "[3] Hack the text-based game\n\r"
                    "[4] Roll your eyes and leave the interview\n\r"
                    "[5] Terminate Connection\n\n\r"
                    "[>] ");
    char *invalidChoice = "[!] Invalid Choice\n\n\n\r";
    char *doneMessage = "[+] Program Complete\n\n\r";

    send(mySock, welcome, strlen(welcome), 0);
    printf("[+] Sent \"Welcome\"\n");

    while (TRUE)
    {
        send(mySock, welcomeOptions, strlen(welcomeOptions), 0);
        printf("[+] Sent \"Welcome Options\"\n");

        while (choice = validateMultipleChoiceInput(mySock))
        {            
            switch(choice)
            {
                case -1:
                    break;
                case 1:
                    printf("[+] Choice 1 Selected\n");
                    playGame(mySock);
                    killSock(mySock);
                    return 0;
                case 2:
                    printf("[+] Choice 2 Selected\n");
                    killSock(mySock);
                    return 0;
                case 3:
                    printf("[+] Choice 3 Selected\n");
                    killSock(mySock);
                    return 0;
                case 4:
                    printf("[+] Choice 4 Selected\n");
                    killSock(mySock);
                    return 0;
                case 5:                    
                    killSock(mySock);
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    printf("[+] Sent \"Invalid Choice\"\n");
                    break;
            }
        }
    }

    send(mySock, doneMessage, strlen(doneMessage), 0);        
    printf("[+] Sent \"Done Message\"\n\n");
    
    return 0;
}

int main(int argc, char *argv[])
{
    WORD wVersionRequested;
    WSADATA wsaData;
    DWORD port = 379;
    struct sockaddr_in address;
    int server_fd, new_sock, addrlen;
    
    int opt = 0;
    char *ip;
     
    /* IPv4 Setup */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    /* WSA Startup */
    wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        perror("[-] WSAStartup failure");
        exit(EXIT_FAILURE);
    }    
    
    /* Initiate Socket */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        perror("[-] Socket failure");
        exit(EXIT_FAILURE);
    }

    /* Set Socket Options */
    if (setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, (char *) &opt, sizeof(int)) == SOCKET_ERROR)
    {
        perror("[-] Set Socket Opt failure");
        exit(EXIT_FAILURE);
    }

    /* Bind Socket to local IP Address and Port */
    if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) == SOCKET_ERROR)
    {
        perror("[-] Bind Socket failure");
        exit(EXIT_FAILURE);
    }

    /* Start Listener */
    if (listen(server_fd, 3) == SOCKET_ERROR)
    {
        perror("[-] Listen Socket failure");
        exit(EXIT_FAILURE);
    }
    
    while (TRUE)
    {
        /* Accept Connection */
        printf("[+] You can connect to the server using \"ncat localhost 379\"\n");
        printf("[+] Listening for Connection...\n");
        addrlen = sizeof(address);
        if ((new_sock = accept(server_fd, (struct sockaddr*) &address, &addrlen)) == INVALID_SOCKET)
        {
            perror("[-] Accept Socket failure");
            exit(EXIT_FAILURE);
        }

        sockHandle = new_sock;
        storyMode(new_sock);
        
        /* Cleanup */
        shutdown(new_sock, SD_BOTH);
        closesocket(new_sock);
    }

    shutdown(server_fd, SD_BOTH);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}