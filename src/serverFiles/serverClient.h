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
        std::thread myThread;

        void recieve();
        bool sendMessage(const char buffer[]);
        void abort();

        int getId();

        client(int clientId, void *data);
        ~client();
    };
}