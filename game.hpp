#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "OpenGL_scene.hpp"     //We'll do 3d here
#include "chunkManagement.hpp"  //Needed here to get the world's data
#include "textures.h"
#include "Shader.h"
#include "camera.h"
#include <vector>

void mainLoop();

class HeightMap{
public:
    HeightMap();
    ~HeightMap();

    bool newHeightMapQUAD(std::string const& toRead, float voxelSize, bool fuckedColors);
    bool newHeightMapLINES(std::string const& toRead, float voxelSize, bool fuckedColors);
    void deleteHeightMap();

    void displayHeightMap(glm::mat4& modelview, glm::mat4& projection) const;
    bool isObstacle(int x, int y, int z) const;
private:
    vector< vector<int> > m_heightMap;
    int m_width, m_height;
    float *m_vertices;
    float *m_color;
    int m_verticesCount;
    GLenum m_drawMode;
    Shader m_shader;
    Uint32 getpixel(SDL_Surface *surface, int x, int y);
};

#endif // GAME_HPP_INCLUDED
