default: 
server:
	gcc ./server/server.c -o server.o && ./server.o

client: 
	gcc ./client/client.c -o client.o && ./client.o

clean:
	rm *.o