# Networking
## User Datagram Protocol (UDP)
Doesn't need to establish a connection and messages are served as fast as they arrive. Therefore udp is to be used wherever bandwidth is a bottleneck or arriving data doesn't need to be complete.

## Transmission Control Protocol (TCP)
Establishes a connection between server and client. During that session, data can be sent back and forth. The package drop checks are costly. Whenever one needs to make sure that the data arrives one chooses tcp.

# Build and Run
Create a new directory and build with cmake:
```cmake -S . -B build && cd build && make -j```

## Run TCP example
Start two terminals, then run ```./build/tcp_server``` and ```./build/tcp_client```.

## Run UDP example
Start two terminals, then run ```./build/udp_server``` and ```./build/udp_client```.