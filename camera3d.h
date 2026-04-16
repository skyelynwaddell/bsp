#pragma once
#include "raylib.h"
#include "raymath.h"
#include "rlights.h"
#include <algorithm>
#include <memory>
#include "rlgl.h"

inline std::unique_ptr<Camera> camera;
inline long shadermodtime;
inline Shader camshader;
inline Light camlight;
inline int lightpower = 50;
inline int LIGHT_COUNT = 0;

/*
HandleMouseCursorActive
right click to free mouse , or return to game
*/
static bool HandleMouseCursorActive(bool menu_mode)
{
  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
  {
    if (menu_mode = !menu_mode)
    {
      ShowCursor();
      EnableCursor();
    }
    else
    {
      HideCursor();
      DisableCursor();
    }
  }
  return menu_mode;
};

/*
Camera3D_UpdateShaders
Updates camera lighting & shaders
*/
static void Camera3D_UpdateShaders()
{
  if (!camera)
    return;

  long currentShaderModTime = std::max(GetFileModTime(VS_PATH), GetFileModTime(FS_PATH));
  if (currentShaderModTime != shadermodtime)
  {
    Shader updatedShader = LoadShader(VS_PATH, FS_PATH);
    if (updatedShader.id != rlGetShaderIdDefault()) // It was correctly loaded
    {
      UnloadShader(camshader);
      camshader = updatedShader;
      camshader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(camshader, "viewPos");

      LIGHT_COUNT = 0;
      camlight = CreateLight(LIGHT_POINT, camera->position, {}, WHITE, camshader);
      SetShaderValue(camshader, GetShaderLocation(camshader, "lightPower"), &lightpower, SHADER_UNIFORM_INT);
    }

    shadermodtime = currentShaderModTime;
  }
};

/*
Camera3D_Move
Used for free Spectator mode
*/
static void Camera3D_Move(Camera &camera, bool enabled)
{
  if (!enabled)
    return;

  float speed = 10.0f * GetFrameTime();
  Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
  Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

  if (IsKeyDown(KEY_W))
    camera.position = Vector3Add(camera.position, Vector3Scale(forward, speed));
  if (IsKeyDown(KEY_S))
    camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, speed));
  if (IsKeyDown(KEY_D))
    camera.position = Vector3Add(camera.position, Vector3Scale(right, speed));
  if (IsKeyDown(KEY_A))
    camera.position = Vector3Subtract(camera.position, Vector3Scale(right, speed));
  if (IsKeyDown(KEY_SPACE))
    camera.position.y += speed;
  if (IsKeyDown(KEY_LEFT_CONTROL))
    camera.position.y -= speed;

  Vector2 mouseDelta = GetMouseDelta();
  float sensitivity = 0.003f;
  static float yaw, pitch = 0.0f;

  yaw += mouseDelta.x * sensitivity;
  pitch -= mouseDelta.y * sensitivity;

  pitch = Clamp(pitch, -1.5f, 1.5f);

  Vector3 direction = {
      cosf(pitch) * sinf(yaw),
      sinf(pitch),
      -cosf(pitch) * cosf(yaw)};

  camera.target = Vector3Add(camera.position, direction);
};

/*
Camera3D_Init
inits the 3d game camera and stats
*/
inline void Camera3D_Init()
{
  shadermodtime = std::max(GetFileModTime(VS_PATH), GetFileModTime(FS_PATH));
  camshader = LoadShader(VS_PATH, FS_PATH);
  camshader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(camshader, "viewPos");

  camera = std::make_unique<Camera>(Camera{
      .position = {10.0f, 10.0f, 10.0f},
      .target = {0.0f, 0.0f, 0.0f},
      .up = {0.0f, 1.0f, 0.0f},
      .fovy = 90.0f,
      .projection = CAMERA_PERSPECTIVE});

  camlight = CreateLight(LIGHT_POINT, camera->position, {}, WHITE, camshader);
  SetShaderValue(camshader, GetShaderLocation(camshader, "lightPower"), &lightpower, SHADER_UNIFORM_INT);
};

/*
Camera3D_Update
updates the camera pos & data every tick
*/
inline void Camera3D_Update(bool &menu_mode)
{
  if (!camera)
    return;

  menu_mode = HandleMouseCursorActive(menu_mode);
  Camera3D_UpdateShaders();
  camlight.position = camera->position;
};