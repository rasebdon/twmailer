# all
all: client server

# Client files
client: protocol clientFiles/client.cpp protocol.o
	g++ -o twmailer-client clientFiles/client.cpp protocol.o

# Server files
server: protocol mail messageHandler serverClient main serverFiles/server.cpp protocol.o mail.o messageHandler.o serverClient.o main.o
	g++ -o twmailer-server serverFiles/server.cpp protocol.o mail.o messageHandler.o serverClient.o main.o

main: serverFiles/main.cpp
	g++ -c -Wall serverFiles/main.cpp

serverClient: serverFiles/serverClient.cpp
	g++ -c -Wall serverFiles/serverClient.cpp

messageHandler: serverFiles/messageHandler.cpp
	g++ -c -Wall serverFiles/messageHandler.cpp

mail: serverFiles/mail.cpp
	g++ -c -Wall serverFiles/mail.cpp

# Shared files
protocol: shared/protocol.cpp
	g++ -c -Wall shared/protocol.cpp

# Cleans
clean:
	find . -type f -name '*.h.gch' -delete -o -name '*.o' -delete
