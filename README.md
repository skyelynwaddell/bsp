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

### Parse BSP Entities

```c
// Parse Entities from BSP
void SpawnBSPEntities()
{
  if (!bsp_renderer.bsp_file)
    return;

  // ref to all entities in BSP file
  auto entities = bsp_renderer.bsp_file->entities();

  for (auto &e : entities)
  {
    // find all entities with a classname (ie. info_player_start)
    auto class_it = e.tags.find("classname");
    if (class_it == e.tags.end())
      continue;

    // get classname
    const std::string &classname = class_it->second;

    // parse origin
    Vector3 origin = {0, 0, 0};
    auto org = e.tags.find("origin");
    if (org != e.tags.end())
    {
      float qx = 0, qy = 0, qz = 0;
      sscanf(org->second.c_str(), "%f %f %f", &qx, &qy, &qz);
      origin = FromQuake({qx, qy, qz});
    }

    // parse other fields here....

    // Now do something with the data, spawn an instance, etc.
  }
};
```
