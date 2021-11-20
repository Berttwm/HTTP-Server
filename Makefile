all:
	g++ -std=c++11 -lsocket main.cpp -o server utils/Socket.cpp utils/BindingSocket.cpp