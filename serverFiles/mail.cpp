#include <iostream>
#include <sstream>
#include "mail.h"

namespace twMailerServer
{

    mail::mail(std::string sender,
               std::string receiver,
               std::string subject,
               std::string content)
    {
        _sender = sender;
        _receiver = receiver;
        _subject = subject;
        _content = content;
    }
    mail::mail(std::string mailString)
    {
        std::istringstream file(mailString);

        // Parse mail
        if (!getNextLine(file, "SENDER:", _sender))
            return;
        if (!getNextLine(file, "RECEIVER:", _receiver))
            return;
        if (!getNextLine(file, "SUBJECT:", _subject))
            return;

        // Parse content
        std::string line;
        getline(file, line);
        if (line != "CONTENT:")
        {
            std::cerr << "Mail body not correct! Cancelling parsing of this mail!" << std::endl;
            return;
        }

        while (std::getline(file, line))
        {
            if (line + "\n" == "\n")
                break;

            _content += line + "\n";
        }
    }

    bool mail::getNextLine(std::istringstream &file, std::string expectedDescriptor, std::string &string)
    {
        std::string line;
        getline(file, line);
        if (line != expectedDescriptor)
        {
            std::cerr << "Mail body not correct! Cancelling parsing of this mail!" << std::endl;
            return false;
        }
        // Set sender
        getline(file, line);
        string = line;
        return true;
    }

    std::string mail::toString()
    {
        return "SENDER:\n" + _sender + "\nRECEIVER:\n" + _receiver + "\nSUBJECT:\n" + _subject + "\nCONTENT:\n" + _content;
    }

    // ===== GETTER =====

    std::string mail::getSender()
    {
        return _sender;
    }
    std::string mail::getReceiver()
    {
        return _receiver;
    }
    std::string mail::getSubject()
    {
        return _subject;
    }
    std::string mail::getContent()
    {
        return _content;
    }

    mail::~mail()
    {
    }
}
