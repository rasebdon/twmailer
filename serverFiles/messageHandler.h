#pragma once

#include <iostream>
#include "serverClient.h"
// #include "storageManager.h"

namespace twMailerServer
{
    class messageHandler
    {
    private:
        // static storageManager* _storage;
    public:
        // static void loadStorage(twMailerServer::storageManager* storage);
        // static void deleteStorageManager();
        // static bool hasStorage();

        static std::string getSubject(std::string &data);
        static std::string getUsername(std::string &data);
        static std::string getContent(std::string &data);
        static std::string getNextLine(std::string &data);
        static std::string getNextLine(std::string &data, size_t maxChars);
        static std::string sendMail(std::string &data);
        static std::string handleMessage(std::string msg, client &c);
    };
}