#include "header/header.h"
#include "header/function.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./server <port>\n");
        exit(1);
    }
    int serv = start_server(argv[1]);

    struct sockaddr_in cliAddr;
    socklen_t clilen;

    clilen = sizeof(cliAddr);

    int clifd = accept(serv, (struct sockaddr *)&cliAddr, &clilen);

    if (clifd < 0)
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
        read(clifd, buffer, 1000);
        printf("%s\n", buffer);
        if (strcmp("GET", buffer) == 0)
        {
            write(clifd, "OK", 2);
            bzero(buffer, 1000);
            read(clifd, buffer, 1000);
            PUT(clifd, buffer);
        }
        else if (strcmp("PUT", buffer) == 0)
        {
            write(clifd, "OK", 2);
            bzero(buffer, 1000);
            read(clifd, buffer, 1000);
            GET(clifd, buffer);
        }
        else if (strcmp("MPUT", buffer) == 0)
        {
            write(clifd, "OK", 2);
            bzero(buffer, 1000);
            read(clifd, buffer, 1000);
            MGET(clifd, buffer);
        }
        else if (strcmp("MGET", buffer) == 0)
        {
            write(clifd, "OK", 2);
            bzero(buffer, 1000);
            read(clifd, buffer, 1000);
            MPUT(clifd, buffer);
        }
        else if (strcmp("list", buffer) == 0)
        {
            list(clifd);
        }
        else
        {
            write(clifd, "ERROR", 5);
            continue;
        }
    }
    return 0;
}
