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

    mail::mail(std::istringstream &stream, std::string path) : mail(stream) {
        _path = path;
    }

    mail::mail(std::istringstream &stream)
    {
        // Parse mail
        if (!getNextLine(stream, _sender, 8))
            return;
        if (!getNextLine(stream, _receiver, 8))
            return;
        if (!getNextLine(stream, _subject, 80))
            return;

        // Parse content
        std::string line = "";
        while (std::getline(stream, line))
        {
            if (line.size() > 0 && line.at(0) == '.')
                break;

            _content += line + "\n";
        }
    }

    bool mail::getNextLine(std::istringstream &stream, std::string &string, size_t maxLength)
    {
        std::string line;
        getline(stream, line);
        string = line;
        return true;
    }

    std::string mail::toString()
    {
        return _sender + "\n" +_receiver + "\n" + _subject + "\n" + _content;
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
    std::string mail::getPath()
    {
        return _path;
    }

    mail::~mail()
    {
    }
}
