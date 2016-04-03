
#include "textures.h"

using namespace std;

Texture::Texture() : m_id(0), m_height(0), m_width(0)
{

}

Texture::Texture(std::string const& textureFilePath)
{

}

bool Texture::loadTexture(std::string const& textureFilePath)
{
    SDL_Surface *imageSDL = IMG_Load(textureFilePath.c_str());

    if(imageSDL == 0)
    {
        std::cout << "Error : couldn't load the image properly. Reason : " << SDL_GetError() << std::endl;
        return false;
    }

    m_width = imageSDL->w;
    m_height = imageSDL->h;

    //imageSDL = inverserPixels(imageSDL);z

    if(glIsTexture(m_id) == GL_TRUE)
        glDeleteTextures(1, &m_id);

    glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    GLenum internalFormat(0);
    GLenum format(0);

    //Let's determine the texture format
    if(imageSDL->format->BytesPerPixel == 3)
    {

        internalFormat = GL_RGB;

        if(imageSDL->format->Rmask == 0xff)
            format = GL_RGB;

        else
            format = GL_BGR;
    }
    else if(imageSDL->format->BytesPerPixel == 4)
    {
        internalFormat = GL_RGBA;

        if(imageSDL->format->Rmask == 0xff)
            format = GL_RGBA;

        else
            format = GL_BGRA;
    }
    else//Unknown format, loading failed ;(
    {
        std::cout << "Error, internal texture file format not recognized" << std::endl;
        SDL_FreeSurface(imageSDL);
        return false;
    }
    //Filters applying



    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    //Pixels copying
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageSDL->w, imageSDL->h, 0, format, GL_UNSIGNED_BYTE, imageSDL->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(imageSDL);
    return true;
}

SDL_Surface* Texture::inverserPixels(SDL_Surface *imageSource) const
{
    // Copie conforme de l'image source sans les pixels

    SDL_Surface *imageInversee = SDL_CreateRGBSurface(0, imageSource->w, imageSource->h, imageSource->format->BitsPerPixel, imageSource->format->Rmask,
                                                         imageSource->format->Gmask, imageSource->format->Bmask, imageSource->format->Amask);


    // Tableau intermédiaires permettant de manipuler les pixels

    unsigned char* pixelsSources = (unsigned char*) imageSource->pixels;
    unsigned char* pixelsInverses = (unsigned char*) imageInversee->pixels;


    // Inversion des pixels

    for(int i = 0; i < imageSource->h; i++)
    {
        for(int j = 0; j < imageSource->w * imageSource->format->BytesPerPixel; j++)
            pixelsInverses[(imageSource->w * imageSource->format->BytesPerPixel * (imageSource->h - 1 - i)) + j] = pixelsSources[(imageSource->w * imageSource->format->BytesPerPixel * i) + j];
    }


    // Retour de l'image inversée
    return imageInversee;
}

Texture::Texture(Texture const& toAssign)
{
    m_id = toAssign.m_id;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

GLuint Texture::getID() const
{
    return m_id;
}

int Texture::getHeight() const
{
    return m_height;
}

int Texture::getWidth() const
{
    return m_width;
}

Texture& Texture::operator=(Texture const& toCopy)
{
    return *this;
}
