all: client.exe server.exe
client.exe:client.cpp
	g++ -g -o $@ -lpthread $<
server.exe:server.cpp server/chat_server.cpp  server/server_socket.cpp
	g++ -g -Iserver -Icommon/ -o $@ $^
clean:
	rm -f client.exe server.exe
