all:
	g++ -std=c++11 -pthreads -lsocket main.cpp -o server utils/Lib-sockets.h utils/Lib-servers.h utils/Lib-threads.h