all:
	g++ -std=c++11 -lsocket main.cpp -o server utils/socket.cpp