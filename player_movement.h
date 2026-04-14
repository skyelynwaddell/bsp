  void PlayerMovement()
  {
    if (!IsMenuMode)
    {
      Vector2 mouseDelta = GetMouseDelta();
      global_cam_yaw += mouseDelta.x * global_cam_sensitivity;
      global_cam_pitch -= mouseDelta.y * global_cam_sensitivity;

      if (IsGamepadAvailable(0))
      {
        const float deadzone = 0.15f;
        float stickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
        float stickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
        if (fabsf(stickX) > deadzone)
          global_cam_yaw += stickX * global_cam_sensitivity * global_analog_sens * GetFrameTime();
        if (fabsf(stickY) > deadzone)
          global_cam_pitch -= stickY * global_cam_sensitivity * global_analog_sens * GetFrameTime();
      }
      global_cam_pitch = Clamp(global_cam_pitch, -global_cam_pitch_max, global_cam_pitch_max);
    }

    float dt = GetFrameTime();
    Vector3 forward = {sinf(global_cam_yaw), 0.f, -cosf(global_cam_yaw)};
    Vector3 right = {cosf(global_cam_yaw), 0.f, sinf(global_cam_yaw)};

    Vector3 wish = {0, 0, 0};
    if (IsKeyDown(KEY_W))
      wish = Vector3Add(wish, forward);
    if (IsKeyDown(KEY_S))
      wish = Vector3Subtract(wish, forward);
    if (IsKeyDown(KEY_D))
      wish = Vector3Add(wish, right);
    if (IsKeyDown(KEY_A))
      wish = Vector3Subtract(wish, right);

    if (IsGamepadAvailable(0))
    {
      const float deadzone = 0.15f;
      float moveX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
      float moveY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
      if (fabsf(moveX) > deadzone)
        wish = Vector3Add(wish, Vector3Scale(right, moveX));
      if (fabsf(moveY) > deadzone)
        wish = Vector3Add(wish, Vector3Scale(forward, -moveY));
    }

    float len = Vector3Length(wish);
    if (len > 1.0f)
      wish = Vector3Scale(wish, 1.0f / len);

    // Horizontal
    Vector3 target_h = Vector3Scale(wish, speed);
    velocity.x = Lerp(velocity.x, target_h.x, acceleration * dt);
    velocity.z = Lerp(velocity.z, target_h.z, acceleration * dt);

    // Gravity + jump
    const float gravity = 20.0f;
    const float jump_vel = 8.0f;
    bool grounded = bsp_collider.IsGrounded(position, collision_box);
    if (grounded && velocity.y <= 0.f)
      velocity.y = 0.f;
    else
      velocity.y -= gravity * dt;

    if (grounded && IsKeyPressed(KEY_SPACE))
      velocity.y = jump_vel;
    if (grounded && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
      velocity.y = jump_vel;

    position = bsp_collider.MoveAndSlide(position, velocity, collision_box, dt);
  };