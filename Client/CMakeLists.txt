add_executable(Client
    ClientMain.cpp
)

list(APPEND includePath "${CMAKE_SOURCE_DIR}/Socket/include" "${CMAKE_SOURCE_DIR}/Server/include")

target_include_directories(Client PUBLIC "$<BUILD_INTERFACE:${includePath}>")

target_link_libraries(Client socket-static)
target_link_libraries(Client pthread)
