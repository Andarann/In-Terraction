#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED

#include <GL/glew.h>//A lot of stuff ^^
#include <SDL2/SDL_image.h>//IMG_Load();
#include <string>//std::string
#include <iostream>//cerr, endl

class Texture
{
    public:
    //Constructor/Destructor
    Texture();
    Texture(std::string const& textureFilePath);
    Texture(Texture const& toAssign);
    ~Texture();
    //Accessors
    GLuint getID() const;
    int getHeight() const;
    int getWidth() const;
    //Mutators
    Texture& operator=(Texture const& toCopy);
    bool loadTexture(std::string const& textureFilePath);

    private:
    GLuint m_id;
    int m_height, m_width;

    SDL_Surface* inverserPixels(SDL_Surface *imageSource) const;
};



#endif // TEXTURES_H_INCLUDED
