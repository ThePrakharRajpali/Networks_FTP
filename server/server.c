#include "header/function.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./server <port>\n");
        exit(1);
    }
    int serv = start_server(argv[1]);

    struct sockaddr_in clientAddress;
    socklen_t clientLen;

    clientLen = sizeof(clientAddress);

    int clientfd = accept(serv, (struct sockaddr *)&clientAddress, &clientLen);

    if (clientfd < 0)
    {
        printf("Error connecting.\n");
        exit(1);
    }

    printf("CONNECTED.\n");

    char buffer[1000];
    char *args[10];
    while (1)
    {
        bzero(buffer, 1000);
        read(clientfd, buffer, 1000);
        printf("%s\n", buffer);
        if (strcmp("GET", buffer) == 0)
        {
            write(clientfd, "OK", 2);
            bzero(buffer, 1000);
            read(clientfd, buffer, 1000);
            PUT(clientfd, buffer);
        }
        else if (strcmp("PUT", buffer) == 0)
        {
            write(clientfd, "OK", 2);
            bzero(buffer, 1000);
            read(clientfd, buffer, 1000);
            GET(clientfd, buffer);
        }
        else if (strcmp("MPUT", buffer) == 0)
        {
            write(clientfd, "OK", 2);
            bzero(buffer, 1000);
            read(clientfd, buffer, 1000);
            MGET(clientfd, buffer);
        }
        else if (strcmp("MGET", buffer) == 0)
        {
            write(clientfd, "OK", 2);
            bzero(buffer, 1000);
            read(clientfd, buffer, 1000);
            MPUT(clientfd, buffer);
        }
        else
        {
            write(clientfd, "ERROR", 5);
            continue;
        }
    }
    return 0;
}
