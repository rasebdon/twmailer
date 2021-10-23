#include "storageManager.h"

namespace twMailerServer
{
    storageManager::storageManager(std::string path)
    {
        this->_path = path;
    }

    // ===== SAVING =====
    void storageManager::saveMail(
        std::string sender,
        std::string receiver,
        std::string subject,
        std::string content)
    {
        // Get folder

        // Save inbox

        // Save outbox

    }

    void storageManager::saveInbox(
        std::string receiver,
        std::string subject,
        std::string content)
    {
        // Get/Create folder of receiver with inbox

        // Generate file name

        // Save mail as txt

    }

    void storageManager::saveOutbox(
        std::string sender,
        std::string subject,
        std::string content)
    {
        // Get/Create folder of sender with outbox

        // Generate file name

        // Save mail as txt

    }

}
