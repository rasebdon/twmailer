#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <thread>
#include "server.h"

// Program entry point
int main(int argc, char const *argv[])
{
    int port;
    std::string mailSpoolDirectoryname;
    std::vector<std::string> args(argv, argv + argc);

    // Get start arguments
    try {
        if(args.size() != 3) {
            throw std::invalid_argument("Invalid number of program arguments given!");
        }
        // String to int
        port = std::stoi(argv[1]);
        mailSpoolDirectoryname = argv[2];
    }
    catch (...) {
        std::cerr << "Error: Invalid program usage" << std::endl;
        std::cout << "Mail server usage: ./twmailer-server <port> <mail-spool-directoryname>" << std::endl;
        return EXIT_FAILURE;
    }

    // Start server
    try {
        twMailerServer::server server(port, mailSpoolDirectoryname);
        server.start();
    }
    catch(const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}

namespace twMailerServer 
{

    server::server(int port, std::string mailSpoolDirectoryname)
    {
        this->port = port;
        this->mailSpoolDirectoryname = mailSpoolDirectoryname;
    }

    server::~server()
    {
        abort();
    }

    void server::start() {
        std::cout << "Starting server on port: " << this->port << std::endl;

        // Create socket
        create_socket = -1;
        if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
        throw std::runtime_error("Socket error");
        }

        int reuseValue = 1;
        // Set socket options
        // Reuse socket address
        if (setsockopt(create_socket, SOL_SOCKET, SO_REUSEADDR, &reuseValue,
                    sizeof(reuseValue)) == -1) 
        {
        throw std::runtime_error("Set socket options - reuseAddr");
        }

        // Reuse port
        if (setsockopt(create_socket, SOL_SOCKET, SO_REUSEPORT, &reuseValue,
                    sizeof(reuseValue)) == -1) 
        {
        throw std::runtime_error("Set socket options - reuseAddr");
        }

        // Init address
        struct sockaddr_in address;
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind socket to port
        if (bind(create_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
            throw std::runtime_error("Bind error");
        }

        std::cout << "Server setup complete!" << this->port << std::endl;

        // Start listening
        startListening();
    }

    void server::startListening() {
        std::cout << "Start listening..." << std::endl;

        if (listen(create_socket, 5) == -1)
        {
            throw std::runtime_error("Listen error");
        }

        currentClientId = 0;
        int abortRequested = 0;
        int new_socket = -1;
        socklen_t addrlen;
        struct sockaddr_in cliaddress;

        // Start listening loop
        while (!abortRequested)
        {
            std::cout << "Waiting for connections..." << std::endl;

            // Accept new connections
            addrlen = sizeof(struct sockaddr_in);
            if ((new_socket = accept(create_socket,
                                    (struct sockaddr *)&cliaddress,
                                    &addrlen)) == -1)
            {
                if (abortRequested)
                {
                    perror("accept error after aborted");
                }
                else
                {
                    perror("accept error");
                }
                break;
            }

        
            // Create new client
            std::cout << "Client connected from " << inet_ntoa(cliaddress.sin_addr)
                    << ":" << ntohs(cliaddress.sin_port) << "..." << std::endl;
            
            client* c = new client(currentClientId++, &new_socket, this->mailSpoolDirectoryname);
            clients.push_back(c);
            
            // Start client as new thread
            // client.myThread = std::thread(&client::recieve, *c);
            c->recieve(); // Currently single threaded

            new_socket = -1;
        }
    }

    void server::abort() {
        // Frees the descriptor
        if (create_socket != -1)
        {
            if (shutdown(create_socket, SHUT_RDWR) == -1)
            {
                perror("shutdown create_socket");
            }
            if (close(create_socket) == -1)
            {
                perror("close create_socket");
            }
            create_socket = -1;
        }

        // Delete clients
        for (size_t i = 0; i < clients.size(); i++)
        {
            delete(clients.at(i));
        }
    }
}