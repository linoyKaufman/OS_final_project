#include "Reactor.hpp"
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <functional>

// Initializes the Reactor and creates an epoll instance
Reactor::Reactor() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        exit(1);
    }
}

// Destructor: Closes the epoll instance to release resources
Reactor::~Reactor() {
    close(epoll_fd);
}

// Adds a file descriptor to the epoll instance and associates it with a callback
int Reactor::addFd(int fd, std::function<void(int)> callback) {
    struct epoll_event ev;
    ev.events = EPOLLIN; // Monitor for read events
    ev.data.fd = fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        perror("epoll_ctl");
        return -1;
    }

    callbacks[fd] = callback;
    return 0;
}

// Removes a file descriptor from the epoll instance
int Reactor::removeFd(int fd) {
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) < 0) {
        perror("epoll_ctl");
        return -1;
    }

    callbacks.erase(fd);
    return 0;
}

// Starts the event loop to handle events
void Reactor::start() {
    eventLoop();
}

// Stops the Reactor by closing the epoll file descriptor
void Reactor::stop() {
    close(epoll_fd);
}

// Main event loop: Monitors file descriptors and invokes their callbacks on events
void Reactor::eventLoop() {
    struct epoll_event events[10];

    while (true) {
        int n = epoll_wait(epoll_fd, events, 10, -1);
        if (n < 0) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; ++i) {
            int fd = events[i].data.fd;
            if (callbacks.find(fd) != callbacks.end()) {
                callbacks[fd](fd); // Invoke the callback for the file descriptor
            }
        }
    }
}
