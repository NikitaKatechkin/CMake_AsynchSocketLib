cmake_minimum_required(VERSION 3.23.0)

project(AsynchLib C CXX)

set(Headers
    ./include/IncludeMe.h
    ./include/IPEndpoint.h
    ./include/NetworkInitializer.h
    ./include/ServiceStructs.h
    ./include/Socket.h
)

set(Sources
    ./src/IPEndpoint.cpp
    ./src/NetworkInitializer.cpp
    ./src/Socket.cpp
)

add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})

target_include_directories(${PROJECT_NAME}
    PUBLIC ./include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE ./src
)

target_link_libraries(${PROJECT_NAME} ws2_32)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)