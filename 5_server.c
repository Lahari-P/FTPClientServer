#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

#define PORT 8081
#define HOST 127.0.0.1
char *readFromFile(char *filePath)
{
    int fd1, rc;
    static char buf[1024];

    fd1 = open(filePath, O_RDONLY);

    if ((fd1 == -1) && (errno == ENOENT))
        perror("open");
    else
    {
        rc = read(fd1, buf, sizeof(buf));
        printf("The contents of file: %s\n%s", filePath, buf);
    }
    close(fd1);
    return buf;
}
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

   
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             &addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    /* Read from the Client */

    /* Cleaning the buffer contents */
    memset(buffer, 0, sizeof(buffer));

    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    char *fileContents = readFromFile(buffer);
    send(new_socket, fileContents, strlen(fileContents), 0);

    return 0;
}