cmake -DCMAKE_GENERATOR_PLATFORM=x64 -S ./src -B ./build
cmake --build ./build --config Release
copy .\res .\build\bin
pause