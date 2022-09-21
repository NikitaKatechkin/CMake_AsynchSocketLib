cmake_minimum_required(VERSION 3.23.0)

project(AsynchClient C CXX)

set(Headers 
    ./include/Client.h
)

set(Sources 
    ./src/Source.cpp
    ./src/Client.cpp
)

add_subdirectory(../AsynchLib ../AsynchLib)

add_library(${PROJECT_NAME} STATIC ${Sources})

target_include_directories(${PROJECT_NAME}
    PUBLIC ./include
    PUBLIC ../AsynchLib/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE ./src
    PRIVATE ../AsynchLib/src
)

target_link_libraries(${PROJECT_NAME} AsynchLib)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)