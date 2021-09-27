CS342 Assignment 3
Group: 40 
Students:  
    Aryan Rathod - 190101014
    Prakhar Rajpali - 190101070 
    Shaurya Sinha - 191023058
    Ayush Sanwal - 190123016 

File Structure:
Client Folder
    store (Files to be sent/recieved)
    header
        function.h
        header.h 
    client.c

Server Folder
    store (Files to be sent/recieved)
    header
        function.h
        header.h
    server.c

Execution of code:
1) Compilation: Run 2 instances of the terminal and execute server.c in one and client.c in another. Run: "gcc server.c -o server.o" & "gcc client.c -o client.o" on other.
2) Execution: Run: "./server.o <port>" first in server terminal then "./client.o <ip address(server)> <server port>" in client terminal.

Terminal 1:
$ cd ./server 
$ gcc server.c -o server.o
$ ./server.o <PORT>

Terminal 2:
$ cd ./client 
$ cd ./store
$ touch <TEST FILES>
$ cd ..
$ gcc client.c -o client.o
$ ./client.o <IP Address of Server> <PORT>

Syntax: 
1) PUT: PUT <file>
2) GET: GET <file>
3) MGET: MGET <extension> //don't include dot '.'
4) MPUT: MPUT <extension> //don't include dot '.'
