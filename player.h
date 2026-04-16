#pragma once
#include "raylib.h"
#include "raymath.h"
#include "bsp.h"

class Player
{
public:
    Vector3 position;
    Vector3 velocity;
    float yaw;
    float pitch;

    const float move_speed = 200.0f;
    const float jump_height = 8.0f;
    const float mouse_sensitivity = 0.003f;
    const float gamepad_sensitivity = 2.0f;
    const float pitch_limit = 1.5f;

    Player(Vector3 spawn_pos)
    {
        position = spawn_pos;
        velocity = {0, 0, 0};
        yaw = 0.0f;
        pitch = 0.0f;
    };
    virtual ~Player() = default;

    /*
    Player Update
    called every frame, player update event
    */
    void Update(Camera3D *camera, bool menu_mode)
    {
        if (!menu_mode)
            HandleRotation(GetFrameTime());

        Vector3 forward = {sinf(yaw), 0.f, -cosf(yaw)};
        Vector3 right = {cosf(yaw), 0.f, sinf(yaw)};

        float fmove, smove = 0.0f; // forward & side move

        if (!menu_mode)
            GetInput(fmove, smove);

        position = bsp_collider.MoveAndSlide(position, velocity, forward, right, fmove, smove);

        UpdateCamera(camera);
    };

    /*
    HandleRotation
    handles mouse yaw and pitch
    */
    void HandleRotation(float dt)
    {
        Vector2 mouse_delta = GetMouseDelta();
        yaw += mouse_delta.x * mouse_sensitivity;
        pitch -= mouse_delta.y * mouse_sensitivity;

        if (IsGamepadAvailable(0))
        {
            const float deadzone = 0.15f;
            float stick_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
            float stick_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

            if (fabsf(stick_x) > deadzone)
                yaw += stick_x * mouse_sensitivity * gamepad_sensitivity * 100.0f * dt;
            if (fabsf(stick_y) > deadzone)
                pitch -= stick_y * mouse_sensitivity * gamepad_sensitivity * 100.0f * dt;
        }

        pitch = Clamp(pitch, -pitch_limit, pitch_limit);
    };

    /*
    GetInput
    handles directional input of player
    */
    void GetInput(float &fmove, float &smove)
    {
        Vector2 direction = {0.0f, 0.0f};

        if (IsKeyDown(KEY_W))
            direction.y += 1.0f;
        if (IsKeyDown(KEY_S))
            direction.y -= 1.0f;
        if (IsKeyDown(KEY_D))
            direction.x += 1.0f;
        if (IsKeyDown(KEY_A))
            direction.x -= 1.0f;

        if (IsGamepadAvailable(0))
        {
            const float deadzone = 0.15f;
            float move_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            float move_y = -GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

            if (fabsf(move_x) > deadzone)
                direction.x = move_x;
            if (fabsf(move_y) > deadzone)
                direction.y = move_y;
        }

        if (Vector2Length(direction) > 1.0f)
            direction = Vector2Normalize(direction);

        smove = direction.x * move_speed;
        fmove = direction.y * move_speed;
    };

    /*
    Update Camera
    updates camera position, yaw, and pitch
    */
    void UpdateCamera(Camera3D *camera)
    {
        if (!camera)
            return;

        Vector3 viewdir = {
            sinf(yaw),
            tanf(pitch),
            -cosf(yaw)};
        camera->position = (Vector3){position.x, position.y + 0.5f, position.z};
        camera->target = Vector3Add(camera->position, viewdir);
    };
};