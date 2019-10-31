all: client server server2

client: client.cpp
	g++ -std=c++11 md5/md5.cpp client.cpp -o client

server: server.cpp
	g++ -std=c++11 server.cpp -o server
	
server2:server2.cpp
	g++ -std=c++11 md5/md5.cpp server2.cpp -o server2

clean:
	rm client
	rm server
	rm server2
