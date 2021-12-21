#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>
#include <cstring>

#define PORT 8080

class TCPServer
{
    using SocketPtr = struct sockaddr*;
    using ServerAddress = struct sockaddr_in;
    using ClientAddress = struct sockaddr_in; 

public:
    TCPServer();
    ~TCPServer();

    /**
     * @brief Server is up.
     */
    bool OK() const { return server_up_; }

    /**
     * @brief Receive some data from a client
     */
    std::pair<char*, unsigned int> Receive();

    /**
     * @brief Send data to a client
     */
    bool Send(char* data, unsigned int size);

private:
    static constexpr int kBytes = 1024;
    bool server_up_ = false;
    int listen_fd_;
    int connection_fd_;
    char buffer_[kBytes];
    ServerAddress address_;
};

TCPServer::TCPServer()
{
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ == -1) {
        std::cout << "Socket creation failed..." << std::endl;
        return;
    }

    // Set server address
    address_.sin_addr.s_addr = htonl(INADDR_ANY); 
    address_.sin_family = AF_INET; 
    address_.sin_port = htons(PORT);
    if (listen_fd_ != -1 && bind(listen_fd_, (SocketPtr)&address_, sizeof(address_)) != -1) {
        if(listen(listen_fd_, 10) == -1) {
            std::cout << "Listening for connections failed..." << std::endl;
        }
    } else {
        std::cout << "Binding to socket failed..." << std::endl;
        return;
    }

    // Accept the data packet from client and verification 
    ClientAddress cli; 
    unsigned int len = sizeof(cli);
    connection_fd_ = accept(listen_fd_, (SocketPtr)&cli, &len); 
    if (connection_fd_ == -1) { 
        std::cout << "Server accept failed..." << std::endl;
        return; 
    } else {
        std::cout << "Server accepted the client..." << std::endl; 
    }
    server_up_ = true;
}

TCPServer::~TCPServer()
{
    close(listen_fd_);
}

std::pair<char*, unsigned int> TCPServer::Receive() { 
    bzero((void *)buffer_, sizeof(buffer_));
    read(connection_fd_, (void *)buffer_, sizeof(buffer_));
    return {buffer_, sizeof(buffer_)};
}

bool TCPServer::Send(char* data, unsigned int size) {
    // and send that buffer to client 
    printf("Sending...\n"); 
    int err = write(connection_fd_, data, size);
    printf("Sent...\n"); 

    // if msg contains "Exit" then server exit and chat ended. 
    if (err == -1 || strncmp("exit", (const char*)data, 4) == 0) { 
        printf("Server Exit...\n"); 
        server_up_ = false;
        return false;
    }
    return true;
}
  
int main(int argc, char *argv[]) {
    // Create tcp server
    TCPServer server;
    
    char buff[1024];
    while (server.OK()) {
        int n = 0; 
        // Receive data
        auto [buffer, buffer_bytes] = server.Receive();
        (void) buffer_bytes;
        printf("From client: %s\t To client : ", (const char*)buffer); 

        // Send data
        bzero(buff, sizeof(buff));
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') ;
        server.Send(buff, sizeof(buff));
    }
} 