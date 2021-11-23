all: server client

server:
		g++ -std=c++11 -pthreads -lsocket server.cpp -o server utils/Lib-sockets.h utils/Lib-servers.h utils/Lib-threads.h

client:
		g++ -std=c++11 -pthreads -lsocket client.cpp -o client utils/Lib-sockets.h utils/Lib-clients.h utils/Lib-threads.h