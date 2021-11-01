#include <string>
#include <vector>
#include <iostream>
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