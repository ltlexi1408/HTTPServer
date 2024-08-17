#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string>
#include <cstring>
#include <unistd.h>

#define PORT 8080

int main(){

    int HTTPServer;
    char html[1024] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><body><h1>C++ http server</h1><p>Testing.</p></body></html> ";

    if((HTTPServer = socket(AF_INET, SOCK_STREAM, 0 )) < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if(bind(HTTPServer, (struct sockaddr*)&address, sizeof(address)) < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    if(listen(HTTPServer, 5) < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    socklen_t addressLength = sizeof(address);
    int new_request = accept(HTTPServer, (struct sockaddr*)&address, &addressLength);
    
    if(new_request < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    if(send(new_request, html, strlen(html), 0) < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    close(new_request);
    close(HTTPServer);

    return 0;
}