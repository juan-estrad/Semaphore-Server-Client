#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/queue.h>
#include <semaphore.h>
#define SIZE 100

void systemCall1(int id, int n, int size, char str[], int fdID[]);
void systemCall2(int id, int n, int size, char str[], int fdID[]);
void systemCall3(int id, int n, int size, char str[], int fdID[]);
void systemCall4(int id, int n, int size, char str[], int fdID[]);
void systemCall5(int id, int n, int size, char str[], int fdID[]);
void concatenate_string(char* s, char* s1);

sem_t mutex;

int main(int argc, char* argv[]){
    if(mkfifo("serverfifo", 0777) == -1){
        if(errno != EEXIST){
            printf("Could not create fifo file\n");
            return 1;
        }
    }

    char str1[80];
    int fdID[50];
    char q[5][50];
    int rear = -1;
    int front = -1;

    printf("Opening own read fifo...\n");
    int fd = open("serverfifo", O_RDWR);
    printf("Opened\n");

    sem_init(&mutex, 0, 1);

    int open = 1;
    while(open > 0){
        printf("Waiting for request...\n");
        if(read(fd, &str1, 80) == -1){
            return 2;
        }
        int clientID;
        int systemCall;
        int n;
        int size;
        char str[50];

        char *token = strtok(str1, " ");

        int count = 1;
        int writeInt;
        while (token != NULL)
        {
            if(count == 1){
                clientID = atoi(token);
            }
            else if(count == 2){
                systemCall = atoi(token);
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

        printf("System call from Client %d\n", clientID);
        printf("System call Requested: %d with %d parameters which are:\n", systemCall, n);
        printf(" Param1: %d  Param2: %s\n", size, str);
        if(systemCall == 1){
            systemCall1(clientID,n,size,str,fdID); 
        }
        if(systemCall == 2){
            systemCall2(clientID,n,size, str, fdID); 
        }
        if(systemCall == 3){
            systemCall3(clientID,n,size, str, fdID); 
        }
        if(systemCall == 4){
            systemCall4(clientID, n, size, str, fdID);
        }
        if(systemCall == 5){
            systemCall5(clientID, n, size, str, fdID);
        }
        if(systemCall == 6){
            if(front== - 1)            
                front = 0;
            rear = rear + 1;
            strcpy(q[rear], str);
            printf("Added '%s' to queue. \n", str);
        }
        if(systemCall == 7){
            char message[20];
            if(front == - 1 || front > rear){
                printf("Queue Underflow \n");
            }
            else{
                //printf("Element deleted from queue is : %dn", queue_array[front]);
                printf("Element deleted from queue is : %s\n", q[front]);
                strcpy(message, q[front]);
                write(fdID[clientID], &message, sizeof(message));
                front = front + 1;
            }
        }

        if(systemCall == 0){
            writeInt = -1;
            write(fdID[clientID], &writeInt, 4);
            close(fdID[clientID]);
        }
        if(systemCall == -1){
            writeInt = -1;
            write(fdID[clientID], &writeInt, 4);
            close(fdID[clientID]);
            open = -1;
        }
    }
    printf("Closing Server...\n");
    close(fd);
    printf("Closed\n");
    
    return 0;
}

void systemCall1(int id, int n, int size, char str[], int fdID[]){
    printf("Opening client %d fifo....\n", id);
    fdID[id] = open(str, O_WRONLY);
    printf("Opened client %d fifo...\n", id);
    return;
}

void systemCall2(int id, int n, int size, char str[], int fdID[]){
    int i, count = 0;
    char strOut[] = "Number of Vowels: ";
    char vowels[8];
    char testVowels[100];

    for(i = 0; str[i] != '\0'; i++)
    {
        if( str[i] == 'a' ||
            str[i] == 'e' ||
            str[i] == 'i' || 
            str[i] == 'o' || 
            str[i] == 'u'    )
        {
            count++;
        }
    }


    printf("Sending vowels...\n");
    write(fdID[id], &count, sizeof(count));
    return;
}

void systemCall3(int id, int n, int size, char str[], int fdID[]){
    int i, count = 0, nonVowel;
    char nonV[8];
    char returnStr[] = "Number of non-vowels: ";

    for(i = 0; str[i] != '\0'; i++)
    {
        if( str[i] == 'a' ||
            str[i] == 'e' ||
            str[i] == 'i' || 
            str[i] == 'o' || 
            str[i] == 'u'    )
        {
            count++;
        }
    }
    
    nonVowel = size - count;
    printf("Sending non-vowels...\n");
    write(fdID[id], &nonVowel, sizeof(nonVowel));
    return;
}

void systemCall4(int id, int n, int size, char str[], int fdID[]){
    int status = 0;
    write(fdID[id], &status, sizeof(status));
    printf("Client %d waiting for semaphore...\n", id);
    sem_wait(&mutex);
    printf("Client %d locked semaphore\n\n", id);
    status = 1;
    write(fdID[id], &status, sizeof(status));
    return;
}

void systemCall5(int id, int n, int size, char str[], int fdID[]){
    int status;
    printf("Unlocking semaphore from client %d\n", id);
    sem_post(&mutex);
    status = 3;
    printf("Unlocked semaphore\n");
    write(fdID[id], &status, sizeof(status));
    return;
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
