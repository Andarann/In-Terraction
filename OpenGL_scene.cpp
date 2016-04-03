
#include "OpenGL_scene.hpp"

using namespace std;

OpenGL_scene::OpenGL_scene(string windowName, int windowWidth, int windowHeight) : m_input()
{
    m_windowName = windowName;
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    initializeSDL();
    initializeOpenGL();
}

OpenGL_scene::~OpenGL_scene()
{
    clog << "Destroying scene..." << endl;
    SDL_DestroyWindow(m_window);
    SDL_GL_DeleteContext(m_context);
    TTF_Quit();
    SDL_Quit();
    clog << "Done." << endl;
}

void OpenGL_scene::initializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cerr << "Error : Something went wrong during SDL initialization. Reason : " << SDL_GetError() << endl;
        cerr << "Closing the application..." << endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    else
    {
        clog << "SDL correctly initialized !" << endl;
    }

    //Create our main window
    m_window = SDL_CreateWindow(m_windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!m_window)
    {
        cerr << "Error : Something went wrong during the SDL_Window creation. Reason : " << SDL_GetError() << endl;
        cerr << "Closing the application..." << endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    else
    {
        clog << "SDL_Window correctly created !" << endl;
    }

    //Initialize SDL_ttf
    if (TTF_Init() < 0)
    {
        cerr << "Error : something went wrong during the SDL_ttf initialization. Reason : " << SDL_GetError() << endl;
        cerr << "Closing the application..." << endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    else
    {
        clog << "SDL_TTF correctly initialized ! " << endl;
    }
}

void OpenGL_scene::initializeOpenGL()
{
    //Let's create the GL context
    m_context = SDL_GL_CreateContext(m_window);

    if (!m_context)
    {
        cerr << "Error : Something went wrong during the SDL_GLContext creation. Reason : " << SDL_GetError() << endl;
        cerr << "Closing the application..." << endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    else
    {
        clog << "SDL_GLContext correctly created !" << endl;
    }

    #ifdef WIN32
        //Initialize GLEW
        GLenum initializationGlew(glewInit());

        if (initializationGlew != GLEW_OK)
        {
            cerr << "Error : something went wrong during the GLEW initialization. Reason : " << glewGetErrorString(initializationGlew) << endl;
            cerr << "Closing the application..." << endl;
            SDL_DestroyWindow(m_window);
            SDL_GL_DeleteContext(m_context);
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
        else
        {
            clog << "GLEW correctly initialized !" << endl;
        }
    #endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}
