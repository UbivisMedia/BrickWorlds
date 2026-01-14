#include "Camera.h"
#include <math>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : m_position(0.0f, 10.0f, 30.0f)
    , m_pitch(0.0f)
    , m_yaw(-90.0f)
    , m_fov(fov)
    , m_aspectRatio(aspectRatio)
    , m_nearPlane(nearPlane)
    , m_farPlane(farPlane)
{
    updateVectors();
}

void Camera::setPosition(const BrickWorlds::Core::Vector3f& pos) {
    m_position = pos;
}

void Camera::setRotation(float pitch, float yaw) {
    m_pitch = pitch;
    m_yaw = yaw;

    // Constrain pitch
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    updateVectors();
}

void Camera::move(const BrickWorlds::Core::Vector3f& offset) {
    m_position = m_position + offset;
}

void Camera::rotate(float pitchDelta, float yawDelta) {
    m_pitch += pitchDelta;
    m_yaw += yawDelta;

    // Constrain pitch
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    updateVectors();
}

void Camera::updateVectors() {
    float pitchRad = m_pitch * M_PI / 180.0f;
    float yawRad = m_yaw * M_PI / 180.0f;

    m_forward.x = cos(yawRad) * cos(pitchRad);
    m_forward.y = sin(pitchRad);
    m_forward.z = sin(yawRad) * cos(pitchRad);
    m_forward = m_forward.normalized();

    // Right = forward x worldUp
    BrickWorlds::Core::Vector3f worldUp(0.0f, 1.0f, 0.0f);
    m_right.x = m_forward.y * worldUp.z - m_forward.z * worldUp.y;
    m_right.y = m_forward.z * worldUp.x - m_forward.x * worldUp.z;
    m_right.z = m_forward.x * worldUp.y - m_forward.y * worldUp.x;
    m_right = m_right.normalized();

    // Up = right x forward
    m_up.x = m_right.y * m_forward.z - m_right.z * m_forward.y;
    m_up.y = m_right.z * m_forward.x - m_right.x * m_forward.z;
    m_up.z = m_right.x * m_forward.y - m_right.y * m_forward.x;
    m_up = m_up.normalized();
}

BrickWorlds::Core::Vector3f Camera::getForward() const {
    return m_forward;
}

BrickWorlds::Core::Vector3f Camera::getRight() const {
    return m_right;
}

void Camera::getViewMatrix(float* out) const {
    // LookAt matrix
    BrickWorlds::Core::Vector3f center = m_position + m_forward;

    BrickWorlds::Core::Vector3f f = (center - m_position).normalized();
    BrickWorlds::Core::Vector3f r = m_right;
    BrickWorlds::Core::Vector3f u = m_up;

    // Column-major order for OpenGL
    out[0] = r.x; out[4] = r.y; out[8] = r.z;   out[12] = -(r.x * m_position.x + r.y * m_position.y + r.z * m_position.z);
    out[1] = u.x; out[5] = u.y; out[9] = u.z;   out[13] = -(u.x * m_position.x + u.y * m_position.y + u.z * m_position.z);
    out[2] = -f.x; out[6] = -f.y; out[10] = -f.z; out[14] = (f.x * m_position.x + f.y * m_position.y + f.z * m_position.z);
    out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f; out[15] = 1.0f;
}

void Camera::getProjectionMatrix(float* out) const {
    // Perspective projection matrix
    float tanHalfFov = tan((m_fov * M_PI / 180.0f) / 2.0f);

    for (int i = 0; i < 16; i++) out[i] = 0.0f;

    out[0] = 1.0f / (m_aspectRatio * tanHalfFov);
    out[5] = 1.0f / tanHalfFov;
    out[10] = -(m_farPlane + m_nearPlane) / (m_farPlane - m_nearPlane);
    out[11] = -1.0f;
    out[14] = -(2.0f * m_farPlane * m_nearPlane) / (m_farPlane - m_nearPlane);
}
