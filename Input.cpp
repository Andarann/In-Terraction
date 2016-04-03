
#include "Input.hpp"

Input::Input() : m_x(0), m_y(0), m_xRel(0), m_yRel(0), m_closeWindow(false)
{
    for (int i(0);i<SDL_NUM_SCANCODES;i++)
        m_keys[i] = false;

    for (int i(0);i<8;i++)
        m_mouseKeys[i] = false;
}

Input::~Input()
{}

void Input::updateEvents()
{
    m_xRel = m_yRel = 0;

    while (SDL_PollEvent(&m_event))//While there are events to treat, we loop
    {
        switch (m_event.type)
        {
            case SDL_KEYDOWN:
                m_keys[m_event.key.keysym.scancode] = true;
                break;

            case SDL_KEYUP:
                m_keys[m_event.key.keysym.scancode] = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                m_mouseKeys[m_event.button.button] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                m_mouseKeys[m_event.button.button] = false;
                break;

            case SDL_MOUSEMOTION:
                m_x = m_event.motion.x;
                m_y = m_event.motion.y;

                m_xRel = m_event.motion.xrel;
                m_yRel = m_event.motion.yrel;
                break;

            case SDL_WINDOWEVENT:
                if (m_event.window.type == SDL_WINDOWEVENT_CLOSE)
                    m_closeWindow = true;
                break;

            default:
                break;
        }
    }
}

bool Input::keyPressed(SDL_Scancode const toCheck) const
{
    return m_keys[toCheck];
}

bool Input::mouseButtonPressed(Uint8 const toCheck) const
{
    return m_mouseKeys[toCheck];
}

bool Input::closeWindow() const
{
    return m_closeWindow;
}

bool Input::mouseMoved() const
{
    return (m_xRel || m_yRel);//If one of these != 0, the mouse has moved
}

int Input::getMouseX() const
{
    return m_x;
}

int Input::getMouseY() const
{
    return m_y;
}

int Input::getMouseRelX() const
{
    return m_xRel;
}

int Input::getMouseRelY() const
{
    return m_yRel;
}

void Input::showMouse(bool show) const
{
    if(show)
        SDL_ShowCursor(SDL_ENABLE);
    else
        SDL_ShowCursor(SDL_DISABLE);
}

void Input::blockMouse(bool block) const
{
    if (block)
        SDL_SetRelativeMouseMode(SDL_TRUE);
    else
        SDL_SetRelativeMouseMode(SDL_FALSE);
}
