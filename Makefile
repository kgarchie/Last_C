all: client server

client: client.o helper.o myqueue.o
	gcc -pthread -o client client.o helper.o myqueue.o

server: server.o helper.o myqueue.o
	gcc -pthread -o server server.o helper.o myqueue.o

client.o: client.c
	gcc -c client.c

server.o: server.c
	gcc -c server.c

helper.o: helper.c
	gcc -c helper.c

myqueue.o: myqueue.c
	gcc -c myqueue.c

clean:
	rm *.o client server
