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



```
usage: Graph_drawer <path_to_json.json>
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
