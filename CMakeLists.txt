cmake_minimum_required(VERSION 3.16)
project(ChessClient LANGUAGES CXX)

option(MAKE_TEST "Build test executable" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Recursive globbing for source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Find Boost (required for Boost.Beast)
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# Find OpenSSL (required for SSL/TLS with Boost.Beast)
find_package(OpenSSL REQUIRED)

# Fetch SFML v3
include(FetchContent)

# Configure SFML build options before declaring
set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_GRAPHICS ON)
set(SFML_BUILD_WINDOW ON)
set(SFML_BUILD_SYSTEM ON)
set(SFML_BUILD_NETWORK OFF)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0-rc.1
    GIT_SHALLOW ON
    SYSTEM)

FetchContent_MakeAvailable(SFML)

# Main executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-graphics 
    sfml-audio
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

if(NOT MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out
)

# Test executable (optional)
if(EXISTS ${CMAKE_SOURCE_DIR}/test/playground.cpp AND MAKE_TEST)
    add_executable(chess_test playground.cpp)
    target_include_directories(chess_test PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
    )
    target_link_libraries(chess_test PRIVATE 
        sfml-graphics 
        sfml-audio
        Boost::system
    )
endif()