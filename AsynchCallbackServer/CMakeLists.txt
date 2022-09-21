cmake_minimum_required(VERSION 3.23.0)

project(AsynchCallbackServer C CXX)

set(Headers 
    ./include/CallbackServer.h
)

set(Sources 
    ./src/Source.cpp
    ./src/CallbackServer.cpp
)

add_subdirectory(../AsynchServer ../AsynchServer)

add_executable(${PROJECT_NAME} ${Sources})

target_include_directories(${PROJECT_NAME}
    PUBLIC ./include
    PUBLIC ../AsynchServer/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE ./src
    PRIVATE ../AsynchServer/src
)

target_link_libraries(${PROJECT_NAME} AsynchServer)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)