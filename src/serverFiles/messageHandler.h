#pragma once

#include <mutex>
#include <string>
#include <sstream>
#include <vector>
#include "serverClient.h"
#include "mail.h"
#include "blacklist.h"

namespace twMailerServer
{
    class messageHandler
    {
    private:
        static std::mutex getMailMutex;
        static std::mutex createTxtMutex;
        static std::mutex createDirMutex;
        static std::string storagePath;

        // Storage handling
        static bool getMailsFromUser(std::string username, bool inbox, std::vector<mail> &mails);
        static bool saveMail(mail mail);
        static bool tryMakeDir(std::string path);
        static bool tryMakeTxt(std::string path, std::string content);
    public:
        static twMailerServer::blacklist *myBlacklist;
        
        static void init(std::string storagePath);

        // Data handling
        // static std::string getSubject(std::istringstream &stream);
        static std::string getUsername(std::istringstream &stream);
        // static std::string getContent(std::istringstream &stream);
        static std::string getNextLine(std::istringstream &stream);
        static std::string getNextLine(std::istringstream &stream, size_t maxChars);

        // Message handling (Commands)
        static std::string handleMessage(std::string msg, client &c);
        static std::string sendMail(std::istringstream &stream, client &c);
        static std::string readMail(std::istringstream &stream);
        static std::string listMails(std::istringstream &stream);
        static std::string deleteMail(std::istringstream &stream);
        static std::string login(std::istringstream &stream, client &c);

    };
}