conan remote add sfml:bincrafters https://api.bintray.com/conan/bincrafters/public-conan 
conan install ./src -if ./build --build missing

cmake -DCMAKE_GENERATOR_PLATFORM=x64 -S ./src -B ./build
cmake --build ./build --config Release
copy .\res .\build\bin
pause