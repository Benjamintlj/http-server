/**
* @file main.c
* @brief This mini-project uses UNIX based TCP sockets to make a request to a HTTP server.
* @author Ben Lewis-Jones, Lewis Heath
* @version 1.0
* @date 2024-05-19
*/

/////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

/////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////
#define PORT 8080

/////////////////////////////////////////////////////////////
// Method Headers
/////////////////////////////////////////////////////////////

/// @brief Creates a TCP socket, and makes an instat request to a localhost HTTP server.
/// @return int Return status of the main function
int main();

/////////////////////////////////////////////////////////////
// Method Definitions
/////////////////////////////////////////////////////////////
int main(){
    int socket_file_descripter, conn_file_descripter;
    struct sockaddr_in server_addr;

    const int address_family = AF_INET;

    if ((socket_file_descripter = socket(address_family, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed...");
        exit(EXIT_FAILURE);
    }
    
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = address_family;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(address_family, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("unable to gen address for host");
        exit(EXIT_FAILURE);
    }

    if (connect(socket_file_descripter, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("failed to connect with host");
        exit(EXIT_FAILURE);
    }

    char* msg = "GET /index.html HTTP/1.1\r\n\r\n";
    send(socket_file_descripter, msg, strlen(msg), 0);
    printf("sent\n");
    
    char buffer[4096] = {0};
    long response = read(socket_file_descripter, buffer, 4096);
    printf("%s\n", buffer);

    return 0;
}