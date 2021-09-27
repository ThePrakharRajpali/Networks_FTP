CS342 Assignment 3
Group: 40 
Students:  
    Aryan Rathod - 190101014
    Prakhar Rajpali - 190101070 
    Shaurya Sinha - 191023058
    Ayush Sanwal - 190123016 

File Structure:
Client Folder
    client.c
    files

Server Folder
    server.c

Execution of code:
1) Compilation: Run 2 instances of the terminal and execute server.c in one and client.c in another. Run: "gcc server.c -o server" & "gcc client.c -o client" on other.
2) Execution: Run: "./server <port>" first in server terminal then "./client <ip address(server)> <server port>" in client terminal.

Syntax: (.txt & .c files)
1) PUT: PUT <file>
2) GET: GET <file>
3) MGET: MGET <extension> //don't include dot '.'
4) MPUT: MPUT <extension> //don't include dot '.'
5) quit: quit



