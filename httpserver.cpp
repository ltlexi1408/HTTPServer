#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string>
#include <cstring>

#define PORT 8080

int main(){

    int HTTPServer;
    char html[1024] = "<!DOCTYPE html><html><body><h1>C++ http server</h1><p>Testing.</p></body></html> ";

    if ((HTTPServer = socket(AF_INET, SOCK_STREAM, 0 )) < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = PORT;
    address.sin_addr.s_addr = INADDR_ANY;

    if (connect(HTTPServer, (struct sockaddr*)&address, sizeof(address)) < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}