
#include "camera.h"

using namespace std;

Camera::Camera() : m_circlePoint(), m_circleCenter(), m_radius(100.0), m_phi(180.0), m_theta(-100.0), m_eyeSpeed(0.5),
                   m_cameraSpeed(5.5), m_staticEye(true)
{
    m_circleCenter = glm::vec3(0,100,0);
    m_circlePoint = glm::vec3(0,0,0);
}

Camera::~Camera()
{

}

void Camera::newCameraIO(float x, float y, float z)
{

}

void Camera::newCameraOI(float x, float y, float z, float radius)
{

}

void Camera::placeCamera(Input const& inputs, glm::mat4& modelview)
{
    double phiRadians(0.0), thetaRadians(0.0);
    m_theta -= static_cast<double>(inputs.getMouseRelX()) * static_cast<double>(m_eyeSpeed);
    if (m_theta>=360.0 || m_theta<=(-360.0))
        m_theta=0.0;

    if (m_staticEye)
    {
        m_phi += static_cast<double>(inputs.getMouseRelY()) * static_cast<double>(m_eyeSpeed);
        if (m_phi>=269.0)
            m_phi=269.0;
        else if (m_phi<=91.0)
            m_phi=91.0;
    }
    else
    {
        m_phi -= static_cast<double>(inputs.getMouseRelY()) * static_cast<double>(m_eyeSpeed);
        if (m_phi>=269.0)
            m_phi=269.0;
        else if (m_phi<=91.0)
            m_phi=91.0;
    }

    phiRadians = m_phi*M_PI/180;
    thetaRadians = m_theta*M_PI/180;

    //Here only the y-axis is used as the vertical axis. But it always should. Z is depth, not height.
    glm::vec3 movingPoint;
    movingPoint.x = cos(phiRadians) * sin(thetaRadians)* static_cast<double>(m_radius);
    movingPoint.y = sin(phiRadians) * static_cast<double>(m_radius);
    movingPoint.z = cos(phiRadians) * cos(thetaRadians) * static_cast<double>(m_radius);

    glm::vec3 temp(m_circleCenter);

    if (inputs.keyPressed(SDL_SCANCODE_W))
    {
        m_circleCenter.x -= sin(thetaRadians) * m_cameraSpeed;
        m_circleCenter.z -= cos(thetaRadians) * m_cameraSpeed;
    }

    if (inputs.keyPressed(SDL_SCANCODE_S))
    {
        m_circleCenter.x += sin(thetaRadians) * m_cameraSpeed;
        m_circleCenter.z += cos(thetaRadians) * m_cameraSpeed;
    }

    if (inputs.keyPressed(SDL_SCANCODE_A))
    {
        if (m_staticEye)
        {
            m_circleCenter.x -= sin((m_theta+90)*M_PI/180) * m_cameraSpeed;
            m_circleCenter.z -= cos((m_theta+90)*M_PI/180) * m_cameraSpeed;
        }
        else
        {
            m_circleCenter.x += sin((m_theta+90)*M_PI/180) * m_cameraSpeed;
            m_circleCenter.z += cos((m_theta+90)*M_PI/180) * m_cameraSpeed;
        }
    }

    if (inputs.keyPressed(SDL_SCANCODE_D))
    {
        if (m_staticEye)
        {
            m_circleCenter.x += sin((m_theta+90)*M_PI/180) * m_cameraSpeed;
            m_circleCenter.z += cos((m_theta+90)*M_PI/180) * m_cameraSpeed;
        }
        else
        {
            m_circleCenter.x -= sin((m_theta+90)*M_PI/180) * m_cameraSpeed;
            m_circleCenter.z -= cos((m_theta+90)*M_PI/180) * m_cameraSpeed;
        }
    }

    if (inputs.keyPressed(SDL_SCANCODE_SPACE))
    {
        m_circleCenter.y += m_cameraSpeed;
    }

    if (inputs.keyPressed(SDL_SCANCODE_LSHIFT))
    {
        m_circleCenter.y -= m_cameraSpeed;
    }




    /*if (temp != m_circleCenter)
        cout << "(" << m_circleCenter.x/(CHUNK_DIMENSIONS*SINGLE_BLOCK_SIZE) << "," << m_circleCenter.y/(CHUNK_DIMENSIONS*SINGLE_BLOCK_SIZE) << "," << m_circleCenter.z/(CHUNK_DIMENSIONS*SINGLE_BLOCK_SIZE) << ")\n";
    */
    movingPoint += m_circleCenter;

    m_circlePoint = movingPoint;

    if (m_staticEye)
    {
        modelview = glm::lookAt(m_circleCenter, m_circlePoint, glm::vec3(0,1,0));
    }
    else
    {
        modelview = glm::lookAt(m_circlePoint, m_circleCenter, glm::vec3(0,1,0));
    }


}

glm::vec3 Camera::getCameraPosition() const
{
    return m_circleCenter;
}

glm::vec3 Camera::getPointPosition() const
{
    return m_circlePoint;
}
