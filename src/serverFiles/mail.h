#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace twMailerServer
{
    class mail
    {
    private:
        std::string _path;
        std::string _sender;
        std::string _receiver;
        std::string _subject;
        std::string _content;
        mail(std::string sender,
            std::string receiver,
            std::string subject,
            std::string content);

        bool getNextLine(std::istringstream &stream, std::string &string, size_t maxLength);
    public:
        std::string toString();

        mail(std::istringstream &stream, std::string sender, std::string path);
        mail(std::istringstream &stream, std::string sender);
        ~mail();

        std::string getSender();
        std::string getReceiver();
        std::string getSubject();
        std::string getContent();
        std::string getPath();
    };
}