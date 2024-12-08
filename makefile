all:  client server

client: tcpcli01.c
	gcc -o client tcpcli01.c -lsqlite3
server: tcpserv01.c
	cc -o server tcpserv01.c
clean:
	rm server client
