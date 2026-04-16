# BSP Loader for Raylib

- Load & Parse .BSP files
- Render the maps, with optional PVS culling
- Lightmap Support
- Collision & Movement Functions
- Player Example

## TODO LIST:

```c
- Wall collisions are pretty jank
- Make a pure C version?
```

## Build

```bash
# Build the example
mkdir build && cd build
cmake -G "Ninja" ..
ninja
cd ..

# Run the example from root folder
./build-win/game.exe
```
