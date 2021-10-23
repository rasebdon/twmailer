#pragma once

#include <thread>

namespace twMailerServer 
{
    class client
    {
    private:
        int clientId;
        int* socket;
    public:
        std::thread myThread;

        void recieve();
        bool sendMessage(const char buffer[]);
        void abort();

        int getId();

        client(int clientId, void *data, std::string storagePath);
        ~client();
    };
}