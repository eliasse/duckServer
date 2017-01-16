cmake_minimum_required(VERSION 3.6)
project(asyncServer)

set(CMAKE_CXX_STANDARD 11)
set(SERVER_SOURCE_FILES Server.cpp SimpleSerial.cpp serialSession.cpp chatServer.cpp configReader.hpp)
set(CLIENT_SOURCE_FILES chatClient.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES})
    add_executable(asyncServer ${SERVER_SOURCE_FILES})
    add_executable(asyncClient ${CLIENT_SOURCE_FILES})
    target_link_libraries(asyncServer ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} Threads::Threads)
    target_link_libraries(asyncClient ${Boost_LIBRARIES} Threads::Threads)
endif()