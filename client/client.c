#include "header/function.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./client <server IP> <port>");
        exit(1);
    }

    int sockfd;
    sockfd = connect_server(argv[1], argv[2]);

    char command[1000], buffer[1000];
    char *args[10];

    while (true)
    {
        bzero(command, 1000);
        bzero(buffer, 1000);

        printf("$ ");

        fgets(command, 1000, stdin);
        printf("\n");

        int cargs = break_line(command, args);
        if (cargs == 0)
        {
            printf("$ ");
            continue;
        }

        if (strcmp("GET", args[0]) == 0)
        {
            if (cargs != 2)
            {
                printf("Invalid Syntax.\n");
            }
            else
            {
                write(sockfd, args[0], strlen(args[0]));

                bzero(buffer, 1000);
                read(sockfd, buffer, 1000);
                if (strcmp("OK", buffer))
                {
                    printf("ERROR\n");
                    continue;
                }
                write(sockfd, args[1], strlen(args[1]));

                GET(sockfd, args[1]);
            }
        }
        else if (strcmp("PUT", args[0]) == 0)
        {
            if (cargs != 2)
            {
                printf("Invalid Syntax.\n");
            }
            else
            {
                write(sockfd, args[0], strlen(args[0]));

                bzero(buffer, 1000);
                read(sockfd, buffer, 1000);
                if (strcmp("OK", buffer))
                {
                    printf("ERROR\n");
                    continue;
                }
                write(sockfd, args[1], strlen(args[1]));

                PUT(sockfd, args[1]);
            }
        }
        else if (strcmp("MPUT", args[0]) == 0)
        {
            if (cargs != 2)
            {
                printf("Invalid Syntax.\n");
            }
            else
            {
                write(sockfd, args[0], strlen(args[0]));

                bzero(buffer, 1000);
                read(sockfd, buffer, 1000);
                if (strcmp("OK", buffer))
                {
                    printf("ERROR\n");
                    continue;
                }
                write(sockfd, args[1], strlen(args[1]));

                MPUT(sockfd, args[1]);
            }
        }
        else if (strcmp("MGET", args[0]) == 0)
        {
            if (cargs != 2)
            {
                printf("Invalid Syntax.\n");
            }
            else
            {
                write(sockfd, args[0], strlen(args[0]));

                bzero(buffer, 1000);
                read(sockfd, buffer, 1000);
                if (strcmp("OK", buffer))
                {
                    printf("ERROR\n");
                    continue;
                }
                write(sockfd, args[1], strlen(args[1]));

                MGET(sockfd, args[1]);
            }
        }
        else if (strcmp("exit", args[0]) == 0)
        {
            if (cargs != 1)
            {
                printf("Invalid Syntax.\n");
            }
            printf("\n");
            break;
        }
        else
        {
            printf("Invalid Command.\n");
            printf("\n");
        }
    }
    close(sockfd);
    return 0;
}
