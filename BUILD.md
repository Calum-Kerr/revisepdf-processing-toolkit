# Build Instructions

## Windows Local Build

### Prerequisites
- Visual Studio 2022 or Build Tools
- CMake 3.20+
- vcpkg

### Setup vcpkg
```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

### Build
```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

### Run
```
cd ..
.\build\Release\revisepdf.exe
```

Visit http://localhost:8080

## Heroku Deployment

### Prerequisites
- Heroku CLI
- Docker

### Deploy
```
heroku login
heroku create your-app-name
heroku stack:set container
git push heroku main
```

The app will bind to the PORT environment variable automatically.

## WASM Build

Requires Emscripten SDK installed.

```
.\scripts\build-wasm.ps1
```

This generates frontend/algorithms.js and frontend/algorithms.wasm

