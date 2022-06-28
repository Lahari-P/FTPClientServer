#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

#define PORT 8081
#define HOST "127.0.0.1"

void writeToFile(char *buffer)
{
    int filePointer = open("clientCopied.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    write(filePointer,buffer,strlen(buffer));
    printf("The file is copied from server to client.");
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(HOST);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
        printf("Enter the file name to be searched in server: ");
        scanf("%s", buffer);
        send(sock, buffer, strlen(buffer), 0);

        /* Read from the server */

        /* Cleaning the buffer contents */
        memset(buffer, 0, sizeof(buffer));

        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
        writeToFile(buffer);
  

    return 0;
}