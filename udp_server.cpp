#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <string>

#define PORT 8080
#define MAXLINE 1024

int main(int argc, char *argv[]) {
    using SocketPtr = struct sockaddr*;
    using ConstSocketPtr = const struct sockaddr*;
    using ServerAddress = struct sockaddr_in;
    using ClientAddress = struct sockaddr_in;

    // Filling server information 
    ServerAddress servaddr;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET; // IPv4 

    // Creating socket file descriptor 
    int listener_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listener_fd  < 0 ) { std::cout << "socket creation failed"; return -1; }
      
    // Bind the socket with the server address 
    int bound = bind(listener_fd, (ConstSocketPtr) &servaddr, sizeof(servaddr));
    if (bound < 0) { std::cout << "Failed binding to socket..." << std::endl; return -1; }

    // Receive message from client
    ClientAddress cliaddr;
    unsigned int len = sizeof(cliaddr);
    char buffer[MAXLINE];
    int n = recvfrom(listener_fd, buffer, sizeof(buffer), MSG_WAITALL, (SocketPtr) &cliaddr, &len);
    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer);

    // Send message to client
    std::string message = "Hello client. I'm a message from the server.";
    const char* msg = message.c_str();
    sendto(listener_fd, msg, MAXLINE, MSG_CONFIRM, (ConstSocketPtr) &cliaddr, sizeof(cliaddr)); 
    close(listener_fd);

    return 0;
}
