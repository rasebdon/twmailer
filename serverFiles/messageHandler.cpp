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
#include "messageHandler.h"
#include "mail.h"

namespace twMailerServer
{
    std::string messageHandler::storagePath;

    void messageHandler::init(std::string storagePath)
    {
        messageHandler::storagePath = storagePath;
    }

    std::string messageHandler::handleMessage(std::string msg, client &c)
    {
        std::cout << "Client(" << c.getId() << ") - Message received: " << msg << std::endl;
        // Get message id (first character as integer)
        int id = atoi(&msg.at(0));
        msg.erase(0, 2); // Remove read string and new line spacer

        switch (id)
        {
        // 0 is parsing error (from atoi)
        case 0:
        default:
            return "Invalid command!";
        // SEND
        case 1:
            return messageHandler::sendMail(msg);
        // LIST
        case 2:
            return messageHandler::listMails(msg);
        // READ
        case 3:
            return messageHandler::readMail(msg);
        // DEL
        case 4:
            return messageHandler::deleteMail(msg);
            break;
        // QUIT
        case 5:

            break;
        }

        return std::string("FATAL_ERROR");
    }

    std::string messageHandler::deleteMail(std::string &data)
    {
        // Get username
        std::string username(messageHandler::getNextLine(data, 8));

        std::vector<mail> mails;
        if(!messageHandler::getMailsFromUser(username, true, mails)) 
        {
            return "ERR\n";
        }

        // Get mail index
        std::string line;
        std::istringstream stream(data);
        getline(stream, line);
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

    std::string messageHandler::readMail(std::string &data)
    {
        // Get username
        std::string username(messageHandler::getNextLine(data, 8));

        std::vector<mail> mails;
        if(!messageHandler::getMailsFromUser(username, true, mails)) 
        {
            return "ERR\n";
        }

        // Get mail index
        std::string line;
        std::istringstream stream(data);
        getline(stream, line);
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

            std::cout << fileName << std::endl;

            // Try to read mail
            std::ifstream ifs(path + fileName);
            std::string content;
            content.assign(
                (std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));

            mails.push_back(mail(content, path + fileName));
        }
        closedir(dir);

        return true;
    }

    std::string messageHandler::listMails(std::string &data)
    {
        // Get username
        std::string username(messageHandler::getNextLine(data, 8));

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

        std::cout << answer << std::endl;

        return answer;
    }

    std::string messageHandler::sendMail(std::string &data)
    {
        std::cout << "SEND command received!" << std::endl;
        // Parse sender username
        std::string sender = messageHandler::getNextLine(data, 8);
        std::cout << "Sender: " << sender << std::endl;
        // Parse reciever username
        std::string receiver = messageHandler::getNextLine(data, 8);
        std::cout << "Reciever: " << receiver << std::endl;
        // Parse subject (Max 80 chars)
        std::string subject = messageHandler::getNextLine(data, 80);
        std::cout << "Subject: " << subject << std::endl;
        // Parse rest as message
        std::string content = messageHandler::getContent(data);
        std::cout << "Content: " << content << std::endl;

        // Check if mail data is valid
        if (sender.size() == 0 || receiver.size() == 0 || subject.size() == 0 || content.size() == 0)
            return "ERR\n";

        // Save mail as file in inbox of reciever and sent messages from sender
        mail mail(sender, receiver, subject, content);

        bool success = messageHandler::saveMail(mail);

        return success ? "OK\n" : "ERR\n";
    }

    // Reads the rest of the message and deletes it from the string
    std::string messageHandler::getContent(std::string &data)
    {
        std::string str("");
        std::string i("");
        do
        {
            i = getNextLine(data);
            i.push_back('\n');

            if (i.at(0) == '\n')
                break;

            str += i;
        } while (data.size() > 0);

        return str;
    }

    // Reads until double new lines are read
    std::string messageHandler::getNextLine(std::string &data)
    {
        std::string str("");
        size_t i = 0;
        do
        {
            if (i >= data.size())
            {
                data.erase(0, i);
                return str;
            }

            char c = data.at(i);
            if (c == '\n')
            {
                if (i == 0)
                {
                    str.push_back(c);
                    return str;
                }
                break;
            }
            else
            {
                str.push_back(c);
                i++;
            }
        } while (true);
        data.erase(0, i + 1);
        return str;
    }

    std::string messageHandler::getNextLine(std::string &data, size_t maxChars)
    {
        std::string str(getNextLine(data));
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