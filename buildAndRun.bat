@echo off
REM Build and Run Script for Windows using CMake
REM This script builds and runs the Chess Client

echo Configuring project with CMake...

REM Configure the project
cmake -S . -B build

REM Check if configuration was successful
if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo Building project...

REM Build the project
cmake --build build

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo Running Chess Client...
    echo ------------------------
    build\Debug\ChessClient.exe
    if %ERRORLEVEL% NEQ 0 (
        build\Release\ChessClient.exe
        if %ERRORLEVEL% NEQ 0 (
            build\ChessClient.exe
        )
    )
) else (
    echo Build failed!
    pause
    exit /b 1
)

pause
