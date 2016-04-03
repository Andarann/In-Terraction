#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <iostream>
#include <cmath>
#include "OpenGL_scene.hpp"
#include "chunkManagement.hpp"

class Camera{
public:
    Camera();
    ~Camera();

    void newCameraIO(float x, float y, float z);//Camera that looks from a static point
    void newCameraOI(float x, float y, float z, float radius);//The eye is on a circle around a defined point
    void placeCamera(Input const& inputs, glm::mat4& modelview);

    glm::vec3 getCameraPosition() const;
    glm::vec3 getPointPosition() const;
private:
    glm::vec3 m_circlePoint;//Position of the point on the circle (the eye in OI mode)
    glm::vec3 m_circleCenter;//Position of the circle's center (the eye in IO mode)

    float m_radius;
    double m_phi, m_theta;//In degrees
    bool m_staticEye;//IO camera, the camera is in the center of the circle

    float m_cameraSpeed;//For m_circleCenter movement speed with keyboard
    float m_eyeSpeed;//For m_phi, m_theta movement speed
};

#endif // CAMERA_H_INCLUDED
