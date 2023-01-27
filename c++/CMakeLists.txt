
cmake_minimum_required(VERSION 3.10)

project(SocketExamples)

add_executable(UDPServer UDPServer.cpp)
add_executable(UDPClient UDPClient.cpp)
#add_executable(TCPServer TCPServer.cpp)
add_executable(TCPClient TCPClient.cpp)
