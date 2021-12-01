LIBS=-lldap -llber
SERVER_FILES_PATH=serverFiles

# all
all: client server

# Client files
client: protocol clientFiles/client.cpp protocol.o
	g++ -o twmailer-client clientFiles/client.cpp protocol.o

# Server files
server: protocol blacklist mail messageHandler serverClient main ${SERVER_FILES_PATH}/server.cpp protocol.o blacklist.o mail.o messageHandler.o serverClient.o main.o
	g++ -o twmailer-server ${SERVER_FILES_PATH}/server.cpp protocol.o blacklist.o mail.o messageHandler.o serverClient.o main.o ${LIBS}

main: ${SERVER_FILES_PATH}/main.cpp
	g++ -c -Wall ${SERVER_FILES_PATH}/main.cpp

serverClient: ${SERVER_FILES_PATH}/serverClient.cpp
	g++ -c -Wall ${SERVER_FILES_PATH}/serverClient.cpp

messageHandler: ${SERVER_FILES_PATH}/messageHandler.cpp
	g++ -c -Wall ${SERVER_FILES_PATH}/messageHandler.cpp

mail: ${SERVER_FILES_PATH}/mail.cpp
	g++ -c -Wall ${SERVER_FILES_PATH}/mail.cpp

blacklist: ${SERVER_FILES_PATH}/blacklist.cpp
	g++ -c -Wall ${SERVER_FILES_PATH}/blacklist.cpp

# Shared files
protocol: shared/protocol.cpp
	g++ -c -Wall shared/protocol.cpp

# Cleans
clean:
	find . -type f -name '*.h.gch' -delete -o -name '*.o' -delete
