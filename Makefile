all:
	g++ -std=c++11 -lsocket main.cpp -o server utils/Lib-sockets.h