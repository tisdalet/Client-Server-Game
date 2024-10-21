all:  client server

client: tcpcli01.c str_cli.c
	cc -o client tcpcli01.c str_cli.c
server: tcpserv01.c str_echo.c
	cc -o server tcpserv01.c str_echo.c
clean:
	rm server client
