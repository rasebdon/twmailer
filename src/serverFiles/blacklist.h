#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <mutex>

namespace twMailerServer
{
    class blacklist
    {
    private:
        std::mutex m;
        std::map<std::string, int> attempts;
    public:
        std::string savePath;
        bool canLogin(std::string ipAddress);
        void failedAttempt(std::string ipAddress);
        blacklist(std::string savePath);
        ~blacklist();
    };
}
