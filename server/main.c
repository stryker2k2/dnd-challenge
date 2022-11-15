#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

int emptyBuffer(int mySock)
{   
    char trash[1024] = { 0 };
    int bytesRcvd = 0;

    do 
    {
        bytesRcvd = recv(mySock, trash, sizeof(trash), 0);
        printf("[~] Deleting the excess trash of %d bytes\n", bytesRcvd);

        if (bytesRcvd < sizeof(trash))
        {            
            memset(trash, 0, sizeof(trash));
            break;
        }
        
    } while (bytesRcvd);

    return 0;
}

BOOL validateMultipleChoiceInput(int mySock)
{
    char *invalidChoice = "[!] Invalid Choice\n\n";
    u_long bytesAvailable = 0;
        do
        {
            ioctlsocket(mySock, FIONREAD, &bytesAvailable);            
        } while (!bytesAvailable);

        // printf("[+] Bytes Available: %d\n", bytesAvailable);

        if (bytesAvailable > 2)
        {
            emptyBuffer(mySock);
            send(mySock, invalidChoice, strlen(invalidChoice), 0);
            printf("[+] Sent \"Invalid Choice\"\n");

            bytesAvailable = 0;
            return FALSE;
        }

    return TRUE;
}

int playGame(int mySock)
{
    int bytesRcvd = 0;

    char *enterYourName = ("\n\nThe game is a Dungeons and Dragons type text-based game.\n"
                            "It wants you to pick a name. \"Champion, what name do you \n"
                            "hail by?\"\n\n"
                            "[>] ");
    
    char *namePtr = (char*)malloc(1024);
    char characterName[2048] = { "" };

    memset(namePtr, 0, strlen(namePtr));

    send(mySock, enterYourName, strlen(enterYourName), 0);
    printf("[+] Sent \"Enter Your Name\"\n");

    bytesRcvd = recv(mySock, characterName, sizeof(characterName), 0);

    strcpy(namePtr, characterName);

    for (int i = 0; i < strlen(characterName); i++)
    {
        if (characterName[i] == 0x0A)
        {
            characterName[i] = 0x00;
        }
    }

    send(mySock, characterName, strlen(namePtr), 0);
    printf("[+] Sent \"Player Name is %s\"\n", characterName);

    return 0;
}

int storyMode(int mySock)
{
    char numChoice[2] = { 0 };   
    char buffer[1024] = { 0 }; 
    int bytesRcvd = 0;
    char tmp[64];
    int choice;

    /* Read from New Connection */
    char *welcome = ("*** Welcome to the Black Lantern Security Interview ***\n\n"
                    "You walk into the interview room and the interviewer gives you \n"
                    "a laptop with a text-based role playing game installed on it.\n"
                    "What do you do?\n");
    char *welcomeOptions = ("[1] Play the game\n"
                    "[2] Compliment the Interviewer on the attire\n"
                    "[3] Hack the text-based game\n"
                    "[4] Roll your eyes and leave the interview\n\n"
                    "[>] ");
    char *invalidChoice = "[!] Invalid Choice\n\n";
    char *doneMessage = "[+] Program Complete\n\n";

    send(mySock, welcome, strlen(welcome), 0);
    printf("[+] Sent \"Welcome\"\n");

    BOOL choiceVal = FALSE;

    while (TRUE)
    {
        send(mySock, welcomeOptions, strlen(welcomeOptions), 0);            
        printf("[+] Sent \"Welcome Options\"\n");

        if (validateMultipleChoiceInput(mySock))
        {
            char *end;

            bytesRcvd = recv(mySock, numChoice, sizeof(numChoice), 0);
            choice = (int)strtol(numChoice, &end, 10);
            if (numChoice == end)
            {
                send(mySock, invalidChoice, strlen(invalidChoice), 0);
                printf("[+] Sent \"Invalid Choice\"\n");
                
                memset(numChoice, 0, sizeof(numChoice));
            }
            else
            {
                choiceVal = TRUE;
            }
        }

        if (choiceVal)
        {
            break;
        }
    }
    
    switch(choice)
    {
        case 1:
            printf("[+] Choice 1 Selected\n");
            playGame(mySock);
            break;
        case 2:
            printf("[+] Choice 2 Selected\n");
            break;
        case 3:
            printf("[+] Choice 3 Selected\n");
            break;
        case 4:
            printf("[+] Choice 4 Selected\n");
            break;
        default:
            printf("[+] Default Selected\n");
            break;
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
    char tmp[64];
     
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
        printf("[+] Listening for Connection...\n");
        addrlen = sizeof(address);
        if ((new_sock = accept(server_fd, (struct sockaddr*) &address, &addrlen)) == INVALID_SOCKET)
        {
            perror("[-] Accept Socket failure");
            exit(EXIT_FAILURE);
        }

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