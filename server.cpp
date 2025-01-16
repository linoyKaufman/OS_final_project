#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Graph.hpp"
#include "MST_algorithm.hpp"
#include "MST_factory.hpp"
#include "Tree.hpp"
#include "Reactor.hpp"

#define PORT 9034

// Pipeline and thread pool data structures
struct Request {
    int clientFd;
    std::string command;
    std::shared_ptr<Tree> graph;
};

std::queue<Request> requestQueue;
std::mutex queueMutex;
std::condition_variable queueCV;
bool serverRunning = true;

void pipelineWorker() {
    while (serverRunning) {
        Request request;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait(lock, [] { return !requestQueue.empty() || !serverRunning; });
            if (!serverRunning && requestQueue.empty()) return;

            request = requestQueue.front();
            requestQueue.pop();
        }

        std::cout << "Processing request: " << request.command << " from client " << request.clientFd << std::endl;

        // Process the request
        std::istringstream input(request.command);
        std::ostringstream result;
        std::string cmd;
        input >> cmd;

        if (cmd == "Newgraph") {
            int n, m;
            input >> n >> m;
            std::cout << "Creating new graph with " << n << " vertices and " << m << " edges." << std::endl;
            request.graph = std::make_shared<Tree>(n);
            for (int i = 0; i < m; ++i) {
                int u, v;
                input >> u >> v;
                request.graph->addEdge(u, v);
            }
            result << "Graph created with " << n << " vertices and " << m << " edges.\n";
        } else if (cmd == "prim") {
            std::cout << "Executing Prim's MST algorithm for client " << request.clientFd << std::endl;
            auto mst = request.graph->primMST();
            result << "MST created using Prim.\n";
        } else if (cmd == "kruskal") {
            std::cout << "Executing Kruskal's MST algorithm for client " << request.clientFd << std::endl;
            auto mst = request.graph->kruskalMST();
            result << "MST created using Kruskal.\n";
        } else {
            std::cout << "Unknown command received: " << cmd << " from client " << request.clientFd << std::endl;
            result << "Unknown command.\n";
        }

        // Send response back to client
        std::string response = result.str();
        send(request.clientFd, response.c_str(), response.size(), 0);
        std::cout << "Response sent to client " << request.clientFd << ": " << response << std::endl;
    }
}

void handleClient(int fd, Tree& graph) {
    char buffer[1024];

    while (true) {
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            std::cout << "Client disconnected: " << fd << std::endl;
            close(fd);
            return;
        }

        buffer[bytesRead] = '\0';
        std::string command(buffer);
        std::cout << "Received command from client " << fd << ": " << command << std::endl;

        // Push request to pipeline
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            requestQueue.push({fd, command, std::make_shared<Tree>(graph)});
        }
        queueCV.notify_one();
    }
}

int main() {
    Reactor reactor;
    Tree graph(0);

    // Thread pool setup
    int numThreads = 4; // Adjust as needed
    std::vector<std::thread> workers;
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back(pipelineWorker);
        std::cout << "Worker thread " << i << " started." << std::endl;
    }

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;              // Address family
    serverAddr.sin_port = htons(PORT);            // Port number (network byte order)
    serverAddr.sin_addr.s_addr = INADDR_ANY;      // Bind to any available address

    if (bind(listener, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(listener, 10) < 0) {
        perror("Listen failed");
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "..." << std::endl;

    reactor.addFd(listener, [&](int fd) {
        int client = accept(fd, nullptr, nullptr);
        if (client < 0) {
            perror("Accept failed");
            return;
        }
        std::cout << "New client connected: " << client << std::endl;
        reactor.addFd(client, [&](int clientFd) {
            handleClient(clientFd, graph);
        });
    });

    reactor.start();

    // Graceful shutdown
    serverRunning = false;
    queueCV.notify_all();
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
        std::cout << "Worker thread joined." << std::endl;
    }

    close(listener);

    std::cout << "Server shutdown complete." << std::endl;

    return 0;
}
