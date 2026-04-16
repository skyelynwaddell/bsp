# All in one BSP file for Raylib
## TODO: Wall collisions are a bit jank

## Build
```bash
# Build the example
mkdir build-win && cd build-win
cmake -G "Ninja" ..
ninja
cd ..

# Run the example from root folder
./build-win/game.exe
```