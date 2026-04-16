#pragma once
#include "raylib.h"
#include "raymath.h"
#include "bsp.h"

class Player {
public:
    Player(Vector3 startPos) {
        position = startPos;
        velocity = { 0, 0, 0 };
        yaw = 0.0f;
        pitch = 0.0f;
    }

    void Update(Camera3D* camera, bool isMenuMode) {
        float dt = GetFrameTime();

        if (!isMenuMode) {
            HandleRotation(dt);
        }

        Vector3 forward = { sinf(yaw), 0.f, -cosf(yaw) };
        Vector3 right = { cosf(yaw), 0.f, sinf(yaw) };
        
        float fmove = 0.0f;
        float smove = 0.0f;

        if (!isMenuMode) {
            GetInput(fmove, smove);
        }

        bool grounded = bsp_collider.IsGrounded();
        if (grounded && (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) {
            velocity.y = jumpImpulse;
        }

        position = bsp_collider.MoveAndSlide(position, velocity, forward, right, fmove, smove);

        UpdateCamera(camera);
    }

    Vector3 GetPosition() const { return position; }
    Vector3 GetVelocity() const { return velocity; }

private:
    Vector3 position;
    Vector3 velocity;
    float yaw;
    float pitch;

    const float moveSpeed = 200.0f;
    const float jumpImpulse = 8.0f;
    const float mouseSensitivity = 0.003f;
    const float gamepadSens = 2.0f;
    const float pitchLimit = 1.5f; 

    void HandleRotation(float dt) {
        Vector2 mouseDelta = GetMouseDelta();
        yaw += mouseDelta.x * mouseSensitivity;
        pitch -= mouseDelta.y * mouseSensitivity;

        if (IsGamepadAvailable(0)) {
            const float deadzone = 0.15f;
            float stickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
            float stickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
            
            if (fabsf(stickX) > deadzone) yaw += stickX * mouseSensitivity * gamepadSens * 100.0f * dt;
            if (fabsf(stickY) > deadzone) pitch -= stickY * mouseSensitivity * gamepadSens * 100.0f * dt;
        }

        pitch = Clamp(pitch, -pitchLimit, pitchLimit);
    }

    void GetInput(float& fmove, float& smove) {
        if (IsKeyDown(KEY_W)) fmove += moveSpeed;
        if (IsKeyDown(KEY_S)) fmove -= moveSpeed;
        if (IsKeyDown(KEY_D)) smove += moveSpeed;
        if (IsKeyDown(KEY_A)) smove -= moveSpeed;

        if (IsGamepadAvailable(0)) {
            const float deadzone = 0.15f;
            float moveX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            float moveY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
            if (fabsf(moveX) > deadzone) smove = moveX * moveSpeed;
            if (fabsf(moveY) > deadzone) fmove = -moveY * moveSpeed;
        }
    }

    void UpdateCamera(Camera3D* camera) {
        if (!camera) return;

        Vector3 viewDir = {
            sinf(yaw) * cosf(pitch),
            sinf(pitch),
            -cosf(yaw) * cosf(pitch)
        };

        camera->position = (Vector3){ position.x, position.y + 0.5f, position.z };
        camera->target = Vector3Add(camera->position, viewDir);
    }
};