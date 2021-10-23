#pragma once

#include <iostream>

namespace twMailerServer
{
    class storageManager
    {
    private:
        std::string _path;

        void saveOutbox(std::string sender, std::string subject, std::string content);
        void saveInbox(std::string receiver, std::string subject, std::string content);
    public:
        storageManager(std::string path);
        ~storageManager();

        void saveMail(
            std::string sender, 
            std::string receiver,
            std::string subject,
            std::string content);
    };
}
