# Client files
client: clientFiles/client.cpp
	g++ -o twmailer-client clientFiles/client.cpp

# Server files
server: storageManager messageHandler serverClient serverFiles/server.cpp storageManager.o messageHandler.o serverClient.o
	g++ -o twmailer-server serverFiles/server.cpp storageManager.o messageHandler.o serverClient.o

serverClient: serverFiles/serverClient.cpp
	g++ -c -Wall serverFiles/serverClient.cpp

messageHandler: serverFiles/messageHandler.cpp
	g++ -c -Wall serverFiles/messageHandler.cpp

storageManager: serverFiles/storageManager.cpp
	g++ -c -Wall serverFiles/storageManager.cpp

# Cleans
clean:
	find . -type f -name '*.h.gch' -delete -o -name '*.o' -delete
