#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Client {
private:
    int sock;                         // Socket identifier
    std::string server_ip;            // Server IP address
    int server_port;                  // Server port
    struct sockaddr_in server_address; // Server address structure

public:
    // Constructor: receives server IP and port
    Client(const std::string& ip, int port);

    // Destructor: close the socket
    ~Client();

    // Connect to the server
    void connectToServer();

    // Send a simple string request to the server
    void sendRequest(const std::string& request);

    // Receive a response from the server
    std::string receiveResponse();
};

#endif // CLIENT_HPP
