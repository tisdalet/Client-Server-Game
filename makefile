all:  client server

client: tcpcli01.c
	cc -o client tcpcli01.c
server: tcpserv01.c
	cc -o server tcpserv01.c
clean:
	rm server client
