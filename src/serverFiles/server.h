#pragma once

#include <vector>
#include <iostream>
#include "serverClient.h"
#include "messageHandler.h"

namespace twMailerServer
{
    class server
    {
    private:
        std::vector<client *> clients;
        int port;
        std::string mailSpoolDirectoryname;
        int create_socket;
        int currentClientId;
    public:
        void start();
        void startListening();
        void abort();

        server(int port, std::string mailSpoolDirectoryname);
        ~server();
    };
}
