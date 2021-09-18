add_executable(Client
    ClientMain.cpp
)

target_link_libraries(Client socket-static)
target_link_libraries(Client pthread)
