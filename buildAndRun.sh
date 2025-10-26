#!/bin/bash

# Build and Run Script for Linux using CMake
# This script builds and runs the Chess Client

echo "Configuring project with CMake..."

# Configure the project
cmake -S . -B build

# Check if configuration was successful
if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

echo "Building project..."

# Build the project
cmake --build build

# Check if build was successful
if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful!"
    echo "Running Chess Client..."
    echo "------------------------"
    ./build/ChessClient
else
    echo "Build failed!"
    exit 1
fi
