add_executable(Server
    include/MessageReader.h
    include/locklessQ.hpp
    src/ServerMain.cpp
    src/MessageReader.cpp
)

list(APPEND includePath "${CMAKE_SOURCE_DIR}/Server/include" "${CMAKE_SOURCE_DIR}/Socket/include")

target_include_directories(Server PUBLIC "$<BUILD_INTERFACE:${includePath}>") 

target_link_libraries(Server socket-static)
target_link_libraries(Server pthread)
