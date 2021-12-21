#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>
#include <cstring>

#define PORT 8080
#define MAXLINE 1024 

void func(int sockfd) 
{ 
    int n; 
    char buff[MAXLINE];
    while (true) { 
        bzero(buff, sizeof(buff));
        printf("Enter the string : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') ; 
        write(sockfd, buff, sizeof(buff)); 
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 
        printf("From Server : %s", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break;
        } 
    } 
} 
  
int main(int argc, char *argv[]) {
    using SocketPtr = struct sockaddr*;
    using ServerAddress = struct sockaddr_in;
  
    // Filling server information 
    ServerAddress servaddr;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    
    // Create the socket
    int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (client_socket_fd < 0) { std::cout << "Socket creation failed..." << std::endl; return -1; }
  
  
    // connect the client socket to server socket 
    if (connect(client_socket_fd, (SocketPtr) &servaddr, sizeof(servaddr)) < 0) { 
        std::cout << "Connection with the server failed..." << std::endl; 
        return -1; 
    } 
    else {
        std::cout << "Connection with the server succeeded..." << std::endl; 
    }
  
    // function for chat 
    func(client_socket_fd); 
  
    // close the socket 
    close(client_socket_fd);
    return 0;
} 