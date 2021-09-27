#include "header.h"

int start_server(char *port)
{
    int portno = atoi(port);

    int servfd = socket(AF_INET, SOCK_STREAM, 0);

    if (servfd < 0)
    {
        printf("Error opening socket.\n");
        exit(1);
    }

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(portno);

    if (bind(servfd, (struct sockaddr *)&servAddr, sizeof(servAddr)))
    {
        printf("Bind failed. Possibly invalid Port Number.\n");
        exit(1);
    }

    listen(servfd, 5);

    printf("SERVER UP AND LISTENING.");

    return servfd;
}

int PUT(int sockfd, char *filename)
{
    char filepath[256];
    bzero(filepath, 256);
    strcpy(filepath, STORE);
    strcat(filepath, filename);

    FILE *fd = fopen(filepath, "r");

    char buffer[1024];

    if (fd == NULL)
    {
        bzero(buffer, 1024);
        sprintf(buffer, "%s not found on server.", filename);
        write(sockfd, buffer, strlen(buffer));
        return -1;
    }
    write(sockfd, "OK", 2);

    bzero(buffer, 1024);
    read(sockfd, buffer, 1024);
    if (strcmp("OK", buffer) != 0)
    {
        write(sockfd, "ABORT", 5);
        fclose(fd);
        return -1;
    }

    int size;

    fseek(fd, 0L, SEEK_END);
    size = ftell(fd);

    fseek(fd, 0L, SEEK_SET);
    if (size == 0)
    {
        write(sockfd, "EMPTY", 5);
        read(sockfd, buffer, 1024);
        goto end;
    }
    write(sockfd, "OK", 5);
    read(sockfd, buffer, 1024);

    do
    {
        bzero(buffer, 1024);
        size = fread(buffer, sizeof(char), 1023, fd);
        send(sockfd, buffer, size, 0);
    } while (size == 1023);

end:
    fclose(fd);

    return 0;
}

int GET(int sockfd, char *filename)
{
    char buffer[1024];
    bzero(buffer, 1024);
    write(sockfd, "OK", 2);
    read(sockfd, buffer, 1024);
    if (strcmp(buffer, "OK") != 0)
    {
        write(sockfd, "ABORT", 5);
        return -1;
    }
    char filepath[256];
    bzero(filepath, 256);
    strcpy(filepath, STORE);
    strcat(filepath, filename);

    FILE *fd = fopen(filepath, "r");

    if (fd != NULL)
    {
        write(sockfd, "EXIST", 5);

        bzero(buffer, 1024);
        read(sockfd, buffer, 1024);

        if (strcmp(buffer, "ABORT") == 0)
        {
            write(sockfd, "ABORT", 5);
            fclose(fd);
            return -1;
        }
    }

    fd = fopen(filepath, "w");

    if (fd == NULL)
    {
        write(sockfd, "ABORT", 5);
        return -1;
    }

    write(sockfd, "OK", 2);

    bzero(buffer, 1024);
    read(sockfd, buffer, 1024);
    if (strcmp("EMPTY", buffer) != 0)
    {
        write(sockfd, "OK", 2);
        while (1)
        {
            bzero(buffer, 1023);
            recv(sockfd, buffer, 1023, 0);
            buffer[1023] = '\0';
            fwrite(buffer, sizeof(char), strlen(buffer), fd);
            if (strlen(buffer) < 1023)
                break;
        }
    }

    fclose(fd);

    write(sockfd, "OK", 2);

    return 0;
}

int list(int sockfd)
{
    int count = 0;
    DIR *di;
    struct dirent *dir;
    di = opendir(STORE);
    char buffer[32];
    while ((dir = readdir(di)) != NULL)
    {
        write(sockfd, dir->d_name, strlen(dir->d_name));
        bzero(buffer, 32);
        read(sockfd, buffer, 32);
        if (strcmp("OK", buffer) != 0)
        {
            printf("ERROR");
            break;
        }
        count++;
    }
    write(sockfd, "DONE", 4);
    bzero(buffer, 32);
    read(sockfd, buffer, 32);
    bzero(buffer, 32);
    sprintf(buffer, "%d file(s).", count);
    write(sockfd, buffer, strlen(buffer));
    return 0;
}

int MGET(int sockfd, char *extension)
{
    write(sockfd, "OK", 2);
    char buffer[256];
    while (1)
    {
        bzero(buffer, 256);
        read(sockfd, buffer, 256);
        if (strcmp(buffer, "DONE") == 0)
            break;
        else
            GET(sockfd, buffer);
    }
    write(sockfd, "OK", 2);
    return 0;
}

int MPUT(int sockfd, char *extension)
{
    char buffer[256];

    DIR *di;
    struct dirent *dir;
    di = opendir(STORE);
    char *filename;
    char *ext;

    while ((dir = readdir(di)) != NULL)
    {
        filename = dir->d_name;
        if (filename == NULL)
            continue;
        ext = strtok(filename, ".");
        if (ext == NULL)
            continue;
        ext = strtok(NULL, ".");
        if (ext != NULL && strcmp(ext, extension) == 0)
        {
            strcat(filename, ".");
            strcat(filename, extension);

            write(sockfd, filename, strlen(filename));
            read(sockfd, buffer, 256);
            PUT(sockfd, filename);
            read(sockfd, buffer, 256);
        }
    }

    write(sockfd, "DONE", 4);

    return 1;
}
