#include "server.h"

Server::Server(int port){
    this->port = port;
}

int Server::Start(){
    if((this->HTTPServer = socket(AF_INET, SOCK_STREAM, 0 )) < 0){
        std::cerr << strerror(errno) << std::endl;
    }

    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(this->port);
    this->address.sin_addr.s_addr = INADDR_ANY;

    if(bind(this->HTTPServer, (struct sockaddr*)&this->address, sizeof(this->address)) < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    if(listen(this->HTTPServer, 5) < 0){
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    while(true){
        socklen_t addressLength = sizeof(this->address);
        int new_request = accept(this->HTTPServer, (struct sockaddr*)&this->address, &addressLength);
        
        if(new_request < 0){
            std::cerr << strerror(errno) << std::endl;
            close(new_request);
        }

        std::thread http(Server::Request, new_request);
        http.detach();
    }

    close(HTTPServer);
    return 0;
}

int Server::Request(int new_request){
    char requestBytes[4096];
    ssize_t requestRecived = recv(new_request, requestBytes, 4096 - 1, 0);

    if(requestRecived < 0){
        std::cerr << strerror(errno) << std::endl;
        close(new_request);
        return -1;
    }

    std::string request(requestBytes, requestRecived);
    std::size_t fplace = request.find(" ");
    std::size_t splace = request.find(" ", fplace + 1);
    std::string requestFile = request.substr(fplace + 1, splace - fplace - 1);

    if(!requestFile.compare("/")){
        requestFile = "/index.html";
    }

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
        return -1;
    }

    close(new_request);
    return 0;
}