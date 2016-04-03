#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include <SDL2/SDL.h>

class Input{
public:
    //Constructors/destructors
    Input();
    ~Input();

    //Mutators
    void updateEvents();

    //Accessors
    bool keyPressed(SDL_Scancode const toCheck) const;
    bool mouseButtonPressed(Uint8 const toCheck) const;
    bool closeWindow() const;
    bool mouseMoved() const;
    int getMouseX() const;
    int getMouseY() const;
    int getMouseRelX() const;
    int getMouseRelY() const;
    void showMouse(bool show) const;
    void blockMouse(bool block) const;
private:
    SDL_Event m_event;
    bool m_keys[SDL_NUM_SCANCODES];
    bool m_mouseKeys[8];

    int m_x, m_y;
    int m_xRel, m_yRel;
    bool m_closeWindow;
};

#endif // INPUT_HPP_INCLUDED
