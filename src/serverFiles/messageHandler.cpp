#include <bits/stdc++.h>
#include <ctime>
#include <dirent.h>
#include <errno.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include "../shared/protocol.h"
#include "messageHandler.h"
#include "mail.h"

namespace twMailerServer
{
    std::string messageHandler::storagePath;

    void messageHandler::init(std::string storagePath)
    {
        messageHandler::storagePath = storagePath;

        // Try create storage dir
        messageHandler::tryMakeDir(storagePath);
    }

    std::string messageHandler::handleMessage(std::string msg, client &c)
    {
        // Convert string to stream
        std::istringstream stream(msg);

        // Get message command (first line)
        std::string cmd = "";
        getline(stream, cmd);

        std::cout << "Client(" << c.getId() << ") - " << cmd << " command received" << std::endl;

        if (cmd == COMMAND_SEND)
        {
            return messageHandler::sendMail(stream);
        }
        else if (cmd == COMMAND_LIST)
        {
            return messageHandler::listMails(stream);
        }
        else if (cmd == COMMAND_READ)
        {
            return messageHandler::readMail(stream);
        }
        else if (cmd == COMMAND_DEL)
        {
            return messageHandler::deleteMail(stream);
        }
        else if (cmd == COMMAND_QUIT)
        {
            return "Client quit!";
        }

        return std::string("Unknown message received!");
    }

    std::string messageHandler::deleteMail(std::istringstream &stream)
    {
        // Get username
        std::string username = messageHandler::getUsername(stream);

        std::vector<mail> mails;
        if(!messageHandler::getMailsFromUser(username, true, mails)) 
        {
            return "ERR\n";
        }

        // Get mail index
        std::string line;
        stream >> line;
        size_t index = atol(line.c_str());

        if(mails.size() <= index)
        {
            return "ERR\n";
        }

        // Delete mail file
        if(remove(mails.at(index).getPath().c_str()) == 0) {
            return "OK\n";
        }
        else {
            return "ERR\n";
        }
    }

    std::string messageHandler::readMail(std::istringstream &stream)
    {
        // Get username
        std::string username = messageHandler::getUsername(stream);

        std::vector<mail> mails;
        if(!messageHandler::getMailsFromUser(username, true, mails)) 
        {
            return "ERR\n";
        }

        // Get mail index
        std::string line;
        stream >> line;
        size_t index = atol(line.c_str());
        if(mails.size() <= index)
        {
            return "ERR\n";
        }

        // Return mail
        return "OK\n" + mails.at(index).toString();
    }

    bool messageHandler::getMailsFromUser(std::string username, bool inbox, std::vector<mail> &mails) {
        // Find folder
        std::string path(storagePath + "/" + username + "/" + (inbox ? "inbox" : "outbox") + "/");
        DIR *dir;
        struct dirent *ent;

        // Try to open the directory
        if ((dir = opendir(path.c_str())) == NULL)
        {
            return false;
        }

        // Iterate through the directory
        while ((ent = readdir(dir)) != NULL)
        {
            std::string fileName(ent->d_name);

            if (fileName == "." || fileName == "..")
                continue;

            // Try to read mail
            std::ifstream ifs(path + fileName);
            std::string content;
            content.assign(
                (std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));

            std::istringstream fileStream(content);
            mails.push_back(mail(fileStream, path + fileName));
        }
        closedir(dir);

        return true;
    }

    std::string messageHandler::listMails(std::istringstream &stream)
    {
        // Get username
        std::string username = messageHandler::getUsername(stream);

        std::vector<mail> mails;
        if(!messageHandler::getMailsFromUser(username, true, mails)) 
        {
            return "0\n";
        }

        // Build message
        std::string answer("");
        answer += "" + std::to_string(mails.size()) + "\n";
        for (size_t i = 0; i < mails.size(); i++)
        {
            answer += mails.at(i).getSubject() + "\n";
        }

        return answer;
    }

    std::string messageHandler::sendMail(std::istringstream &stream)
    {
        try {
            // Create mail from stream and return the save state
            mail mail(stream);
            bool success = messageHandler::saveMail(mail);
            return success ? "OK\n" : "ERR\n";
        }
        catch (...) {
            return "ERR\n";
        }
    }

    std::string messageHandler::getUsername(std::istringstream &stream) {
        return getNextLine(stream, 8);
    }

    // Reads until double new lines are read
    std::string messageHandler::getNextLine(std::istringstream &stream)
    {
        std::string line("");
        getline(stream, line);
        return line;
    }

    std::string messageHandler::getNextLine(std::istringstream &stream, size_t maxChars)
    {
        std::string str = getNextLine(stream);
        while (str.size() > maxChars && str.size() > 0)
        {
            str.pop_back();
        }
        return str;
    }

    // ===== STORAGE =====

    bool messageHandler::tryMakeDir(std::string path)
    {
        if (mkdir((path).c_str(), 0777) != 0)
        {
            // Check if the folder exists
            DIR *dir = opendir(path.c_str());
            if (!dir && ENOENT == errno)
            {
                std::cerr << path << " could not be created!" << std::endl;
                return false;
            }
        }
        return true;
    }

    bool messageHandler::tryMakeTxt(std::string path, std::string content)
    {
        std::ofstream file(path);
        file << content;
        file.close();

        // Check if file was written
        return access(path.c_str(), F_OK) == 0;
    }

    bool messageHandler::saveMail(mail mail)
    {
        // Get/Create folders
        std::string senderFolderPath(messageHandler::storagePath + "/" + mail.getSender());
        std::string receiverFolderPath(messageHandler::storagePath + "/" + mail.getReceiver());
        std::string senderOutboxFolderPath(messageHandler::storagePath + "/" + mail.getSender() + "/outbox");
        std::string receiverInboxFolderPath(messageHandler::storagePath + "/" + mail.getReceiver() + "/inbox");

        if (!messageHandler::tryMakeDir(senderFolderPath))
            return false;
        if (!messageHandler::tryMakeDir(receiverFolderPath))
            return false;
        if (!messageHandler::tryMakeDir(senderOutboxFolderPath))
            return false;
        if (!messageHandler::tryMakeDir(receiverInboxFolderPath))
            return false;

        // Generate file name
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y%m%d%m%H%M%S") << ".txt";
        auto filename = oss.str();

        // File content creation
        std::string fileContent(mail.toString());

        // Save mail as txt to both folders
        if (!messageHandler::tryMakeTxt(senderOutboxFolderPath + "/" + filename, fileContent))
            return false;
        if (!messageHandler::tryMakeTxt(receiverInboxFolderPath + "/" + filename, fileContent))
            return false;

        return true;
    }
}