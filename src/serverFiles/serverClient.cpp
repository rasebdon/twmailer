#include <sys/socket.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <iomanip>
#include <ctime>
#include <bits/stdc++.h>
#include "serverClient.h"
#include "messageHandler.h"

#define BUF 2048

namespace twMailerServer
{

    client::client(int clientId, std::string ipAddress, void *data)
    {
        this->ipAddress = ipAddress;
        this->abortRequested = false;
        this->username = "";
        this->loggedIn = false;
        this->clientId = clientId;
        this->socket = (int *)data;
    }
    client::~client()
    {
        abort();
    }

    // ===== COMMUNICATION =====
    void client::recieve()
    {
        char buffer[BUF];
        int size;

        // Receive data
        do
        {
            size = recv(*socket, buffer, BUF - 1, 0);
            if (size == -1)
            {
                if (abortRequested)
                {
                    std::cerr << "Client(" << clientId << ") receive error after aborted";
                }
                else
                {
                    std::cerr << "Client(" << clientId << ") receive error";
                }
                break;
            }

            if (size == 0)
            {
                std::cout << "Client(" << clientId << ") closed remote socket" << std::endl;
                break;
            }

            // remove ugly debug message, because of the sent newline of client
            if (buffer[size - 2] == '\r' && buffer[size - 1] == '\n')
            {
                size -= 2;
            }
            else if (buffer[size - 1] == '\n')
            {
                --size;
            }

            buffer[size] = '\0';

            std::string returnMessage;
            try
            {
                returnMessage = messageHandler::handleMessage(std::string(buffer), *this);
            }
            catch (const std::exception &ex)
            {
                std::cout << std::string(ex.what()) << std::endl;
                returnMessage = "Error sending message!";
            }

            if(abortRequested) break;

            if (sendMessage(returnMessage.c_str()) == false)
            {
                throw std::runtime_error("Send answer failed");
            }
        } while (strcmp(buffer, "QUIT") != 0 && !abortRequested);

        // Stop client
        abort();
    }
    bool client::sendMessage(const char buffer[])
    {
        if (send(*socket, buffer, strlen(buffer), 0) == -1)
        {
            std::cerr << "Send failed!" << std::endl;
            return false;
        }
        return true;
    }

    // ===== MISC =====

    void client::abort()
    {
        // Closes/Frees the descriptor if not already
        if (*socket != -1)
        {
            if (shutdown(*socket, SHUT_RDWR) == -1)
            {
                perror("shutdown new_socket");
            }
            if (close(*socket) == -1)
            {
                perror("close new_socket");
            }
            *socket = -1;
        }

        std::cout << "Client(" << clientId << " disconnected!";

        // TODO : Join thread
        // myThread.join();
    }

    int client::getId()
    {
        return this->clientId;
    }
}