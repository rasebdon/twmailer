#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace twMailerServer
{
    class mail
    {
    private:
        std::string _sender;
        std::string _receiver;
        std::string _subject;
        std::string _content;

        bool getNextLine(std::istringstream &file, std::string expectedDescriptor, std::string &string);
    public:
        std::string toString();

        mail(std::string sender,
        std::string receiver,
        std::string subject,
        std::string content);
        mail(std::string mailString);
        ~mail();

        std::string getSender();
        std::string getReceiver();
        std::string getSubject();
        std::string getContent();
    };
}