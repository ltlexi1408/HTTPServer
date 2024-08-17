#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string>
#include <cstring>
#include <unistd.h>
#include <csignal>

#define PORT 8080

int HTTPServer;

void signalHandler(int signal){

    if(HTTPServer >= 0){
        close(HTTPServer);
        std::cout << "Closing Server" << std::endl;
    }

    exit(signal);
}

int main(){

    std::signal(SIGINT, signalHandler);

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

    while(true){
        socklen_t addressLength = sizeof(address);
        int new_request = accept(HTTPServer, (struct sockaddr*)&address, &addressLength);
        
        if(new_request < 0){
            std::cerr << strerror(errno) << std::endl;
            close(new_request);
        }

        char requestBytes[4096];
        ssize_t requestRecived = recv(new_request, requestBytes, 4096 - 1, 0);

        if(requestRecived < 0){
            std::cerr << strerror(errno) << std::endl;
            close(new_request);
        }

        std::string request(requestBytes, requestRecived);
        std::size_t fplace = request.find(" ");
        std::size_t splace = request.find(" ", fplace + 1);
        std::string requestFile = request.substr(fplace + 1, splace - fplace - 1);

        std::string html = "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n";
        std::ifstream file("." + requestFile);

        if(!file.is_open()){
            html += "404 Forbiden";
            std::cerr << strerror(errno) << std::endl;
        } else{
            std::string text;
            while(getline(file, text)){
                html += text + "\n";
            }
        }
      
        if(send(new_request, html.c_str(), html.length(), 0) < 0){
            std::cerr << strerror(errno) << std::endl;
            close(new_request);
        }

        close(new_request);
    }

    close(HTTPServer);

    return 0;
}