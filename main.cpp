#include "server.h"

int main(){
    Server http(8080);

    http.Start();

    return 0;
}