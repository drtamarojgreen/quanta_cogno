# Build Instructions

## Requirements
- C++17 compliant compiler (GCC 8+, Clang 5+, MSVC 2017+)
- `make` (optional, for automation)

## Windows (MinGW/GCC)
```powershell
g++ -std=c++17 src/core/*.cpp src/io/*.cpp src/visualization/*.cpp src/app/*.cpp src/main.cpp -Isrc -o build/quanta_cogno_v2.exe -pthread
```

## Linux / macOS
```bash
g++ -std=c++17 src/core/*.cpp src/io/*.cpp src/visualization/*.cpp src/app/*.cpp src/main.cpp -Isrc -o build/quanta_cogno_v2 -pthread
```

## Running Tests
```bash
g++ -std=c++17 src/core/*.cpp src/io/*.cpp src/visualization/*.cpp src/app/*.cpp tests/unit/**/*.cpp tests/bdd/*.cpp tests/e2e/*.cpp src/test_runner_main.cpp -Isrc -o build/test_suite -pthread
./build/test_suite
```
