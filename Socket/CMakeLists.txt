add_library(socket-static STATIC
    include/SocketBase.h
    include/ServerSocket.h
    include/ClientSocket.h
    src/SocketBase.cpp
    src/ServerSocket.cpp
    src/ClientSocket.cpp
)

target_include_directories(socket-static PUBLIC "include")
