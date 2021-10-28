#pragma once

#include <string>
#include "serverClient.h"

namespace twMailerServer
{
    class messageHandler
    {
    private:

    public:
        static std::string storagePath;
        static void init(std::string storagePath);

        static void saveMail(std::string sender, std::string receiver, std::string subject, std::string content);

        static std::string getSubject(std::string &data);
        static std::string getUsername(std::string &data);
        static std::string getContent(std::string &data);
        static std::string getNextLine(std::string &data);
        static std::string getNextLine(std::string &data, size_t maxChars);
        static std::string sendMail(std::string &data);
        static std::string handleMessage(std::string msg, client &c);
    };
}