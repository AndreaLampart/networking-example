cmake_minimum_required(VERSION 2.8.11)
project(networking_intro CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -march=native")

find_package(X11 REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

add_executable(udp_client
  udp_client.cpp
)
add_executable(tcp_client
  tcp_client.cpp
)
add_executable(udp_server
  udp_server.cpp
)
add_executable(tcp_server
  tcp_server.cpp
)

target_link_libraries(tcp_server 
PRIVATE
  ${X11_LIBRARIES}
)
target_link_libraries(tcp_client 
PRIVATE
  ${X11_LIBRARIES}
)

