#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h> 

#include <iostream>
#include <string> 
  
#define PORT    8080 
#define MAXLINE 1024 
  
// Driver code 
int main() {
    using SocketPtr = struct sockaddr*;
    using ConstSocketPtr = const struct sockaddr*;
    using ServerAddress = struct sockaddr_in;

    // Filling server information 
    ServerAddress servaddr;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET; 

    // Creating socket file descriptor 
    int sender_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sender_fd < 0) { std::cout << "Socket creation failed..." << std::endl; return -1; } 

    // Connect to server
    if(connect(sender_fd, (SocketPtr) &servaddr, sizeof(servaddr)) < 0) 
    { 
        std::cout << "Error : Connection Failed" << std::endl;
        return -1;
    }

    // Send data to server
    std::string message = "Hello server. I'm a message from the client."; 
    const char* msg = message.c_str();
    sendto(sender_fd, msg, MAXLINE, MSG_CONFIRM, (ConstSocketPtr) &servaddr, sizeof(servaddr)); 
    
    // Receive data from server
    unsigned int len = sizeof(servaddr);
    char buffer[MAXLINE]; 
    int n = recvfrom(sender_fd, buffer, sizeof(buffer), MSG_WAITALL, (SocketPtr) &servaddr, &len); 
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    close(sender_fd); 
    return 0; 
} 