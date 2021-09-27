#include "header.h"

int break_line(char *str, char *words[])
{
    str = strtok(str, "\n");

    int k = 0;
    char *ptr = strtok(str, " ");

    while (ptr != NULL)
    {
        words[k++] = ptr;
        ptr = strtok(NULL, " ");
    }

    return k;
}

int connect_server(char *ip, char *port)
{
    printf("\n");
    int portno = atoi(port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        printf("Error opening socket.");
        exit(1);
    }

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(portno);

    if (inet_pton(AF_INET, ip, &servAddr.sin_addr) < 0)
    {
        printf("Invalid Address.");
        exit(1);
    }

    int sockconn = connect(sockfd, (struct sockaddr *)&servAddr,
                           sizeof(servAddr));

    if (sockconn < 0)
    {
        printf("Error connecting to server.");
        exit(1);
    }
    printf("CONNECTED TO %s:%s.\n", ip, port);
    printf("\n");
    return sockfd;
}

int PUT(int sockfd, char *filename)
{
    printf("Sending %s. \n", filename);
    char filepath[256];
    bzero(filepath, 256);
    strcpy(filepath, STORE);
    strcat(filepath, filename);

    FILE *fd;
    fd = fopen(filepath, "r");

    char buffer[1024];

    read(sockfd, buffer, 1024);

    if (fd == NULL)
    {
        write(sockfd, "ABORT", 5);
        read(sockfd, buffer, 1024);
        printf("File %s does not exist.\n", filepath);
        printf("\n");
        return -1;
    }

    write(sockfd, "OK", 2);

    bzero(buffer, 1024);
    read(sockfd, buffer, 1024);

    if (strcmp(buffer, "EXIST") == 0)
    {
        bzero(buffer, 1024);
        sprintf(buffer, "%s already exists on the server. Do you want to overwrite? (Y / N)", filename);
        printf("%s \n", buffer);

        bzero(buffer, 1024);
        fgets(buffer, 1024, stdin);
        if (buffer[0] != 'Y')
        {
            write(sockfd, "ABORT", 5);
            read(sockfd, buffer, 1024);
            printf("Aborting.\n");
            printf("\n");
            fclose(fd);
            return -1;
        }
        else
        {
            write(sockfd, "OK", 2);
            bzero(buffer, 1024);
            read(sockfd, buffer, 1024);
        }
    }

    if (strcmp(buffer, "OK") != 0)
    {
        printf("Error creating file on server.\n");
        printf("\n");
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
        goto end;
    }
    write(sockfd, "OK", 2);

    read(sockfd, buffer, 1024);
    do
    {
        bzero(buffer, 1024);
        size = fread(buffer, sizeof(char), 1023, fd);
        send(sockfd, buffer, size, 0);
    } while (size == 1023);

end:
    read(sockfd, buffer, 1024);

    fclose(fd);

    bzero(buffer, 1024);
    sprintf(buffer, "Successfully sent file %s.", filename);
    printf("%s\n", buffer);
    printf("\n");

    return 0;
}

int GET(int sockfd, char *filename)
{
    char buffer[1024];
    bzero(buffer, 1024);
    read(sockfd, buffer, 1024);
    if (strcmp("OK", buffer) != 0)
    {
        printf("%s\n", buffer);
        printf("\n");
        return -1;
    }

    char filepath[256];
    bzero(filepath, 256);
    strcpy(filepath, STORE);
    strcat(filepath, filename);
    FILE *fd = fopen(filepath, "r");

    if (fd != NULL)
    {
        sprintf(buffer, "%s already exists. Do you want to overwrite? (Y / N)", filename);
        printf("%s\n", buffer);
        fgets(buffer, 1024, stdin);
        if (buffer[0] != 'Y')
        {
            write(sockfd, "ABORT", 5);
            read(sockfd, buffer, 1024);
            printf("Aborting.\n");
            printf("\n");
            return -1;
        }
    }

    fd = fopen(filepath, "w");

    if (fd == NULL)
    {
        write(sockfd, "ABORT", 5);
        read(sockfd, buffer, 1024);
        printf("Error creating file.\n");
        printf("\n");
        return -1;
    }

    write(sockfd, "OK", 1024);

    bzero(buffer, 1024);
    read(sockfd, buffer, 1024);
    if (strcmp(buffer, "EMPTY") == 0)
    {
        write(sockfd, "OK", 2);
        goto end;
    }
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
end:
    fclose(fd);
    bzero(buffer, 1024);

    sprintf(buffer, "Successfully recieved file %s.", filename);
    printf("%s\n", buffer);
    printf("\n");

    return 0;
}

int list(int sockfd)
{
    char buffer[256];
    while (1)
    {
        bzero(buffer, 256);
        read(sockfd, buffer, 256);

        if (strcmp(buffer, "DONE") == 0)
            break;

        printf("%s\n", buffer);

        write(sockfd, "OK", 2);
    }
    printf("\n");
    write(sockfd, "OK", 2);
    bzero(buffer, 256);
    read(sockfd, buffer, 256);
    printf("%s\n", buffer);
    printf("\n");
    return 0;
}

int MPUT(int sockfd, char *extension)
{
    printf("Sending files with %s extension.\n", extension);
    printf("\n");
    char buffer[256];
    read(sockfd, buffer, 256);
    DIR *di;
    struct dirent *dir;
    di = opendir(STORE);
    char *filename;
    char *ext;
    int count = 0;
    int res;

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
            res = MPUT(sockfd, filename);
            if (res >= 0)
                count++;
        }
    }
    write(sockfd, "DONE", 4);
    read(sockfd, buffer, 256);
    printf("--%d file(s) sent.\n", count);
    printf("\n");
    return 1;
}

int MGET(int sockfd, char *extension)
{
    printf("Fetching files with %s extension.\n", extension);
    printf("\n");
    char buffer[256];
    int count = 0, res;
    while (1)
    {
        bzero(buffer, 256);
        read(sockfd, buffer, 256);
        if (strcmp(buffer, "DONE") == 0)
            break;
        write(sockfd, "OK", 2);
        res = GET(sockfd, buffer);
        write(sockfd, "OK", 2);
        if (res >= 0)
            count++;
    }
    printf("--Recieved %d file(s).\n", count);
    printf("\n");
    return 0;
}