#!/bin/bash
# Build script for Linux (WSL/Ubuntu)

set -e

echo "MDU2 Linux Build Script"
echo "======================"

# Check if we're on Linux
if [[ "$OSTYPE" != "linux-gnu"* ]]; then
    echo "Warning: This script is designed for Linux systems"
fi

# Install dependencies if needed
echo "Checking dependencies..."
if ! dpkg -l | grep -q libx11-dev; then
    echo "Installing X11 development libraries..."
    sudo apt update
    sudo apt install -y libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxss-dev libxxf86vm-dev
fi

if ! command -v cmake &> /dev/null; then
    echo "Installing CMake..."
    sudo apt install -y cmake
fi

if ! command -v make &> /dev/null; then
    echo "Installing build tools..."
    sudo apt install -y build-essential
fi

# Create build directory
echo "Setting up build directory..."
mkdir -p build_linux
cd build_linux

# Configure with CMake
echo "Configuring project..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "Building project..."
make -j$(nproc)

echo "Build completed successfully!"
echo "Executable: ./mdu"
echo ""
echo "To run the application:"
echo "  cd build_linux && ./mdu"