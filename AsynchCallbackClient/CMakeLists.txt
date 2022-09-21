cmake_minimum_required(VERSION 3.23.0)

project(AsynchCallbackClient C CXX)

set(Headers 
    ./include/CallbackClient.h
)

set(Sources 
    ./src/Source.cpp
    ./src/CallbackClient.cpp
)

add_subdirectory(../AsynchClient ../AsynchClient)

add_executable(${PROJECT_NAME} ${Sources})

target_include_directories(${PROJECT_NAME}
    PUBLIC ./include
    PUBLIC ../AsynchClient/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE ./src
    PRIVATE ../AsynchClient/src
)

target_link_libraries(${PROJECT_NAME} AsynchClient)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)