#pragma once

#include <BrickWorlds/Core/Types.h>

class Camera {
public:
    Camera(float fov = 70.0f, float aspectRatio = 16.0f / 9.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);

    // Camera movement
    void setPosition(const BrickWorlds::Core::Vector3f& pos);
    void setRotation(float pitch, float yaw);
    void move(const BrickWorlds::Core::Vector3f& offset);
    void rotate(float pitchDelta, float yawDelta);

    // Get matrices
    void getViewMatrix(float* out) const;
    void getProjectionMatrix(float* out) const;

    const BrickWorlds::Core::Vector3f& getPosition() const { return m_position; }
    BrickWorlds::Core::Vector3f getForward() const;
    BrickWorlds::Core::Vector3f getRight() const;

private:
    BrickWorlds::Core::Vector3f m_position;
    float m_pitch;  // Rotation around X axis (up/down)
    float m_yaw;    // Rotation around Y axis (left/right)

    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;

    void updateVectors();
    BrickWorlds::Core::Vector3f m_forward;
    BrickWorlds::Core::Vector3f m_right;
    BrickWorlds::Core::Vector3f m_up;
};
