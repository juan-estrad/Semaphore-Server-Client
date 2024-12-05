#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

void concatenate_string(char* s, char* s1);
void wait(int clientID, int writefd, int readfd);
void signal(int clientID, int writefd, int readfd);
char* copyString(char s[]);

int main(int argc, char* argv[]){
    char fifoID [40] = {'m', 'y', 'f','i','f','o', '\0'};
    char numID[20];
    printf("Process ID: \n");
    fgets(numID, 20, stdin);
    concatenate_string(fifoID, numID);

    char fifoTest[40];
    printf("Fifo name: \n");
    fgets(fifoTest, 40, stdin);

     if(mkfifo(fifoTest, 0777) == -1){
        if(errno != EEXIST){
            printf("Could not create fifo file\n");
            return 1;
        }
    }

    char str1[80];
    char readStr[80];
    printf("Opening Write...\n");
    int writefd = open("serverfifo", O_WRONLY);
    printf("Opened Write\n");

    char initialCall[80];
    printf("\n===============================================================================================================\n");
    printf("1. Initial system call\n");
    printf("0. Exit system call\n");
    printf("-1. Terminate\n");
    printf("Give each parameter followed by a space: Process ID, System Call Number, Number of Parameters, Size, Value:\n");
    fgets(initialCall, 40, stdin);
    write(writefd, initialCall, 50);

    printf("\nOpening Read...\n");
    int readfd = open(fifoTest, O_RDONLY);
    printf("Opened Read...\n");

    int loop = 1;
    while(loop == 1){
        char systemCall[80];
        printf("\n===============================================================================================================\n");
        printf("1. Initial system call\n");
        printf("2. Number of vowels\n");
        printf("3. Number of non-vowels\n");
        printf("4. Lock semaphore\n");
        printf("5. Unlock semaphore\n");
        printf("6. Message Send\n");
        printf("7. Message Recieve\n");
        printf("0. Exit system call\n");
        printf("-1. Terminate\n");
        printf("Give each parameter followed by a space: Process ID, System Call Number, Number of Parameters, Size, Value:\n");
        fgets(systemCall, 40, stdin);

        char* sysCall;
        sysCall = copyString(systemCall);

        int clientID;
        int callNum;
        int n;
        int size;
        char str[50];
        int count = 1;
        char *token = strtok(sysCall, " ");

        while (token != NULL)
        {
            if(count == 1){
                clientID = atoi(token);
            }
            else if(count == 2){
                callNum = atoi(token);
            }
            else if(count == 3){
                n = atoi(token);
            }
            else if(count == 4){
                size = atoi(token);
            }
            else if(count == 5){
                strcpy(str, token);
            }
            count++;
            token = strtok(NULL, " ");
        }

        if(callNum == 4){
            for(int x = 0; x<=10; x++){
                printf("NON-CRITICAL SECTION...\n");
                sleep(1);
            }
            wait(clientID, writefd, readfd);

            for(int x = 0; x<=20; x++){
                printf("INSIDE CRITICAL SECTION\n");
                sleep(1);
            }
        }
        else if(callNum == 5){
            printf("LEAVING THE CRITICAL SECTION\n");
            signal(clientID, writefd, readfd);
            printf("DONE.\n");
        }
        //New location for sending
        else{
            printf("\nSending System Call...\n");
            if(write(writefd, systemCall, 50) == -1){
                return 2;
            }
            printf("Sent System Call...\n");
        }
        
        int test;
        if(callNum == 2 || callNum == 3){
            if (callNum == 2){
                read(readfd, &test, sizeof(test));
                printf("Number of vowels: %d\n", test);
            }
            if (callNum == 3){
                read(readfd, &test, sizeof(test));
                printf("Number of non-vowels: %d\n", test);
            }
        }

        char message[20];
        if(callNum == 6){
            printf("Message Sent.\n");
        } 
        if( callNum ==7){
            read(readfd, &message, sizeof(message));
            printf("Message: %s\n", message);
        }
        if(callNum < 1){
            loop = 2;
        }
    }

    printf("Closing...\n");
    close (writefd);
    close(readfd);
    printf("Closed.\n");
}

void concatenate_string(char* s, char* s1){
    int i; 
    int j = strlen(s);

    for(i = 0; s1[i] != '\0'; i++){
        s[i+j] = s1[i];
    }
    s[i+j] = '\0';
    return;
}

void wait(int clientID, int writefd, int readfd){
    char systemCall[80]; 
    char request[20] = {' ', '4', ' ', '0', ' ', '0', ' ', '0'};
    sprintf(systemCall, "%d", clientID);
    concatenate_string(systemCall, request);

    printf("SENDING WAIT REQUEST...\n");
    write(writefd, systemCall, 50);
    printf("Sent WAIT System Call...\n");

    printf("Waiting TO GET INTO THE CRITICAL SECTION...\n");
    int status = 0;
    while(status == 0){
        read(readfd, &status, sizeof(status));
        sleep(2);
    }
    return;
}

void signal(int clientID, int writefd, int readfd){
    char systemCall[80]; 
    char request[20] = {' ', '5', ' ', '0', ' ', '0', ' ', '0'};
    int status = 0;
    sprintf(systemCall, "%d", clientID);
    concatenate_string(systemCall, request);

    printf("SENDING SIGNAL REQUEST...\n");
    write(writefd, systemCall, 50);
    printf("Sent SIGNAL System Call...\n");
    while(status != 3){
        read(readfd, &status, sizeof(status));
    }
    return;
}

char* copyString(char s[])
{
    char* s2;
    s2 = (char*)malloc(20);
 
    strcpy(s2, s);
    return (char*)s2;
}