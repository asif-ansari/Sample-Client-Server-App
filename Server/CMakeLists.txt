add_executable(Server
    include/Server.h
    src/ServerMain.cpp
    src/MessageReader.cpp
)

target_link_libraries(Server socket-static)
target_link_libraries(Server pthread)
