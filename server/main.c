#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){
    
    const int domain = AF_INET; // means we are going to use IP, since we are building on TCP
    const int type = SOCK_STREAM; // means we are going to use TCP
    const int protocol = 0; // the number indicates the variation of the socket type, 0 is selected since there are no variations
    
    int server_descipter;
    
    if ((server_descipter = socket(domain, type, protocol)) < 0) {
        perror("failed to create socket");
        return -1;
    }
    
    return 0;
}
