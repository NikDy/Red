# Red

run Build.bat to compile executable

or

```
conan remote add sfml:bincrafters https://api.bintray.com/conan/bincrafters/public-conan 
conan install <src> -if <build> --build missing
cmake -S <src> -B <build>
cmake --build <build> --config Release

copy all from res to build/bin
```
visual studio 2019 build tools may be required
https://visualstudio.microsoft.com/ru/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16


```
usage run Red.exe in build/bin
```
```
contols:
 W - zoom in
 S - zoom out
 Arrows - camera movement
 ```



#### Red team:
- Nikita Dyrkov - https://github.com/NikDy
- Ivan Matesha - https://github.com/seillich
- Yulia Kurilenko - https://github.com/titatititatita
