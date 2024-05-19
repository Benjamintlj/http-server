/**
 * @file main.c
 * @brief This mini-project uses UNIX based TCP sockets to build a simple HTTP server.
 * @author Ben Lewis-Jones, Lewis Heath
 * @version 1.0
 * @date 2024-05-19
 */

/////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h> // includes the sockaddr_in struct
#include <string.h>
#include <arpa/inet.h>

/////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////
#define PORT 8080

/////////////////////////////////////////////////////////////
// Method Headers
/////////////////////////////////////////////////////////////
/// @brief fills buffer with file contents
/// @param filename name of file
/// @param buff allocated memory
/// @param buffer_size size of allocated memory
/// @return 0 for success, -1 for failure
int get_file(const char *filename, char *buff, size_t buffer_size);

/// @brief Creates and manages a TCP socket, using HTTP
/// @section sec_Description
///  The function main() is responsible for performing the following tasks:
///   - Socket creation
///   - Binding the socket to a specific port
///   - Listen for a connection
///   - Sending an HTTP message containing the desired content
///  @return int Return status of the main function
int main();

/////////////////////////////////////////////////////////////
// Method Definitions
/////////////////////////////////////////////////////////////
int get_file(const char *filename, char *buff, size_t buffer_size) {
    FILE *file;
    size_t i = 0;
    int character;

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("cannot open file");
        return -1;
    }

    while ((character = fgetc(file)) != EOF && i < buffer_size - 1) {
        buff[i++] = (char)character;
    }
    buff[i] = '\0';

    if (ferror(file)) {
        perror("something went wrong while reading the file");
        return -1;
    }

    fclose(file);
    return 0;
}

int main() {
    // 1. create the socket
    const int address_family = AF_INET; // means we are going to use IP, since we are building on TCP
    const int type = SOCK_STREAM;       // means we are going to use TCP
    const int protocol = 0;             // the number indicates the variation of the socket type, 0 is selected since there are no variations

    int server_file_descriptor; // socket that listens for incomming connections
    int other_sockets;          // sockets that handle clients

    if ((server_file_descriptor = socket(address_family, type, protocol)) < 0) {
        perror("failed to create socket\n");
        exit(EXIT_FAILURE);
    }

    // 2. bind the socket to a port
    struct sockaddr_in addr;
    int size_of_addr = sizeof(addr);

    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = address_family;         // we are using IP protocol
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // let OS select the IP, 0 for OS to select
    addr.sin_port = htons(PORT);              // pass PORT num, INADDR_ANY aka 0.0.0.0, for OS to select
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

    while (1) {
        printf("listening...\n");

        if ((other_sockets = accept(server_file_descriptor, (struct sockaddr *)&addr, (socklen_t *)&size_of_addr)) < 0) {
            perror("failed to accept");
            exit(EXIT_FAILURE);
        }

        // 4. recieve a request
        char buffer[1024] = {0};

        if (read(other_sockets, buffer, sizeof(buffer)) < 0) {
            printf("nothing to be sent\n");
        }

        // 4a. tokenise and extract file_path
        char *split = strtok(buffer, "\r\n");
        char *split_again = strtok(split, " ");
        char *file_to_get = strtok(NULL, " ");
        file_to_get[strlen(file_to_get)] = '\0';
        const char *foldername = "server/views";
        char *file_path = (char *)malloc(strlen(foldername) + strlen(file_to_get) + 1);
        sprintf(file_path, "%s%s", foldername, file_to_get);

        printf("Getting file %s\n", file_path);

        // 5. generate response
        size_t buffer_size = 2048;
        char buff[buffer_size];
        char response[4096];

        if (get_file(file_path, buff, buffer_size) == 0) {
            ////////////////////
            // RESPONSE 200
            ////////////////////
            printf("200\n");
            int content_length = strlen(buff);
            char *init_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
            sprintf(response, "%s%d\r\n\r\n%s", init_header, content_length, buff);
        } else if (strlen(file_to_get) < 2) {
            ////////////////////
            // RESPONSE 301
            ////////////////////
            printf("301\n");
            strcpy(response, "HTTP/1.1 301 MOVED PERMANENTLY\r\nLocation: /index.html\r\n\r\n");
        } else {
            ////////////////////
            // RESPONSE 404
            ////////////////////
            printf("404\n");
            strcpy(response, "HTTP/1.1 404 NOT FOUND\r\n\r\n");
        }

        write(other_sockets, response, strlen(response));
        close(other_sockets);
    }

    return 0;
}
