cmake_minimum_required(VERSION 3.10)

include(FindProtobuf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

find_library(SQLite3 REQUIRED)
include_directories(SQLITE3_INCLUDE_DIR)

find_package(Protobuf REQUIRED)

find_library(pthreads REQUIRED)


add_subdirectory(sockets)
include_directories(sockets/include)
include_directories(${CMAKE_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS Card.proto)
add_executable(Server server.cpp User.cpp ConnectServer.cpp ConnectedUsers.cpp ${PROTO_SRCS})
target_link_libraries(Server sockets sqlite3 pthread)
target_link_libraries(Server ${PROTOBUF_LIBRARIES}) 

add_executable(Game game.cpp ${PROTO_SRCS})
target_link_libraries(Game sockets pthread ${PROTOBUF_LIBRARIES})
