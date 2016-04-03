#ifndef DEF_SHADER
#define DEF_SHADER


// Include Windows

#ifdef WIN32
#include <GL/glew.h>


// Include Mac

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>


// Include UNIX/Linux

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif


// Includes communs

#include <iostream>
#include <string>
#include <fstream>


// Classe Shader

class Shader
{
    public:
    Shader(std::string vertexSource, std::string fragmentSource);
    ~Shader();


    GLuint getProgramID() const;


    private:

    GLuint m_vertexID;
    GLuint m_fragmentID;
    GLuint m_programID;

    bool load(std::string vertexSource, std::string fragmentSource);
    bool compilerShader(GLuint &shader, GLenum type, std::string const &fichierSource);
};

#endif
