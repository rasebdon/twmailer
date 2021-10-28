# Client files
client: clientFiles/client.cpp
	g++ -o twmailer-client clientFiles/client.cpp

# Server files
server: mail messageHandler serverClient main serverFiles/server.cpp mail.o messageHandler.o serverClient.o main.o
	g++ -o twmailer-server serverFiles/server.cpp mail.o messageHandler.o serverClient.o main.o

main: serverFiles/main.cpp
	g++ -c -Wall serverFiles/main.cpp

serverClient: serverFiles/serverClient.cpp
	g++ -c -Wall serverFiles/serverClient.cpp

messageHandler: serverFiles/messageHandler.cpp
	g++ -c -Wall serverFiles/messageHandler.cpp

mail: serverFiles/mail.cpp
	g++ -c -Wall serverFiles/mail.cpp

# Cleans
clean:
	find . -type f -name '*.h.gch' -delete -o -name '*.o' -delete
