#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "../shared/protocol.h"
#include "client.h"

#define BUF 2048

// Program starting point
int main(int argc, char const *argv[])
{
    int port;
    std::string ip;
    std::vector<std::string> args(argv, argv + argc);

    // Get start arguments
    try
    {
        if (args.size() != 3)
        {
            throw std::invalid_argument("Invalid number of program arguments given!");
        }
        // String to int
        ip = argv[1];
        port = std::stoi(argv[2]);
    }
    catch (...)
    {
        std::cerr << "Error: Invalid program usage" << std::endl;
        std::cout << "Mail client usage: ./twmailer-client <ip> <port> " << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        twMailerClient::client myClient(ip, port);
        myClient.start();
    }
    catch (std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Client implementation
namespace twMailerClient
{
    client::client(std::string ip, int port)
    {
        std::cout << "Setting up client..." << std::endl;
        this->port = port;
        this->receiveBuffer[BUF];

        // Create socket
        if ((mySocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            throw std::runtime_error("Socket error");
        }

        // Init address
        memset(&address, 0, sizeof(address)); // init storage with 0
        address.sin_family = AF_INET;         // IPv4
        // Set port
        address.sin_port = htons(port);
        // Set IP-Address
        inet_aton(ip.c_str(), &address.sin_addr);

        std::cout << "Client setup successful!" << std::endl;
    }

    client::~client()
    {
        abort();
    }

    // Starts the connection to the server
    void client::start()
    {
        std::string ipString = inet_ntoa(address.sin_addr);
        std::cout << "Connecting to " << ipString << ":" << ntohs(address.sin_port) << std::endl;

        if (connect(mySocket, (struct sockaddr *)&address, sizeof(address)) == -1)
        {
            // https://man7.org/linux/man-pages/man3/perror.3.html
            throw std::runtime_error("Connect error - no server available");
        }

        std::cout << "Connection to server " << ipString << ":" << ntohs(address.sin_port) << " successful!" << std::endl;

        // Receive welcome message
        receive();

        // Start input loop
        handleUserInput();
    }

    // TODO : Move into input handler
    void client::handleUserInput()
    {
        bool abort = false;
        do
        {
            // Get input
            std::string command("");
            std::cout << ">> ";
            std::cin >> command;

            std::string msg("");

            if (command == COMMAND_SEND)
            {
                // Read sender
                std::cout << "Sender (max. 8 chars (a-z, 0-9): ";
                std::string sender = getUsername();
                // Read reciever
                std::cout << "Receiver (max. 8 chars (a-z, 0-9): ";
                std::string receiver = getUsername();
                // Read subject
                std::cout << "Subject (max. 80 chars): ";
                std::string subject = getSubject();
                // Get mail content
                std::cout << "Content: ";
                std::string content = getContent();

                // Build message
                msg = COMMAND_SEND + std::string("\n") +  sender + receiver + subject + content;
            }
            else if (command == COMMAND_LIST)
            {
                // Read username
                std::cout << "Username (max. 8 chars (a-z, 0-9): ";
                std::string username = getUsername();

                msg = COMMAND_LIST + std::string("\n") + username;
            }
            else if (command == COMMAND_READ)
            {
                // Read username
                std::cout << "Username (max. 8 chars (a-z, 0-9): ";
                std::string username = getUsername();
                // Read mail index
                std::cout << "Message number: ";
                size_t number = 0;
                std::cin >> number;

                msg = COMMAND_READ + std::string("\n") + username + "\n" + std::to_string(number);
            }
            else if (command == COMMAND_DEL)
            {
                // Read username
                std::cout << "Username (max. 8 chars (a-z, 0-9): ";
                std::string username = getUsername();
                // Read mail index
                std::cout << "Message number: ";
                size_t number = 0;
                std::cin >> number;

                msg = COMMAND_DEL + std::string("\n") + username + "\n" + std::to_string(number);
            }
            else if (command == "?" || command == "HELP") {
                std::cout << "Available commands:" << std::endl << std::endl;
                std::cout << "SEND - Send a mail" << std::endl;
                std::cout << "LIST - List the inbox" << std::endl;
                std::cout << "READ - View a mail" << std::endl;
                std::cout << "DEL  - Delete a mail" << std::endl;
                std::cout << "HELP - Shows this list" << std::endl;
            }
            else if (command == COMMAND_QUIT)
            {
                abort = true;
            }
            else
            {
                std::cout << "Write ? or HELP to list all available commands." << std::endl;
                continue;
            }

            // Send the input
            if (msg.size() > 0)
            {
                if (this->sendMessage(msg.c_str(), msg.length()) == false)
                {
                    std::cerr << "There was an error sending the message!" << std::endl;
                    break;
                }
                std::cout << "Message sent!" << std::endl;

                // Receive answer
                receive();
            }
        } while (!abort);
    }

    // TODO : Move into input handler
    std::string client::getUsername()
    {
        return getInputWithLength(8);
    }

    // TODO : Move into input handler
    std::string client::getSubject()
    {
        return getInputWithLength(80);
    }

    std::string client::getContent()
    {
        std::cout << "Ends reading when reading .\n!" << std::endl;
        std::string input("");
        std::string line("");
        do
        {
            std::getline(std::cin, line);
            input += line + "\n";
        } while (
            line.size() == 0 ||
            !(line.size() == 1 && line.at(0) == '.'));

        return input;
    }

    // TODO : Move into input handler
    std::string client::getInputWithLength(int len)
    {
        std::string input;
        std::getline(std::cin >> std::ws, input);
        if (input.size() >= len)
        {
            // Remove chars
            input.erase(len, input.size());
        }
        // Add newline at the end
        if (input.at(input.size() - 1) != '\n')
        {
            input.push_back('\n');
        }
        return input;
    }

    bool client::sendMessage(const char buffer[], int size)
    {
        return !(send(mySocket, buffer, size, 0) == -1);
    }

    // Listens for incoming messages
    void client::receive()
    {
        int size = -1;
        // Receive message
        size = recv(mySocket, receiveBuffer, BUF - 1, 0);
        // Message receive error
        if (size == -1)
        {
            throw std::runtime_error("recv error");
        }
        else if (size == 0)
        {
            throw std::runtime_error("Server closed remote socket");
        }
        else
        {
            receiveBuffer[size] = '\0';
            // Process message
            std::cout << "<< " << receiveBuffer << std::endl;
        }
    }

    void client::abort()
    {
        std::cout << "Stopping client!" << std::endl;

        // Close the descriptor
        if (mySocket != -1)
        {
            if (shutdown(mySocket, SHUT_RDWR) == -1)
            {
                // invalid in case the server is gone already
                std::cout << "Shutting down socket..." << std::endl;
            }
            if (close(mySocket) == -1)
            {
                std::cout << "Closing socket..." << std::endl;
            }
            mySocket = -1;
        }

        std::cout << "Client stopped successfully!" << std::endl;
    }
}