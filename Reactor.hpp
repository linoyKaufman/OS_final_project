#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <unordered_map>
#include <functional>

class Reactor {
    int epoll_fd; // Epoll instance file descriptor
    std::unordered_map<int, std::function<void(int)>> callbacks;

public:
    Reactor();
    ~Reactor();

    int addFd(int fd, std::function<void(int)> callback);
    int removeFd(int fd);
    void start();
    void stop();

private:
    void eventLoop();
};

#endif
