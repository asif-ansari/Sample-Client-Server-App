add_executable(Server
    include/MessageReader.h
    include/SafeQueue.hpp
    src/ServerMain.cpp
    src/MessageReader.cpp
)

target_link_libraries(Server socket-static)
target_link_libraries(Server pthread)
