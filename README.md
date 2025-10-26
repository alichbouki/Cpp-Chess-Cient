# Cpp-Chess-Client

A chess client application where you can play against Stockfish chess engine. This is a school project built with C++.

## Features

- Chess board representation and management
- FEN (Forsyth-Edwards Notation) support for board state serialization
- Integration with Stockfish chess engine
- Cross-platform support (Linux and Windows)

## Prerequisites

Before building the project, make sure you have the following installed:

- **CMake** (version 3.16 or higher)
- **C++ Compiler** with C++17 support:
  - Linux: GCC or Clang
  - Windows: MSVC, MinGW, or Clang
- **Stockfish** chess engine (for bot functionality)

## Project Structure

```
Cpp-Chess-Client/
├── src/
│   ├── Core/          # Core chess logic and board management
│   │   ├── Chess.cpp
│   │   └── Chess.hpp
│   ├── Bot/           # Stockfish integration
│   │   ├── botHandler.cpp
│   │   ├── botHandler.hpp
│   │   └── stfInterface.cpp
│   └── GUI/           # GUI handler
│       └── guiHandler.hpp
├── main.cpp           # Main entry point
├── test.cpp           # Test file
├── CMakeLists.txt     # CMake build configuration
├── buildAndRun.sh     # Linux build script
└── buildAndRun.bat    # Windows build script
```

## Building the Project

### Using CMake (Recommended)

#### Linux / macOS

1. **Configure the project:**
   ```bash
   cmake -S . -B build
   ```

2. **Build the project:**
   ```bash
   cmake --build build
   ```

3. **Run the executable:**
   ```bash
   ./build/ChessClient
   ```

#### Windows

1. **Configure the project:**
   ```cmd
   cmake -S . -B build
   ```

2. **Build the project:**
   ```cmd
   cmake --build build
   ```

3. **Run the executable:**
   ```cmd
   build\Debug\ChessClient.exe
   ```
   or
   ```cmd
   build\Release\ChessClient.exe
   ```

### Using Build Scripts

For convenience, build and run scripts are provided:

#### Linux / macOS
```bash
chmod +x buildAndRun.sh
./buildAndRun.sh
```

#### Windows
```cmd
buildAndRun.bat
```

### Building the Test Executable

The test executable is built automatically if `test.cpp` exists:

```bash
# Run test
./build/chess_test
```

## CMake Options

You can customize the build with CMake options:

```bash
# Debug build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Release build (optimized)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Specify a different compiler
cmake -S . -B build -DCMAKE_CXX_COMPILER=g++
```

## Clean Build

To clean the build directory:

```bash
# Linux/macOS
rm -rf build/

# Windows
rmdir /s /q build
```

## Development

The project uses C++17 standard with strict compiler warnings enabled. The CMakeLists.txt automatically:
- Discovers all source files in the `src/` directory
- Configures proper include paths
- Enables compiler warnings (-Wall, -Wextra, -Wpedantic)

## License

This project is for educational purposes.
