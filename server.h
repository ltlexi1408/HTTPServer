#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string>
#include <cstring>
#include <unistd.h>
#include <csignal>
#include <thread>

class Server{
    public:

    Server(int port){};
    
    int Start(){};

    private:
    int HTTPServer;
    int port;

    struct sockaddr_in address;

    int Request(int new_request){};
};