cmake_minimum_required(VERSION 3.7)

if(${CMAKE_VERSION} VERSION_LESS 3.21)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.21)
endif()

project(PsyQuant VERSION 0.1
            DESCRIPTION "Client Server App"
            LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -g")

add_subdirectory(Socket)
add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(Test)
