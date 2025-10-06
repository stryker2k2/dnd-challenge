#include "defcon.h"

int sockHandle;
char *invalidChoice = "[!] Invalid Choice\n\n\r";
time_t tme;

int storyMode(int mySock);
int killSock(int mySock);
int validateMultipleChoiceInput(int mySock);
int returnMainMenu(int mySock);

int addNullTerminator(char *fixString)
{
    for (int i = 0; i < strlen(fixString); i++)
    {
        if (fixString[i] == 0x0A)
        {
            fixString[i] = 0x00;
        }
    }
}

int printKey()
{
    FILE *keyFile;

    char *banner = "\r\n============= BLS CTF KEY ============\n\r";
    char *tryAgain = "\rPeform this same overflow on the server to find the key!\n";
    char *contantInfo = "\rContact @stryker2k2 on Twitter to get the server IP!\n";
    char *keyExample = "\rThe key will look like BLS-CTF{Ex4mple_K3y} !\n";
    char output[1024];
    char key[64];
    int counter = 0;
    char ch;
    int cp = 1;

    send(sockHandle, banner, strlen(banner), 0);

#ifdef _WIN32
    keyFile = fopen("C:\\key.txt", "r");
#else
    keyFile = fopen("key.txt", "r");
#endif
    if (keyFile == NULL)
    {
        send(sockHandle, tryAgain, strlen(tryAgain), 0);
        send(sockHandle, contantInfo, strlen(contantInfo), 0);
        send(sockHandle, keyExample, strlen(keyExample), 0);
    }
    else
    {
        fgets(key, sizeof(key), keyFile);

        sprintf(output, "\r%s\n\n", key);
        send(sockHandle, output, strlen(output), 0);

        fclose(keyFile);
    }    

    sleep(1);
    killSock(sockHandle);

    return 0;
}

int killSock(int mySock)
{
    char *connectionTerminated = ("\r\nConnection has been terminated.\n"
                                "\rPress \"CTRL+C\" to free your terminal window.\n");

    send(mySock, connectionTerminated, strlen(connectionTerminated), 0);
    printf("[+] Connection Terminated\n");

    sleep(1);
#ifdef _WIN32
    shutdown(mySock, SD_BOTH);
    closesocket(mySock);
#else
    shutdown(mySock, SHUT_RDWR);
    close(mySock);
#endif
}

int sockTimeout(int mySock)
{
    char *timeout = ("\r\n\n*** Your session has timed out ***\n");
    uint32_t bytesAvailable = 0;    
    clock_t startTime, current;

    startTime = clock();

    do
    {
#ifdef _WIN32
        if (ioctlsocket(mySock, FIONREAD, (u_long*)&bytesAvailable) == -1) {
            perror("[-] IoCTL failure\n");
            exit(EXIT_FAILURE);
        }
#else
        if (ioctl(mySock, FIONREAD, &bytesAvailable) == -1) {
            perror("[-] IoCTL failure\n");
            exit(EXIT_FAILURE);
        }
#endif

        current = clock();
        if (((current - startTime) / CLOCKS_PER_SEC) > 30.0)
        {
            printf("[!] User session has timed out\n");
            send(mySock, timeout, strlen(timeout), 0);
            sleep(1);
            killSock(mySock);
            sleep(1);            
            return -2;
        }

    } while (!bytesAvailable);

    return bytesAvailable;
}

int returnMainMenu(int mySock)
{
    int choice;

    char *returnMenu = ("\rWhat shall you do now?\n"
                        "\r[1] Return to Main Menu\n"
                        "\r[2] Terminate Connection\n\n"
                        "\r[>] ");

    while (TRUE)
    {
        send(mySock, returnMenu, strlen(returnMenu), 0);
        printf("[+] Sent \"returnMenu\"\n");

        while (choice = validateMultipleChoiceInput(mySock))
        {          
            switch(choice)
            {
                case -2:
                    killSock(mySock);
                    return 0;
                case -1:
                    break;
                case 1:
                    printf("[+] Main Menu Selected\n");
                    storyMode(mySock);
                    return 0;
                case 2:
                    printf("[+] Disconnect Selected\n");
                    killSock(mySock);
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    send(mySock, returnMenu, strlen(returnMenu), 0);
                    printf("[+] Sent \"Invalid Choice & Return Menu\"\n");                    
                    break;
            }
        }
    }
}

