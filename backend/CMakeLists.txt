cmake_minimum_required(VERSION 3.10.0)
project(backend VERSION 0.1.0 LANGUAGES C CXX)

find_package(SQLite3 REQUIRED)

include(CTest)
enable_testing()


include_directories(${CMAKE_SOURCE_DIR}/dependencies/Crow/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/asio/asio/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/sqllite3)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/Crow/include/crow/middlewares)

set(SQLITE3_SRC ${CMAKE_SOURCE_DIR}/db/sqlite.cpp)
set(CMAKE_OSX_ARCHITECTURES "arm64")

add_executable(backend main.cpp ${SQLITE3_SRC})



set_property(TARGET backend PROPERTY CXX_STANDARD 17)
set_property(TARGET backend PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET backend PROPERTY CXX_EXTENSIONS OFF)
target_link_libraries(backend SQLite::SQLite3)