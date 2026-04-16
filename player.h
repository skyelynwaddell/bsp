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
    */
    void Update(Camera3D *camera, bool menu_mode)
    {
        float dt = GetFrameTime();

        if (!menu_mode)
            HandleRotation(dt);

        Vector3 forward = {sinf(yaw), 0.f, -cosf(yaw)};
        Vector3 right = {cosf(yaw), 0.f, sinf(yaw)};

        float fmove = 0.0f;
        float smove = 0.0f;

        if (!menu_mode)
            GetInput(fmove, smove);

        bool grounded = bsp_collider.IsGrounded();
        if (grounded && (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)))
            velocity.y = jump_height;

        position = bsp_collider.MoveAndSlide(position, velocity, forward, right, fmove, smove);

        UpdateCamera(camera);
    };

    /*
    HandleRotation
    */
    void HandleRotation(float dt)
    {
        Vector2 mouseDelta = GetMouseDelta();
        yaw += mouseDelta.x * mouse_sensitivity;
        pitch -= mouseDelta.y * mouse_sensitivity;

        if (IsGamepadAvailable(0))
        {
            const float deadzone = 0.15f;
            float stickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
            float stickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

            if (fabsf(stickX) > deadzone)
                yaw += stickX * mouse_sensitivity * gamepad_sensitivity * 100.0f * dt;
            if (fabsf(stickY) > deadzone)
                pitch -= stickY * mouse_sensitivity * gamepad_sensitivity * 100.0f * dt;
        }

        pitch = Clamp(pitch, -pitch_limit, pitch_limit);
    };

    /*
    GetInput
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
    */
    void UpdateCamera(Camera3D *camera)
    {
        if (!camera)
            return;

        Vector3 viewDir = {
            sinf(yaw),
            tanf(pitch),
            -cosf(yaw)};
        camera->position = (Vector3){position.x, position.y + 0.5f, position.z};
        camera->target = Vector3Add(camera->position, viewDir);
    };
};