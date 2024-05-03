cmake_minimum_required(VERSION 3.5)

# Uncomment this line if not using Qt Creator. Replace the path according to your system
# set(CMAKE_PREFIX_PATH "/home/aditya-kulkarni/Qt/6.6.2/gcc_64/")

project(qt-network-test LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Network REQUIRED)

# Include directories for server and client code
include_directories(
    ${PROJECT_SOURCE_DIR}/server
    ${PROJECT_SOURCE_DIR}/client
)

# Server executable
add_executable(server
    server_main.cpp
    server/server.hpp
    server/server.cpp
    payload/Message.h
    Node.h
)

# Client executable
add_executable(client
    client_main.cpp
    client/client.hpp
    client/client.cpp
    payload/Message.h
    Node.h
)

# Installation settings for server
include(GNUInstallDirs)
install(TARGETS server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Installation settings for client
install(TARGETS client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Link libraries to server
target_link_libraries(server Qt6::Core Qt6::Network)

# Link libraries to client
target_link_libraries(client Qt6::Core Qt6::Network)
