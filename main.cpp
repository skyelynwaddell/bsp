#include "camera3d.h"
#include "bsp.h"
#include "player.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

inline std::unique_ptr<Player> player;
inline bool menu_mode = false;

/*
main
*/
int main()
{
  // init raylib
  SetTargetFPS(60);
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 800, "bsp");
  SetWindowState(FLAG_WINDOW_MAXIMIZED);
  rlEnableBackfaceCulling();

  // create player
  player = std::make_unique<Player>((Vector3){-50.0f, 0.0f, 5.0f});

  // load map
  std::string maptoload = "dm4.bsp";
  try
  {
    models = LoadModelsFromBSPFile(maptoload);
  }
  catch (...)
  {
  }

  // load camera
  DisableCursor();
  Camera3D_Init();

  // Game Loop
  while (!WindowShouldClose())
  {
    // Update
    player->Update(camera.get(), menu_mode);
    Camera3D_Update(menu_mode);

    // Draw
    static bool enable_wireframe = false;
    BeginDrawing();
    ClearBackground(GRAY);
    BeginMode3D(*camera);
    BSP_Draw(camshader, enable_wireframe, camera->position);
    EndMode3D();

    // DrawGUI
    DrawFPS(0, 0);
    BSP_DrawDebug(camera->position);
    EndDrawing();
  }

  // CleanUp
  UnloadShader(camshader);
  BSP_CleanUp();

  // shutdown
  CloseWindow();
  return 0;
};