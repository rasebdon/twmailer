#include "blacklist.h"
#include <string>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <mutex>
#include <map>
#include <iomanip>

namespace twMailerServer
{
    bool blacklist::canLogin(std::string ipAddress)
    {
        // Get blacklist file
        m.lock();

        std::string fileName(savePath);

        // Try to read file
        std::ifstream ifs(savePath);
        std::string content;
        content.assign(
            (std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));

        std::istringstream filestream(content);

        m.unlock();

        while (!filestream.eof())
        {
            std::string chronoString;
            std::string ip;
            std::string line;

            getline(filestream, line);

            std::stringstream linestream(line);

            getline(linestream, ip, ':');
            getline(linestream, chronoString, ':');

            if(ipAddress == ip)
            {
                // Parse time
                auto blTime = std::stol(chronoString);

                // Get current time
                auto curTp = std::chrono::system_clock::now().time_since_epoch().count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;

                if((curTp - blTime) < 60) 
                {
                    return false;
                }
            }        
        }    

        return true;
    }
    void blacklist::failedAttempt(std::string ipAddress)
    {
        try 
        {
            if(attempts[ipAddress] < 2) 
            {
                attempts[ipAddress]++;
                std::cout << "Failed login from " << ipAddress << std::endl;
            }
            else 
            {
                std::cout << "Too many failed logins, saving " << ipAddress << " to blacklist: " << savePath <<"!" << std::endl;

                std::ofstream myfile(savePath, std::ios::app);

                auto curTp = std::chrono::system_clock::now().time_since_epoch().count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
                std::string line = ipAddress + ":" + std::to_string(curTp) + "\n";

                myfile.write(line.c_str(), line.length());

                attempts[ipAddress] = 0;
            }
        }
        catch(std::exception &ex)
        {
            std::cout << ex.what() << std::endl;

            attempts[ipAddress]++;
        }
    }

    blacklist::blacklist(std::string savePath)
    {       
        this->savePath = savePath + "/blacklist.txt";
    }

    blacklist::~blacklist()
    {

    }

}
