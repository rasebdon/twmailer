#pragma once

#include <string>
#include "serverClient.h"
#include "mail.h"

namespace twMailerServer
{
    class messageHandler
    {
    private:
        static std::string storagePath;

        static bool saveMail(mail mail);
        static bool tryMakeDir(std::string path);
        static bool tryMakeTxt(std::string path, std::string content);
    public:
        static void init(std::string storagePath);

        // Data handling
        static std::string getSubject(std::string &data);
        static std::string getUsername(std::string &data);
        static std::string getContent(std::string &data);
        static std::string getNextLine(std::string &data);
        static std::string getNextLine(std::string &data, size_t maxChars);

        // Message handling
        static std::string handleMessage(std::string msg, client &c);
        static std::string sendMail(std::string &data);
        static std::string listMails(std::string &data);

    };
}