#include "Client.hpp"
#include <string.h>

// Constructor: initializes socket and server address
Client::Client(const std::string& ip, int port) : server_ip(ip), server_port(port), sock(-1) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Error: Unable to create socket.");
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        throw std::runtime_error("Error: Invalid server IP address.");
    }
}

// Destructor: close the socket
Client::~Client() {
    if (sock >= 0) {
        close(sock);
    }
}

// Connect to the server
void Client::connectToServer() {
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        throw std::runtime_error("Error: Unable to connect to the server.");
    }
    std::cout << "Connected to server at " << server_ip << ":" << server_port << std::endl;
}

// Send a string request to the server
void Client::sendRequest(const std::string& request) {
    if (send(sock, request.c_str(), request.size(), 0) < 0) {
        throw std::runtime_error("Error: Unable to send data to the server.");
    }
    std::cout << "Request sent to server: " << request << std::endl;
}

// Receive a response from the server
std::string Client::receiveResponse() {
    char buffer[4096] = {0};
    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        throw std::runtime_error("Error: Unable to receive data from the server.");
    }
    return std::string(buffer, bytes_received);
}

#include "Client.hpp"

int main() {
    try {
        
        Client client("127.0.0.1", 9034);

        
        client.connectToServer();

        
        std::string request = "MST;graph:{vertices:A,B,C,D;edges:A-B-5,B-C-3,C-D-7,A-D-10}";

        
        client.sendRequest(request);

        
        std::string response = client.receiveResponse();

      
        std::cout << "Response: " << response << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
