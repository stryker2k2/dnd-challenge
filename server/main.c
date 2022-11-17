#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

BOOL connected;
char tmpToast[512];

int storyMode(int mySock);

int killSock(int mySock)
{
    char *connectionTerminated = ("\nConnection has been terminated.\n"
                                "Press \"CTRL+C\" to free your terminal window.\n");
    connected = FALSE;

    send(mySock, connectionTerminated, strlen(connectionTerminated), 0);
    printf("[+] Connection Terminated\n");

    shutdown(mySock, SD_BOTH);
    closesocket(mySock);
}

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

int validateMultipleChoiceInput(int mySock)
{
    char *invalidChoice = "[!] Invalid Choice\n";
    u_long bytesAvailable = 0;
    char *end;
    char numChoice[2];
    int choice;

    memset(numChoice, 0, sizeof(numChoice));
    
    do
    {
        ioctlsocket(mySock, FIONREAD, &bytesAvailable);            
    } while (!bytesAvailable);

    if (bytesAvailable > 2)
    {
        emptyBuffer(mySock);
        send(mySock, invalidChoice, strlen(invalidChoice), 0);
        printf("[+] Sent \"Invalid Choice\"\n");

        bytesAvailable = 0;
        return 0;
    }

    recv(mySock, numChoice, sizeof(numChoice), 0);
    numChoice[1] = 0x00;
    
    choice = (int)strtol(numChoice, &end, 10);
    if (numChoice == end)
    {
        send(mySock, invalidChoice, strlen(invalidChoice), 0);
        printf("[+] Sent \"Invalid Choice\"\n");
        
        memset(numChoice, 0, sizeof(numChoice));
        return 0;
    }

    return choice;
}

int getToast(int mySock)
{ 
    // char tmpToast[512];
    // recv(mySock, tmpToast, sizeof(tmpToast), 0);
    // strcpy(playerToast, tmpToast);

    // char hello[] = "hello";
    // char AAAA[] = ("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    //strcpy(hello, AAAA);

    memset(tmpToast, 0, sizeof(tmpToast));
    recv(mySock, tmpToast, 4096, 0);

    return 0;
}

int orderAle(int mySock, char *characterName)
{
    if (!connected) return 1;
    char *proposeToast = ("\n\nThe Bartender slides a cold frosty mug of the finest \n"
                        "ale this side of Buldur's Bridge. He fills a mug up for \n"
                        "himself and raises it high. He suggests that you propose \n"
                        "a toast. To what cause shall we drink to?\n\n"
                        "[>] ");
    char *toastProposed = ("\nYou, the mighty %s, stand up proud with "
                            "your frosty of ale and make a toast to "
                            "%s!\n"
                            "The entire tavern erupts into loud bolsterous cheer "
                            "and celebration! All hail, %s!\n");
    char *returnMenu = ("What shall you do now?\n"
                        "[1] Return to Main Menu\n"
                        "[2] Terminate Connection\n\n"
                        "[>] ");
    char *invalidChoice = "[!] Invalid Choice\n\n";
    char playerToast[256];
    char output[2048];
    int choice;

    send(mySock, proposeToast, strlen(proposeToast), 0);
    printf("[+] Sent \"proposeToast\"\n");
    
    getToast(mySock);

    memset(playerToast, 0, sizeof(playerToast));
    strcpy(playerToast, tmpToast);

    for (int i = 0; i < strlen(playerToast); i++)
    {
        if (playerToast[i] == 0x0A)
        {
            playerToast[i] = 0x00;
        }
    }

    snprintf(output, sizeof(output), toastProposed, characterName, playerToast, characterName);
    send(mySock, output, strlen(output), 0);
    printf(output);

    while (TRUE)
    {
        send(mySock, returnMenu, strlen(returnMenu), 0);
        printf("[+] Sent \"returnMenu\"\n");

        if (choice = validateMultipleChoiceInput(mySock))
        {          
            switch(choice)
            {
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
    if (!connected) return 1;

    int bytesRcvd = 0;

    char *enterYourName = ("\n\nThe game is a Dungeons and Dragons type text-based game.\n"
                            "It wants you to pick a name. \"Champion, what name do you \n"
                            "hail by?\"\n\n"
                            "[>] ");
    char *drinkOrder = ("You find yourself at a Tavern ordering yourself a drink.\n" 
                        "What do you order?\n"
                        "[1] Ale, of course!\n"
                        "[2] Bartender's Choice\n"
                        "[3] Water, please.\n"
                        "[4] Return to Main Menu\n"
                        "[5] Terminate Connection\n\n"
                        "[>] ");
    char *invalidChoice = "[!] Invalid Choice\n";

    //char *namePtr = (char*)malloc(64);
    char characterName[2048] = { "" };
    int choice;

    //memset(namePtr, 0, sizeof(namePtr));

    send(mySock, enterYourName, strlen(enterYourName), 0);
    printf("[+] Sent \"Enter Your Name\"\n");

    bytesRcvd = recv(mySock, characterName, sizeof(characterName), 0);

    //strcpy(namePtr, characterName);

    for (int i = 0; i < strlen(characterName); i++)
    {
        if (characterName[i] == 0x0A)
        {
            characterName[i] = 0x00;
        }
    }
    
    char hailPlayer[1024];

    snprintf(hailPlayer, sizeof(hailPlayer), "\n\nHail, %s!\n", characterName);

    strncat(hailPlayer, drinkOrder, sizeof(hailPlayer));

    while (TRUE)
    {
        send(mySock, hailPlayer, strlen(hailPlayer), 0);
        printf("[+] Sent \"hailPlayer\" and \"drinkOrder\"\n");

        if (choice = validateMultipleChoiceInput(mySock))
        {         
            switch(choice)
            {
                case 1:
                    printf("[+] Choice 1 Selected\n");
                    orderAle(mySock, characterName);
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
    char *welcome = ("\n\n*** Welcome to the Black Lantern Security Interview ***\n\n"
                    "You walk into the interview room and the interviewer gives you \n"
                    "a laptop with a text-based role playing game installed on it.\n"
                    "What do you do?\n");
    char *welcomeOptions = ("[1] Play the game\n"
                    "[2] Compliment the Interviewer on the attire\n"
                    "[3] Hack the text-based game\n"
                    "[4] Roll your eyes and leave the interview\n"
                    "[5] Terminate Connection\n\n"
                    "[>] ");
    char *invalidChoice = "[!] Invalid Choice\n\n\n";
    char *doneMessage = "[+] Program Complete\n\n";

    send(mySock, welcome, strlen(welcome), 0);
    printf("[+] Sent \"Welcome\"\n");

    while (TRUE)
    {
        send(mySock, welcomeOptions, strlen(welcomeOptions), 0);
        printf("[+] Sent \"Welcome Options\"\n");

        if (choice = validateMultipleChoiceInput(mySock))
        {            
            switch(choice)
            {
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

        connected = TRUE;

        while (connected)
        {
            storyMode(new_sock);
        }
        
        /* Cleanup */
        shutdown(new_sock, SD_BOTH);
        closesocket(new_sock);
    }

    shutdown(server_fd, SD_BOTH);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}