#pragma once

#include <string>
#include <thread>

namespace twMailerServer 
{
    class client
    {
    private:
        int clientId;
        int* socket;
    public:
        bool loggedIn;
        bool abortRequested;
        std::string username;
        std::string ipAddress;

        void recieve();
        bool sendMessage(const char buffer[]);
        void abort();

        int getId();

        client(int clientId, std::string ipAddress, void *data);
        ~client();
    };
}