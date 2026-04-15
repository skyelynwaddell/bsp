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

    float fmove = 0.0f;
    float smove = 0.0f;
    const float quake_speed = 320.0f;

    if (IsKeyDown(KEY_W))
      fmove += quake_speed;
    if (IsKeyDown(KEY_S))
      fmove -= quake_speed;
    if (IsKeyDown(KEY_D))
      smove += quake_speed;
    if (IsKeyDown(KEY_A))
      smove -= quake_speed;

    if (IsGamepadAvailable(0))
    {
      const float deadzone = 0.15f;
      float moveX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
      float moveY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
      if (fabsf(moveX) > deadzone)
        smove = moveX * quake_speed;
      if (fabsf(moveY) > deadzone)
        fmove = -moveY * quake_speed;
    }

    bool grounded = bsp_collider.IsGrounded();
    const float jump_vel = 8.0f; // Scale this as needed for your world

    if (grounded && (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)))
    {
      velocity.y = jump_vel;
    }

    position = bsp_collider.MoveAndSlide(position, velocity, forward, right, fmove, smove, dt);

    Vector3 viewDir = {
        sinf(global_cam_yaw) * cosf(global_cam_pitch),
        sinf(global_cam_pitch),
        -cosf(global_cam_yaw) * cosf(global_cam_pitch)};

    camera->position = (Vector3){position.x, position.y + 0.5f, position.z};
    camera->target = Vector3Add(camera->position, viewDir);
  };