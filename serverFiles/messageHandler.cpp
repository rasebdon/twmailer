#include "messageHandler.h"
#include <string>

namespace twMailerServer
{
    // void messageHandler::loadStorage(twMailerServer::storageManager* storage) {
    //     messageHandler::_storage = storage;
    // }

    // void messageHandler::deleteStorageManager() {
    //     delete(_storage);
    // }

    // bool messageHandler::hasStorage() {
    //     return messageHandler::_storage != nullptr;
    // }

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

            break;
        // READ
        case 3:

            break;
        // DEL
        case 4:

            break;
        // QUIT
        case 5:

            break;
        }

        return std::string("OK");
    }

    std::string messageHandler::sendMail(std::string &data)
    {
        std::cout << "SEND command received!" << std::endl;
        // Parse sender username
        std::string sender = messageHandler::getNextLine(data, 8);
        std::cout << "Sender: " << sender << std::endl;
        // Parse reciever username
        std::string reciever = messageHandler::getNextLine(data, 8);
        std::cout << "Reciever: " << reciever << std::endl;
        // Parse subject (Max 80 chars)
        std::string subject = messageHandler::getNextLine(data, 80);
        std::cout << "Subject: " << subject << std::endl;
        // Parse rest as message
        std::string content = messageHandler::getContent(data);
        std::cout << "Content: " << content << std::endl;

        // Save mail as file in inbox of reciever and sent messages from sender


        // TODO -> SAVE MAIL

        return "SENDING SUCCESSFUL!";
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

            if(i.at(0) == '\n')
                break;

            str += i;
        } while (data.size() > 0);
        
        return str;
    }

    std::string messageHandler::getNextLine(std::string &data)
    {
        std::string str("");
        size_t i = 0;
        do
        {
            if(i >= data.size()) {
                data.erase(0, i);
                return str;
            }

            char c = data.at(i);
            if (c == '\n')
            {
                if(i == 0) {
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

    std::string messageHandler::getNextLine(std::string &data, size_t maxChars) {
        std::string str(getNextLine(data)); 
        while (str.size() > maxChars && str.size() > 0)
        {
            str.pop_back();
        }
        return str;
    }
}