int validateMultipleChoiceInput(int mySock)
{
    uint32_t bytesAvailable = 0;
    char *end;
    char numChoice[2];
    int choice;

    memset(numChoice, 0, sizeof(numChoice));

    if (sockTimeout(mySock) == -2)
    {
        return -2;
    }
    if (bytesAvailable > 2)
    {
        send(mySock, invalidChoice, strlen(invalidChoice), 0);
        printf("[+] Sent \"Invalid Choice\"\n\r");

        return 0;
    }
    
    recv(mySock, numChoice, sizeof(numChoice), 0);
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

int logToFile(char *logTxt)
{ 
    char tmpLog[64];
    printf("[+] logging to file\n");
    // strcpy(tmpLog, logTxt);
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

int perceptOne(int mySock)
{
    int choice;
    int bounces;
    char output[2048];
    char *pOneOptions = ("\rWhat shall you do now?\n"
                        "\r[1] Grab a paddle and flip it upwards in the air\n"
                        "\r[2] Try to bounce the ping pong ball on the paddle\n"
                        "\r[3] Return to Main Menu\n"
                        "\r[4] Terminate Connection\n\n"
                        "\r[>] ");

    char *pOneOptOne = ("\r\n\nSmoother than an Olympic Diver and as cunning as the \n"
                            "\rlocal Brazilian Jiu-Jitsu student, you gracefully fling the \n"
                            "\rpaddle upwards as it does a 720 degree backwards flip \n"
                            "\rand land elegantly back in your hand.\n");

    char *pOneOptTwo = ("\r\n\nYou firmly grab the ping pong paddle and with fierce \n"
                            "\rdetermination, you drop the ping pong ball onto the \n"
                            "\rpaddle and start bouncing it on the paddle. Ping - pong - \n"
                            "\rping - pong - you successfully bounce the ball %d \n"
                            "\rtimes before it bounces out of your control!\n");
    
    srand((unsigned) time(&tme));
    bounces = ((rand() % 20) + 5); // 5 to 25 bounces

    snprintf(output, sizeof(output), pOneOptTwo, bounces);
    
    while (TRUE)
    {
        send(mySock, pOneOptions, strlen(pOneOptions), 0);
        printf("[+] Sent \"pOneOptions\"\n");

        while (choice = validateMultipleChoiceInput(mySock))
        {          
            switch(choice)
            {
                case -2:
                    killSock(mySock);
                    return 0;
                case -1:
                    break;
                case 1:
                    printf("[+] Flip Paddle Selected\n");
                    send(mySock, pOneOptOne, strlen(pOneOptOne), 0);
                    returnMainMenu(mySock);
                    return 0;
                case 2:
                    printf("[+] Bounce Ball Selected\n");
                    send(mySock, output, strlen(output), 0);
                    returnMainMenu(mySock);
                    return 0;
                case 3:
                    printf("[+] Main Menu Selected\n");
                    storyMode(mySock);
                    return 0;
                case 4:
                    printf("[+] Disconnect Selected\n");
                    killSock(mySock);
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    send(mySock, pOneOptions, strlen(pOneOptions), 0);
                    printf("[+] Sent \"Invalid Choice & pOneOptions\"\n");                    
                    break;
            }
        }
    }
}

int perceptTwo(int mySock)
{
    int choice;
    char *pTwoOptions = ("\rWhat shall you do now?\n"
                        "\r[1] Eagerly brew yourself a cup of coffee!\n"
                        "\r[2] Kindly brew a cup for the Reverse Engineer guy\n"
                        "\r[3] Return to Main Menu\n"
                        "\r[4] Terminate Connection\n\n"
                        "\r[>] ");

    char *pTwoOptOne = ("\r\n\nYou eagerly make your amazing coffee selection and wait \n"
                            "\rforever for it to brew. It is too hot to drink right now but \n"
                            "\rthose freshly ground beans make it worth the wait!\n");

    char *pTwoOptTwo = ("\r\n\nYou calmly walk over to the resident Reverse Engineer guy and \n"
                            "\rgive him the cup of freshly brewed coffee. He smiles, takes \n"
                            "\ra sip, then starts to think outloud to himself. He says, \n"
                            "\r\"Ya know, I think there is a bug in my program. A user could \n"
                            "\reasily overflow the buffer. Hmmm, I should fix that.\"\n");
    
    while (TRUE)
    {
        send(mySock, pTwoOptions, strlen(pTwoOptions), 0);
        printf("[+] Sent \"pOneOptions\"\n");

        while (choice = validateMultipleChoiceInput(mySock))
        {          
            switch(choice)
            {
                case -2:
                    killSock(mySock);
                    return 0;
                case -1:
                    break;
                case 1:
                    printf("[+] Flip Paddle Selected\n");
                    send(mySock, pTwoOptOne, strlen(pTwoOptOne), 0);
                    returnMainMenu(mySock);
                    return 0;
                case 2:
                    printf("[+] Bounce Ball Selected\n");
                    send(mySock, pTwoOptTwo, strlen(pTwoOptTwo), 0);
                    returnMainMenu(mySock);
                    return 0;
                case 3:
                    printf("[+] Main Menu Selected\n");
                    storyMode(mySock);
                    return 0;
                case 4:
                    printf("[+] Disconnect Selected\n");
                    killSock(mySock);
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    send(mySock, pTwoOptions, strlen(pTwoOptions), 0);
                    printf("[+] Sent \"Invalid Choice & pOneOptions\"\n");                    
                    break;
            }
        }
    }
}

int perceptThree(int mySock)
{
    int choice;
    char *pThreeOptions = ("\rWhat shall you do now?\n"
                        "\r[1] Grab the Nerf Gun and go hunting for the BLS Owners\n"
                        "\r[2] Set up a Nerf Land Mine near the Nerf Machine Gun\n"
                        "\r[3] Return to Main Menu\n"
                        "\r[4] Terminate Connection\n\n"
                        "\r[>] ");

    char *pThreeOptOne = ("\r\n\nYou quickly grab the Nerf Nemesis Machine Gun with both \n"
                            "\rhands and sprint into the nearest Owner's office screaming \n"
                            "\r\"MEET MY LITTLE FRIEND\" at the top of your lungs. But, \n"
                            "\rinstead of a surprise attack - you walk into an ambush. All the \n"
                            "\rOwners unleash Nerf Fury upon you as you walk into the door.\n");

    char *pThreeOptTwo = ("\r\n\nWith amazing skill and finesse, you set up the Nerf Land \n"
                            "\rMine positioned perfect near the Nerf Nemesis Machine \n"
                            "\rGun and lie patiently in the other room with the remote \n"
                            "\rdetonator. One of the Owners walk towards it with delight \n"
                            "\rin their eyes! As you press the Remote Detonator, you \n"
                            "\rquickly remember that \"This Side Towards Enemy\" was \n"
                            "\rfacing you! You get pelted by your own Nerf Land Mine!\n");
    
    while (TRUE)
    {
        send(mySock, pThreeOptions, strlen(pThreeOptions), 0);
        printf("[+] Sent \"pOneOptions\"\n");

        while (choice = validateMultipleChoiceInput(mySock))
        {          
            switch(choice)
            {
                case -2:
                    killSock(mySock);
                    return 0;
                case -1:
                    break;
                case 1:
                    printf("[+] Flip Paddle Selected\n");
                    send(mySock, pThreeOptOne, strlen(pThreeOptOne), 0);
                    returnMainMenu(mySock);
                    return 0;
                case 2:
                    printf("[+] Bounce Ball Selected\n");
                    send(mySock, pThreeOptTwo, strlen(pThreeOptTwo), 0);
                    returnMainMenu(mySock);
                    return 0;
                case 3:
                    printf("[+] Main Menu Selected\n");
                    storyMode(mySock);
                    return 0;
                case 4:
                    printf("[+] Disconnect Selected\n");
                    killSock(mySock);
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    send(mySock, pThreeOptions, strlen(pThreeOptions), 0);
                    printf("[+] Sent \"Invalid Choice & pOneOptions\"\n");                    
                    break;
            }
        }
    }
}

int searchOffice(int mySock)
{
    int choice;
    int perception = 0;

    char *perceptionOne = ("\r\n\nYou roll the three-sided dice and the number \"1\" is facing\n"
                            "\rup. Your critical lack of perception focuses on the \n"
                            "\rfirst thing you see - a ping pong table.\n");

    char *perceptionTwo = ("\r\n\nYou roll the three-sided dice and the number \"2\" is \n"
                            "\rfacing up. Your average amount of perception notices a \n"
                            "\rfancy coffee machine in the break room.\n");

    char *perceptionThree = ("\r\n\nYou roll the three-sided dice and the number \"3\" is \n"
                            "\rfacing up. Your exceptional perception notices a Rival \n"
                            "\rNemesis MXVII-10K NERF GUN on the conference table!\n");
    
    srand((unsigned) time(&tme));
    perception = ((rand() % 3) + 1);
    
    switch(perception)
    {
        case 1:
            send(mySock, perceptionOne, strlen(perceptionOne), 0);
            printf("[+] Perception is 1\n");
            perceptOne(mySock);
            break;
        case 2:
            send(mySock, perceptionTwo, strlen(perceptionTwo), 0);
            printf("[+] Perception is 2\n");
            perceptTwo(mySock);
            break;
        case 3:
            send(mySock, perceptionThree, strlen(perceptionThree), 0);
            printf("[+] Perception is 3\n");
            perceptThree(mySock);
            break;
        default:
            printf("[!] Perception is broken %d\n", perception);                
            return 0;
    }
}

int hackBLS(int mySock)
{
    int rndNum;
    char hackAnswer[256];
    char *butHow = ("\r\n\nInteresting Move. How would you do it?\n"
                    "\r[>] ");
    char *randOne = "\r\n\nYou attempt that but the network is too segmented!\n";
    char *randTwo = ("\r\n\nThat method doesn't work here without the use of \n"
                    "\rthe \"--do-forcefully\" flag. Rerun and try again.\n");
    char *randThree = ("\r\n\nSegmentation fault (core dumped). Better luck next time.\n");
    char *randFour = ("\r\n\nStack Trace: undefined result\n"
                        "\r\t at bls-party (function: idontwannacry.cs:14)\n"
                        "\r\t at bls-party (function: youmadcowbro.cs:45)\n"
                        "\r\t at bls-party (function: styleguide-for-400.nist:800-63)\n");
    char *randFive = ("\r\n\nUsername is not in the sudoers file. This incident will be reported.\n");
    uint32_t bytesAvailable = 0;

    send(mySock, butHow, strlen(butHow), 0);

    if (sockTimeout(mySock) == -2)
    {
        return 0;
    }
    recv(mySock, hackAnswer, sizeof(hackAnswer), 0);

    memset(hackAnswer, 0, sizeof(hackAnswer));

    srand((unsigned) time(&tme));
    rndNum = ((rand() % 5) + 1);

    switch(rndNum)
    {
        case 1:
            printf("[+] BLS Hack Option %d\n", rndNum);
            send(mySock, randOne, strlen(randOne), 0);
            break;
        case 2:
            printf("[+] BLS Hack Option %d\n", rndNum);
            send(mySock, randTwo, strlen(randTwo), 0);
            break;
        case 3:
            printf("[+] BLS Hack Option %d\n", rndNum);
            send(mySock, randThree, strlen(randThree), 0);
            break;
        case 4:
            printf("[+] BLS Hack Option %d\n", rndNum);
            send(mySock, randFour, strlen(randFour), 0);
            break;
        case 5:
            printf("[+] BLS Hack Option %d\n", rndNum);
            send(mySock, randFive, strlen(randFive), 0);
            break;
        default:
            printf("[!] BLS Hack Option Broken! (%d)", rndNum);
            break;
    }
    
    returnMainMenu(mySock);
}

int mrGrinch(int mySock)
{
    char *grinch = "\r\n\nYou're a mean one, Mr. Grinch!\n";

    send(mySock, grinch, strlen(grinch), 0);
    returnMainMenu(mySock);
}

int waterPlease(int mySock)
{
    char *barWater = ("\r\n\nThe Bartender grabs a pitcher full of water and pours it \n"
                        "\rover your head; soaking your garments and causing the \n"
                        "\rentire Tavern to roar with laughter.\n");
    int choice;
    
    send(mySock, barWater, strlen(barWater), 0);
    
    printf("[+] Sent \"barWater\"\n");

    returnMainMenu(mySock);

    return 0;
}

int bartenderChoice(int mySock, char *characterName)
{
    char *barReply = ("\r\n\nThe Bartender scratches his head out of disbelief. After all, \n"
                        "\ra mighty Warrior with a frightening name like %s \n"
                        "\rshould obviously know what they want to drink.\n");
    char output[2048];
    int choice;
    
    addNullTerminator(characterName);
    snprintf(output, sizeof(output), barReply, characterName);
    send(mySock, output, strlen(output), 0);
    
    printf("[+] Sent \"barReply\"\n");

    returnMainMenu(mySock);

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
                            "and celebration! Cheers to %s!\n\r");
    
    char playerToast[1024];
    char output[2048];
    int choice;
    uint32_t bytesAvailable = 0;

    send(mySock, proposeToast, strlen(proposeToast), 0);
    printf("[+] Sent \"proposeToast\"\n");

    if (sockTimeout(mySock) == -2)
    {
        return 0;
    }

    recv(mySock, playerToast, sizeof(playerToast), 0);
    
    logToFile(playerToast);

    addNullTerminator(playerToast);
    addNullTerminator(characterName);

    snprintf(output, sizeof(output), toastProposed, characterName, playerToast, playerToast);
    send(mySock, output, strlen(output), 0);

    returnMainMenu(mySock);

    return 0;
}

int playGame(int mySock)
{

    char *enterYourName = ("\r\n\nThe game is a Dungeons and Dragons type text-based game.\n\r"
                            "It wants you to pick a name. \"Champion, what name do you \n\r"
                            "hail by?\"\n\n\r"
                            "[>] ");
    char *drinkOrder = ("\r\n\nHail, %s! You find yourself at a Tavern  \n\r"
                        "ordering yourself a drink. What do you order?\n\r"
                        "[1] Ale, of course!\n\r"
                        "[2] Bartender's Choice\n\r"
                        "[3] Water, please.\n\r"
                        "[4] Return to Main Menu\n\r"
                        "[5] Terminate Connection\n\n\r"
                        "[>] ");
    //char *invalidChoice = "[!] Invalid Choice\n\r";

    char characterName[2048] = { "" };
    char output[2048];
    int choice;
    uint32_t bytesAvailable = 0;

    send(mySock, enterYourName, strlen(enterYourName), 0);
    printf("[+] Sent \"Enter Your Name\"\n");

    if (sockTimeout(mySock) == -2)
    {
        return 0;
    }

    recv(mySock, characterName, sizeof(characterName), 0);

    addNullTerminator(characterName);
    snprintf(output, sizeof(output), drinkOrder, characterName);

    while (TRUE)
    {
        send(mySock, output, strlen(output), 0);    
        printf("[+] Sent \"drinkOrder\"\n");

        while (choice = validateMultipleChoiceInput(mySock))
        {         
            switch(choice)
            {
                case -2:
                    killSock(mySock);
                    return 0;
                case -1:
                    break;
                case 1:
                    printf("[+] Choice 1 Selected\n");
                    orderAle(mySock, characterName);
                    return 0;
                case 2:
                    printf("[+] Choice 2 Selected\n");
                    bartenderChoice(mySock, characterName);
                    return 0;
                case 3:
                    printf("[+] Choice 3 Selected\n");
                    waterPlease(mySock);
                    return 0;
                case 4:
                    printf("[+] Choice 4 Selected\n");
                    storyMode(mySock);
                    return 0;
                case 5:
                    printf("[+] Choice 5 Selected\n");
                    killSock(mySock);
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    send(mySock, output, strlen(output), 0);
                    printf("[+] Sent \"Invalid Choice & drinkOrder\"\n");                    
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
    char *welcome = ("\n\n*** Welcome to the Black Lantern Christmas Party! ***\n"
                    "\r Disclaimer: Connecting with PuTTY is NOT supported! \n\n"
                    "\rYou walk into the DEFCON 34 Village and notice a sea\n\r"
                    "\rof hackers sitting around tables with their laptops.\n\r"
                    "\rYou notice one table by itself with a lone laptop on\n\r"
                    "\rdisplay with a text-based game running.\n\r"
                    "\rWhat do you do?\n\n\r");
    char *welcomeOptions = ("\r[1] Play the game\n\r"
                    "\r[2] Search the office (Roll a 3-sided dice for Perception)\n"
                    "\r[3] Hack Black Lantern Security\n\r"
                    "\r[4] Turn off the Christmas Music\n\r"
                    "\r[5] Terminate Connection\n\n\r"
                    "\r[>] ");
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
                case -2:
                    killSock(mySock);
                    return 0;
                case -1:
                    break;
                case 1:
                    printf("[+] Play the Game Selected\n");
                    playGame(mySock);
                    killSock(mySock);
                    return 0;
                case 2:
                    printf("[+] Search the Office Selected\n");
                    searchOffice(mySock);
                    killSock(mySock);
                    return 0;
                case 3:
                    printf("[+] Hack Black Lantern Security Selected\n");
                    hackBLS(mySock);
                    killSock(mySock);
                    return 0;
                case 4:
                    printf("[+] Turn off the Christmas Music Selected\n");
                    mrGrinch(mySock);
                    killSock(mySock);
                    return 0;
                case 5:                    
                    killSock(mySock);
                    return 0;
                default:
                    send(mySock, invalidChoice, strlen(invalidChoice), 0);
                    send(mySock, welcomeOptions, strlen(welcomeOptions), 0);
                    printf("[+] Sent \"Invalid Choice and Welcome Options\"\n");
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
    uint16_t wVersionRequested;
    uint32_t port = 3724;
    struct sockaddr_in address;
    int server_fd, new_sock, addrlen;
    
    uint32_t opt = 0;
    char *ip;
     
    /* IPv4 Setup */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

#ifdef _WIN32
    /* WSA Startup */
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        perror("[-] WSAStartup failure\n");
        exit(EXIT_FAILURE);
    }
#endif
    
    /* Initiate Socket */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror("[-] Socket failure");
        exit(EXIT_FAILURE);
    }

    /* Set Socket Options */
    if (setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, (char *) &opt, sizeof(int)) == -1)
    {
        perror("[-] Set Socket Opt failure");
        exit(EXIT_FAILURE);
    }

    /* Bind Socket to local IP Address and Port */
    if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) == -1)
    {
        perror("[-] Bind Socket failure");
        exit(EXIT_FAILURE);
    }

    /* Start Listener */
    if (listen(server_fd, 3) == -1)
    {
        perror("[-] Listen Socket failure");
        exit(EXIT_FAILURE);
    }
    
    while (TRUE)
    {
        /* Accept Connection */
        printf("[!] WARNING: Connecting with PuTTY is NOT SUPPORTED (seriously)\n");
        printf("[+] You can connect to the server using \"ncat localhost 3724\"\n");        
        printf("[+] Listening for Connection...\n");
        addrlen = sizeof(address);
        if ((new_sock = accept(server_fd, (struct sockaddr*) &address, &addrlen)) == -1)
        {
            perror("[-] Accept Socket failure");
            exit(EXIT_FAILURE);
        }

        sockHandle = new_sock;
        storyMode(new_sock);
        
        
#ifdef _WIN32
        /* Cleanup */
        shutdown(new_sock, SD_BOTH);
        closesocket(new_sock);
    }

    shutdown(server_fd, SD_BOTH);
    closesocket(server_fd);
    WSACleanup();
#else
        /* Cleanup */
        shutdown(new_sock, SHUT_RDWR);
        close(new_sock);
    }

    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);
#endif

    return 0;
}