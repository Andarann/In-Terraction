#include "Shader.h"

// Constructeurs et Destructeur

Shader::Shader(std::string vertexSource, std::string fragmentSource) : m_vertexID(0), m_fragmentID(0), m_programID(0)
{
    load(vertexSource, fragmentSource);
}


Shader::~Shader()
{
    // Destruction du shader
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
    glDeleteProgram(m_programID);
}

bool Shader::load(std::string vertexSource, std::string fragmentSource)
{
    // Destruction d'un éventuel ancien Shader

    if(glIsShader(m_vertexID) == GL_TRUE)
        glDeleteShader(m_vertexID);

    if(glIsShader(m_fragmentID) == GL_TRUE)
        glDeleteShader(m_fragmentID);

    if(glIsProgram(m_programID) == GL_TRUE)
        glDeleteProgram(m_programID);


    // Compilation des shaders

    if(!compilerShader(m_vertexID, GL_VERTEX_SHADER, vertexSource))
        return false;

    if(!compilerShader(m_fragmentID, GL_FRAGMENT_SHADER, fragmentSource))
        return false;


    // Création du programme

    m_programID = glCreateProgram();


    // Association des shaders

    glAttachShader(m_programID, m_vertexID);
    glAttachShader(m_programID, m_fragmentID);


    // Verrouillage des entrées shader

    glBindAttribLocation(m_programID, 0, "in_Vertex");
    glBindAttribLocation(m_programID, 1, "in_Color");
    glBindAttribLocation(m_programID, 2, "in_TexCoord0");


    // Linkage du programme

    glLinkProgram(m_programID);


    // Vérification du linkage

    GLint erreurLink(0);
    glGetProgramiv(m_programID, GL_LINK_STATUS, &erreurLink);


    // S'il y a eu une erreur

    if(erreurLink != GL_TRUE)
    {
        // Récupération de la taille de l'erreur

        GLint tailleErreur(0);
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &tailleErreur);


        // Allocation de mémoire

        char *erreur = new char[tailleErreur + 1];


        // Récupération de l'erreur

        glGetShaderInfoLog(m_programID, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';


        // Affichage de l'erreur

        std::cout << erreur << std::endl;
        // Libération de la mémoire et retour du booléen false
        delete[] erreur;
        glDeleteProgram(m_programID);

        return false;
    }
    return true;
}


bool Shader::compilerShader(GLuint &shader, GLenum type, std::string const &fichierSource)
{
    shader = glCreateShader(type);

    if(shader == 0)
    {
        std::cout << "Erreur, le type de shader (" << type << ") n'existe pas" << std::endl;
        return false;
    }

    std::ifstream fichier(fichierSource.c_str());

    if(!fichier)
    {
        std::cout << "Erreur le fichier " << fichierSource << " est introuvable" << std::endl;
        glDeleteShader(shader);

        return false;
    }

    std::string ligne;
    std::string codeSource;

    while(getline(fichier, ligne))
        codeSource += ligne + '\n';

    fichier.close();

    const GLchar* chaineCodeSource = codeSource.c_str();

    glShaderSource(shader, 1, &chaineCodeSource, 0);

    glCompileShader(shader);

    GLint erreurCompilation(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);

    if(erreurCompilation != GL_TRUE)
    {
        GLint tailleErreur(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);

        char *erreur = new char[tailleErreur + 1];

        glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';

        std::cout << erreur << std::endl;

        delete[] erreur;
        glDeleteShader(shader);

        return false;
    }
    else
        return true;
}

GLuint Shader::getProgramID() const
{
    return m_programID;
}
