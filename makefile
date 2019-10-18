all: client server

client: client.cpp
	g++ -std=c++11 client.cpp -o client

server: server.cpp
	g++ -std=c++11 server.cpp -o server

clean:
	rm client
	rm server
