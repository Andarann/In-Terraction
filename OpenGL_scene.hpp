#ifndef OPENGL_SCENE_HPP_INCLUDED
#define OPENGL_SCENE_HPP_INCLUDED

//GLEW :
//If windows
#ifdef WIN32
#include <GL/glew.h>

//Elseif Apple
#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>

// Else (UNIX/Linux)
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

//GLM includes
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//Others
#include <string>
#include <iostream>
#include <stdio.h>
#include "math_3d.h"
#include "Input.hpp"

struct OpenGL_scene//Is not a class, because it would be annoying to use its methods only in a mainLoop() function
{
public:
    OpenGL_scene(std::string windowName, int windowWidth, int windowHeight);
    ~OpenGL_scene();

private:
    void initializeSDL();
    void initializeOpenGL();

private:
    std::string m_windowName;
    int m_windowHeight, m_windowWidth;

public:
    SDL_Window *m_window;
    SDL_GLContext m_context;
    Input m_input;
};

#endif // OPENGL_SCENE_HPP_INCLUDED
