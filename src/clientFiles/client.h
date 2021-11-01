#include <iostream>

namespace twMailerClient
{
    class client
    {
    private:
        struct sockaddr_in address;
        int port; 
        int mySocket;
        char receiveBuffer[];

        std::string getUsername();
        std::string getSubject();
        std::string getContent();
        std::string getInputWithLength(int len);
    public:
        void start();
        bool sendMessage(const char buffer[], int size);
        void receive();
        void abort();
        void handleUserInput();

        client(std::string ip, int port);
        ~client();
    };
}