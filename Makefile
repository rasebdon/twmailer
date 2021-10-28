# Client files
client: clientFiles/client.cpp
	g++ -o twmailer-client clientFiles/client.cpp

# Server files
server: messageHandler serverClient main serverFiles/server.cpp messageHandler.o serverClient.o main.o
	g++ -o twmailer-server serverFiles/server.cpp messageHandler.o serverClient.o main.o

main: serverFiles/main.cpp
	g++ -c -Wall serverFiles/main.cpp

serverClient: serverFiles/serverClient.cpp
	g++ -c -Wall serverFiles/serverClient.cpp

messageHandler: serverFiles/messageHandler.cpp
	g++ -c -Wall serverFiles/messageHandler.cpp

# Cleans
clean:
	find . -type f -name '*.h.gch' -delete -o -name '*.o' -delete
