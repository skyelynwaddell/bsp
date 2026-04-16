#include "camera3d.h"
#include "bsp.h"
#include "player.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

inline std::unique_ptr<Player> player;

/*
Init
*/
static inline void Init()
{
  SetTargetFPS(60);
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 800, "bsp");
  SetWindowState(FLAG_WINDOW_MAXIMIZED);
  rlEnableBackfaceCulling();

  player = std::make_unique<Player>((Vector3){-50.0f, 0.0f, 5.0f});

  std::string maptoload = "dm4.bsp";
  try
  {
    models = LoadModelsFromBSPFile(maptoload);
  }
  catch (...)
  {
  }

  DisableCursor();
  Camera3D_Init();
};

/*
Update
*/
static inline bool menuMode = false;
static inline void Update()
{
  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
  {
    menuMode = !menuMode;
    if (menuMode)
    {
      EnableCursor();
      ShowCursor();
    }
    else
    {
      DisableCursor();
      HideCursor();
    }
  }
  player->Update(camera.get(), menuMode);
  Camera3D_Update();
};

/*
Draw
*/
static inline void Draw()
{
  if (!camera)
    return;

  static bool enable_wireframe = false;
  BeginDrawing();
  ClearBackground(GRAY);
  BeginMode3D(*camera);
  BSP_Draw(shader, enable_wireframe, camera->position);
  EndMode3D();
};

/*
DrawGUI
*/
static inline void DrawGUI()
{
  DrawFPS(0, 0);
  BSP_DrawDebug(camera->position);
  EndDrawing();
};

/*
CleanUp
*/
void CleanUp()
{
  UnloadShader(shader);
  BSP_CleanUp();
};

/*
main
*/
int main()
{
  Init();

  while (!WindowShouldClose())
  {
    Update();
    Draw();
    DrawGUI();
  }

  CleanUp();
  CloseWindow();
  return 0;
};