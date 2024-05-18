#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h> // includes the sockaddr_in struct
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

/*
    struct sockaddr_in {
    __uint8_t       sin_len;
    sa_family_t     sin_family; // AF_INET
    in_port_t       sin_port; // 0 for OS to select
    struct  in_addr sin_addr; // INADDR_ANY aka 0.0.0.0, for OS to select
    char            sin_zero[8];
    };
*/

int main(){
    // 1. create the socket
    const int address_family = AF_INET;     // means we are going to use IP, since we are building on TCP
    const int type = SOCK_STREAM;           // means we are going to use TCP
    const int protocol = 0;                 // the number indicates the variation of the socket type, 0 is selected since there are no variations
    
    int server_file_descriptor;             // socket that listens for incomming connections
    int other_sockets;                      // sockets that handle clients
    
    if ((server_file_descriptor = socket(address_family, type, protocol)) < 0) {
        perror("failed to create socket\n");
        exit(EXIT_FAILURE);
    }
    
    // 2. bind the socket to a port
    struct sockaddr_in addr;
    int size_of_addr = sizeof(addr);

    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = address_family;           // we are using IP protocol
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   // let OS select the IP
    addr.sin_port = htons(PORT);                // pass PORT num
    memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

    if (bind(server_file_descriptor, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("binding failed");
        exit(EXIT_FAILURE);
    }

    // 3. listen for a connection
    if (listen(server_file_descriptor, 3) < 0) {
        perror("failed to listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("listening...\n");

        if ((other_sockets = accept(server_file_descriptor, (struct sockaddr *)&addr, (socklen_t*)&size_of_addr)) < 0) {
            perror("failed to accept");
            exit(EXIT_FAILURE);
        }

        // 4. send and recieve messages
        char buffer[1024] = {0};
        printf("%s\n", buffer);

        if (read(other_sockets, buffer, sizeof(buffer)) < 0) {
            printf("nothing to be sent\n");
        }

        char *msg = "this is the server speaking, can you hear me?";
        write(other_sockets, msg, strlen(msg));

        printf("message sent\n");

        close(other_sockets);
    }
    
    return 0;
}
