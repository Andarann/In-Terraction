
#include "game.hpp"

//I know, global variables are evil, but here since these ones won't be modified by more than one function nobody cares. Yes, even you.
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 780;
float angle = 0.001;

using namespace std;

void mainLoop()
{
    OpenGL_scene scene("My first window", SCREEN_WIDTH, SCREEN_HEIGHT);

    glm::mat4 projection, modelview;

    projection = glm::perspective(70.0, (double) SCREEN_WIDTH / SCREEN_HEIGHT, 1.0, 10000.0);
    modelview = glm::mat4(1.0);
    Camera viewPoint;

    ChunkManager allChunks(4, viewPoint.getCameraPosition());

    int FPS(0);
    double averageFPS(0);
    int averageNumber(0);
    Uint32 timer = SDL_GetTicks();

    scene.m_input.showMouse(true);
    scene.m_input.blockMouse(true);

    Texture merde;
    merde.loadTexture("Textures/terrain.png");

    glm::vec3 testage;
    bool keyPressed(false);

    //allChunks.setVoxelId(0,-1,0, 0);

    while (!(scene.m_input.keyPressed(SDL_SCANCODE_ESCAPE)))//Main loop
    {
        if (SDL_GetTicks()-timer >= 1000)
        {
            if (!averageNumber)
            {
                averageNumber++;
                averageFPS = FPS;
            }
            else
            {
                averageNumber++;
                averageFPS = ((averageFPS*(averageNumber-1))+FPS)/averageNumber;
            }

            cout << "FPS : " << FPS  << ";Average : " << averageFPS << "\r";
            FPS = 0;
            timer = SDL_GetTicks();
        }

        scene.m_input.updateEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear the old stuff

        viewPoint.placeCamera(scene.m_input, modelview);

        testage = viewPoint.getCameraPosition();

        if (scene.m_input.keyPressed(SDL_SCANCODE_TAB))
        {

            allChunks.setVoxelId(static_cast<int>(testage.x)/SINGLE_BLOCK_SIZE, static_cast<int>(testage.y)/SINGLE_BLOCK_SIZE, static_cast<int>(testage.z)/SINGLE_BLOCK_SIZE, 0);
        }

        allChunks.update(viewPoint.getCameraPosition(), viewPoint.getCameraPosition());
        allChunks.displayChunks(projection, modelview, merde);

        SDL_GL_SwapWindow(scene.m_window);//Final render
        FPS++;
    }
}

HeightMap::HeightMap() : m_shader(Shader("Shaders/couleur3D.vert", "Shaders/couleur3D.frag")), m_vertices(0), m_color(0), m_verticesCount(0)
{
    m_width = 0;
    m_height = 0;
}

HeightMap::~HeightMap()
{
    delete[] m_vertices;
    m_vertices = 0;

    delete[] m_color;
    m_color = 0;
}

bool HeightMap::newHeightMapQUAD(std::string const& toRead, float voxelSize, bool fuckedColors)
{
    if (m_vertices || m_color || m_heightMap.size())
        deleteHeightMap();

    SDL_Surface *loadHeightMap = IMG_Load(toRead.c_str());

    if (!loadHeightMap)
    {
        cerr << "Error : couldn't load the heightmap. Reason : " << SDL_GetError() << endl;
        return false;
    }

    m_width = loadHeightMap->w;
    m_height = loadHeightMap->h;

    Uint8 getHeight(0);

    m_heightMap.resize(m_height);
    for (int i(0);i<m_height;i++)
    {
        m_heightMap[i].resize(m_width);
    }

    for (int i(0);i<m_height;i++)
    {
        for (int j(0);j<m_width;j++)
        {
            SDL_GetRGB(getpixel(loadHeightMap, j, i), loadHeightMap->format, &getHeight, &getHeight, &getHeight);
            m_heightMap[i][j] = static_cast<int>(getHeight);
        }
    }

    m_vertices = new float[m_height*m_width*54];
    m_color = new float[m_height*m_width*54];
    int coordinatesCount(0);

    m_drawMode = GL_TRIANGLES;

    for (int i(0);i<(m_height);i++)
    {
        for (int j(0);j<(m_width);j++)
        {
            //Top
            m_vertices[coordinatesCount] = (voxelSize*j); m_vertices[coordinatesCount+1] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+2] = voxelSize*i;//triangle 1
            m_vertices[coordinatesCount+3] = (voxelSize*j)+voxelSize; m_vertices[coordinatesCount+4] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+5] = voxelSize*i;
            m_vertices[coordinatesCount+6] = (voxelSize*j)+voxelSize; m_vertices[coordinatesCount+7] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+8] = voxelSize*i+voxelSize;

            m_vertices[coordinatesCount+9] = (voxelSize*j); m_vertices[coordinatesCount+10] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+11] = voxelSize*i;//triangle 2
            m_vertices[coordinatesCount+12] = (voxelSize*j); m_vertices[coordinatesCount+13] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+14] = voxelSize*i+voxelSize;
            m_vertices[coordinatesCount+15] = (voxelSize*j)+voxelSize; m_vertices[coordinatesCount+16] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+17] = voxelSize*i+voxelSize;

            for (int ii(0);ii<18;ii++)
                m_color[coordinatesCount+ii] = ((float)m_heightMap[i][j]/255.0);

            m_verticesCount+=6;
            coordinatesCount+=18;

            //Left
            if (j>0)
            {
                m_vertices[coordinatesCount] = voxelSize*j; m_vertices[coordinatesCount+1] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+2] = voxelSize*i;//triangle 1
                m_vertices[coordinatesCount+3] = voxelSize*j; m_vertices[coordinatesCount+4] = voxelSize*m_heightMap[i][j-1]; m_vertices[coordinatesCount+5] = voxelSize*i;
                m_vertices[coordinatesCount+6] = voxelSize*j; m_vertices[coordinatesCount+7] = voxelSize*m_heightMap[i][j-1]; m_vertices[coordinatesCount+8] = voxelSize*i+voxelSize;

                m_vertices[coordinatesCount+9] = voxelSize*j; m_vertices[coordinatesCount+10] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+11] = voxelSize*i;//triangle 2
                m_vertices[coordinatesCount+12] = voxelSize*j; m_vertices[coordinatesCount+13] = voxelSize*m_heightMap[i][j-1]; m_vertices[coordinatesCount+14] = voxelSize*i+voxelSize;
                m_vertices[coordinatesCount+15] = voxelSize*j; m_vertices[coordinatesCount+16] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+17] = voxelSize*i+voxelSize;

                m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = ((float)m_heightMap[i][j-1]/255.0);
                m_color[coordinatesCount+6] = m_color[coordinatesCount+7] = m_color[coordinatesCount+8] = ((float)m_heightMap[i][j-1]/255.0);
                m_color[coordinatesCount+9] = m_color[coordinatesCount+10] = m_color[coordinatesCount+11] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+12] = m_color[coordinatesCount+13] = m_color[coordinatesCount+14] = ((float)m_heightMap[i][j-1]/255.0);
                m_color[coordinatesCount+15] = m_color[coordinatesCount+16] = m_color[coordinatesCount+17] = ((float)m_heightMap[i][j]/255.0);


                m_verticesCount+=6;
                coordinatesCount+=18;
            }
            else if (m_heightMap[i][j]>0)
            {
                m_vertices[coordinatesCount] = voxelSize*j; m_vertices[coordinatesCount+1] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+2] = voxelSize*i;//triangle 1
                m_vertices[coordinatesCount+3] = voxelSize*j; m_vertices[coordinatesCount+4] = 0; m_vertices[coordinatesCount+5] = voxelSize*i;
                m_vertices[coordinatesCount+6] = voxelSize*j; m_vertices[coordinatesCount+7] = 0; m_vertices[coordinatesCount+8] = voxelSize*i+voxelSize;

                m_vertices[coordinatesCount+9] = voxelSize*j; m_vertices[coordinatesCount+10] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+11] = voxelSize*i;//triangle 2
                m_vertices[coordinatesCount+12] = voxelSize*j; m_vertices[coordinatesCount+13] = 0; m_vertices[coordinatesCount+14] = voxelSize*i+voxelSize;
                m_vertices[coordinatesCount+15] = voxelSize*j; m_vertices[coordinatesCount+16] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+17] = voxelSize*i+voxelSize;

                m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = 0.0f;
                m_color[coordinatesCount+6] = m_color[coordinatesCount+7] = m_color[coordinatesCount+8] = 0.0f;
                m_color[coordinatesCount+9] = m_color[coordinatesCount+10] = m_color[coordinatesCount+11] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+12] = m_color[coordinatesCount+13] = m_color[coordinatesCount+14] = 0.0f;
                m_color[coordinatesCount+15] = m_color[coordinatesCount+16] = m_color[coordinatesCount+17] = ((float)m_heightMap[i][j]/255.0);

                m_verticesCount+=6;
                coordinatesCount+=18;
            }

            if (i>0)
            {
                m_vertices[coordinatesCount] = voxelSize*j; m_vertices[coordinatesCount+1] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+2] = voxelSize*i;//triangle 1
                m_vertices[coordinatesCount+3] = voxelSize*j; m_vertices[coordinatesCount+4] = voxelSize*m_heightMap[i-1][j]; m_vertices[coordinatesCount+5] = voxelSize*i;
                m_vertices[coordinatesCount+6] = voxelSize*j+voxelSize; m_vertices[coordinatesCount+7] = voxelSize*m_heightMap[i-1][j]; m_vertices[coordinatesCount+8] = voxelSize*i;

                m_vertices[coordinatesCount+9] = voxelSize*j; m_vertices[coordinatesCount+10] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+11] = voxelSize*i;//triangle 2
                m_vertices[coordinatesCount+12] = voxelSize*j+voxelSize; m_vertices[coordinatesCount+13] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+14] = voxelSize*i;
                m_vertices[coordinatesCount+15] = voxelSize*j+voxelSize; m_vertices[coordinatesCount+16] = voxelSize*m_heightMap[i-1][j]; m_vertices[coordinatesCount+17] = voxelSize*i;

                m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = ((float)m_heightMap[i-1][j]/255.0);
                m_color[coordinatesCount+6] = m_color[coordinatesCount+7] = m_color[coordinatesCount+8] = ((float)m_heightMap[i-1][j]/255.0);
                m_color[coordinatesCount+9] = m_color[coordinatesCount+10] = m_color[coordinatesCount+11] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+12] = m_color[coordinatesCount+13] = m_color[coordinatesCount+14] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+15] = m_color[coordinatesCount+16] = m_color[coordinatesCount+17] = ((float)m_heightMap[i-1][j]/255.0);

                m_verticesCount+=6;
                coordinatesCount+=18;
            }
            else if (m_heightMap[i][j]>0)
            {
                m_vertices[coordinatesCount] = voxelSize*j; m_vertices[coordinatesCount+1] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+2] = voxelSize*i;//triangle 1
                m_vertices[coordinatesCount+3] = voxelSize*j; m_vertices[coordinatesCount+4] = 0; m_vertices[coordinatesCount+5] = voxelSize*i;
                m_vertices[coordinatesCount+6] = voxelSize*j+voxelSize; m_vertices[coordinatesCount+7] = 0; m_vertices[coordinatesCount+8] = voxelSize*i;

                m_vertices[coordinatesCount+9] = voxelSize*j; m_vertices[coordinatesCount+10] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+11] = voxelSize*i;//triangle 2
                m_vertices[coordinatesCount+12] = voxelSize*j+voxelSize; m_vertices[coordinatesCount+13] = voxelSize*m_heightMap[i][j]; m_vertices[coordinatesCount+14] = voxelSize*i;
                m_vertices[coordinatesCount+15] = voxelSize*j+voxelSize; m_vertices[coordinatesCount+16] = 0; m_vertices[coordinatesCount+17] = voxelSize*i;

                m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = 0.0f;
                m_color[coordinatesCount+6] = m_color[coordinatesCount+7] = m_color[coordinatesCount+8] = 0.0f;
                m_color[coordinatesCount+9] = m_color[coordinatesCount+10] = m_color[coordinatesCount+11] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+12] = m_color[coordinatesCount+13] = m_color[coordinatesCount+14] = ((float)m_heightMap[i][j]/255.0);
                m_color[coordinatesCount+15] = m_color[coordinatesCount+16] = m_color[coordinatesCount+17] = 0.0f;

                m_verticesCount+=6;
                coordinatesCount+=18;
            }
        }
    }
    if (fuckedColors)
        for (int i(0);i<coordinatesCount;i++)
            m_color[i] = (float)(rand()%256)/255;

    cout << "Heightmap created.\nFile : \"" << toRead << "\"\nSize of one block : " << voxelSize << "\n";
    SDL_FreeSurface(loadHeightMap);
    return true;
}

bool HeightMap::newHeightMapLINES(std::string const& toRead, float voxelSize, bool fuckedColors)
{
    if (m_vertices || m_color || m_heightMap.size())
        deleteHeightMap();

    SDL_Surface *loadHeightMap = IMG_Load(toRead.c_str());

    if (!loadHeightMap)
    {
        cerr << "Error : couldn't load the heightmap. Reason : " << SDL_GetError() << endl;
        return false;
    }

    m_width = loadHeightMap->w;
    m_height = loadHeightMap->h;

    Uint8 getHeight(0);

    m_heightMap.resize(m_height);
    for (int i(0);i<m_height;i++)
    {
        m_heightMap[i].resize(m_width);
    }

    for (int i(0);i<m_height;i++)
    {
        for (int j(0);j<m_width;j++)
        {
            SDL_GetRGB(getpixel(loadHeightMap, j, i), loadHeightMap->format, &getHeight, &getHeight, &getHeight);
            m_heightMap[i][j] = static_cast<int>(getHeight);
        }
    }

    m_vertices = new float[m_height*m_width*48];
    m_color = new float[m_height*m_width*48];
    int coordinatesCount(0);

    float heightXm1(0.0), heightXp1(0.0);
    float heightYm1(0.0), heightYp1(0.0);
    float heightXm1Ym1(0.0), heightXm1Yp1(0.0);
    float heightXp1Ym1(0.0), heightXp1Yp1(0.0);

    m_drawMode = GL_LINES;

    for (int i(0);i<(m_height);i++)
    {
        for (int j(0);j<(m_width);j++)
        {
            if (j>0)
            {
                heightXm1 = (m_heightMap[i][j-1]+m_heightMap[i][j])/2;

                if (i>0)
                    heightXm1Ym1 = (m_heightMap[i-1][j-1]+m_heightMap[i][j])/2;
                else
                    heightXm1Ym1 = 0.0;

                if (i<(m_height-1))
                    heightXm1Yp1 = (m_heightMap[i+1][j-1]+m_heightMap[i][j])/2;
                else
                    heightXm1Yp1 = 0.0;
            }
            else
            {
                heightXm1 = 0.0;
                heightXm1Ym1 = 0.0;
                heightXm1Yp1 = 0.0;
            }

            if (j<(m_width-1))
            {
                heightXp1 = (m_heightMap[i][j+1]+m_heightMap[i][j])/2;

                if (i>0)
                    heightXp1Ym1 = (m_heightMap[i-1][j+1]+m_heightMap[i][j])/2;
                else
                    heightXp1Ym1 = 0.0;

                if (i<(m_height-1))
                    heightXp1Yp1 = (m_heightMap[i+1][j+1]+m_heightMap[i][j])/2;
                else
                    heightXp1Yp1 = 0.0;
            }
            else
            {
                heightXp1 = 0.0;
                heightXp1Ym1 = 0.0;
                heightXp1Yp1 = 0.0;
            }

            if (i>0)
                heightYm1 = (m_heightMap[i-1][j]+m_heightMap[i][j])/2;
            else
                heightYm1 = 0.0;

            if (i<(m_height-1))
                heightYp1 = (m_heightMap[i+1][j]+m_heightMap[i][j])/2;
            else
                heightYp1 = 0.0;

            //Now, let's fill the vertices
            m_vertices[coordinatesCount] = j*voxelSize;
            m_vertices[coordinatesCount+1] = m_heightMap[i][j]*voxelSize;
            m_vertices[coordinatesCount+2] = i*voxelSize;
            m_vertices[coordinatesCount+3] = (j*voxelSize)-(voxelSize/2);
            m_vertices[coordinatesCount+4] = heightXm1*voxelSize;
            m_vertices[coordinatesCount+5] = i*voxelSize;
            m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = m_heightMap[i][j]/255.0;
            m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = heightXm1/255.0;
            coordinatesCount+=6;
            m_verticesCount+=2;

            m_vertices[coordinatesCount] = j*voxelSize;
            m_vertices[coordinatesCount+1] = m_heightMap[i][j]*voxelSize;
            m_vertices[coordinatesCount+2] = i*voxelSize;
            m_vertices[coordinatesCount+3] = (j*voxelSize)+(voxelSize/2);
            m_vertices[coordinatesCount+4] = heightXp1*voxelSize;
            m_vertices[coordinatesCount+5] = i*voxelSize;
            m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = (m_heightMap[i][j])/255.0;
            m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = heightXp1/255.0;
            coordinatesCount+=6;
            m_verticesCount+=2;

            m_vertices[coordinatesCount] = j*voxelSize;
            m_vertices[coordinatesCount+1] = m_heightMap[i][j]*voxelSize;
            m_vertices[coordinatesCount+2] = i*voxelSize;
            m_vertices[coordinatesCount+3] = (j*voxelSize)+(voxelSize/2);
            m_vertices[coordinatesCount+4] = heightXp1Yp1*voxelSize;
            m_vertices[coordinatesCount+5] = (i*voxelSize)+(voxelSize/2);
            m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = (m_heightMap[i][j])/255.0;
            m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = heightXp1Yp1/255.0;
            coordinatesCount+=6;
            m_verticesCount+=2;

            m_vertices[coordinatesCount] = j*voxelSize;
            m_vertices[coordinatesCount+1] = m_heightMap[i][j]*voxelSize;
            m_vertices[coordinatesCount+2] = i*voxelSize;
            m_vertices[coordinatesCount+3] = (j*voxelSize)+(voxelSize/2);
            m_vertices[coordinatesCount+4] = heightXp1Ym1*voxelSize;
            m_vertices[coordinatesCount+5] = (i*voxelSize)-(voxelSize/2);
            m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = (m_heightMap[i][j])/255.0;
            m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = heightXp1Ym1/255.0;
            coordinatesCount+=6;
            m_verticesCount+=2;

            m_vertices[coordinatesCount] = j*voxelSize;
            m_vertices[coordinatesCount+1] = m_heightMap[i][j]*voxelSize;
            m_vertices[coordinatesCount+2] = i*voxelSize;
            m_vertices[coordinatesCount+3] = (j*voxelSize)-(voxelSize/2);
            m_vertices[coordinatesCount+4] = heightXm1Ym1*voxelSize;
            m_vertices[coordinatesCount+5] = (i*voxelSize)-(voxelSize/2);
            m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = (m_heightMap[i][j])/255.0;
            m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = heightXm1Ym1/255.0;
            coordinatesCount+=6;
            m_verticesCount+=2;

            m_vertices[coordinatesCount] = j*voxelSize;
            m_vertices[coordinatesCount+1] = m_heightMap[i][j]*voxelSize;
            m_vertices[coordinatesCount+2] = i*voxelSize;
            m_vertices[coordinatesCount+3] = (j*voxelSize)-(voxelSize/2);
            m_vertices[coordinatesCount+4] = heightXm1Yp1*voxelSize;
            m_vertices[coordinatesCount+5] = (i*voxelSize)+(voxelSize/2);
            m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = (m_heightMap[i][j])/255.0;
            m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = heightXm1Yp1/255.0;
            coordinatesCount+=6;
            m_verticesCount+=2;

            m_vertices[coordinatesCount] = j*voxelSize;
            m_vertices[coordinatesCount+1] = m_heightMap[i][j]*voxelSize;
            m_vertices[coordinatesCount+2] = i*voxelSize;
            m_vertices[coordinatesCount+3] = j*voxelSize;
            m_vertices[coordinatesCount+4] = heightYp1*voxelSize;
            m_vertices[coordinatesCount+5] = (i*voxelSize)+(voxelSize/2);
            m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = (m_heightMap[i][j])/255.0;
            m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = heightYp1/255.0;
            coordinatesCount+=6;
            m_verticesCount+=2;

            m_vertices[coordinatesCount] = j*voxelSize;
            m_vertices[coordinatesCount+1] = m_heightMap[i][j]*voxelSize;
            m_vertices[coordinatesCount+2] = i*voxelSize;
            m_vertices[coordinatesCount+3] = j*voxelSize;
            m_vertices[coordinatesCount+4] = heightYm1*voxelSize;
            m_vertices[coordinatesCount+5] = (i*voxelSize)-(voxelSize/2);
            m_color[coordinatesCount] = m_color[coordinatesCount+1] = m_color[coordinatesCount+2] = (m_heightMap[i][j])/255.0;
            m_color[coordinatesCount+3] = m_color[coordinatesCount+4] = m_color[coordinatesCount+5] = heightYm1/255.0;
            coordinatesCount+=6;
            m_verticesCount+=2;
        }
    }
    if (fuckedColors)
        for (int i(0);i<coordinatesCount;i++)
            m_color[i] = (float)(rand()%256)/255;

    cout << "Heightmap created.\nFile : \"" << toRead << "\"\nSize of one block : " << voxelSize << "\n";
    SDL_FreeSurface(loadHeightMap);
    return true;
}

void HeightMap::deleteHeightMap()
{
    for (int i(m_height-1);i>=0;i--)
    {
        m_heightMap[i].erase(m_heightMap[i].begin(), m_heightMap[i].end());
    }

    m_width = 0;
    m_height = 0;

    delete[] m_vertices;
    m_vertices = 0;

    delete[] m_color;
    m_color = 0;

    m_verticesCount = 0;
}

void HeightMap::displayHeightMap(glm::mat4& modelview, glm::mat4& projection) const
{
    glUseProgram(m_shader.getProgramID());

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, m_color);
        glEnableVertexAttribArray(1);

        glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));

        glDrawArrays(m_drawMode, 0, m_verticesCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

    glUseProgram(0);
}

bool HeightMap::isObstacle(int x, int y, int z) const
{
    if (x>=0 && z>=0 && x<m_width && z<m_width)
    {
        return !(y>m_heightMap[z][x]);
    }
    else
        return false;//Out of the world
}

Uint32 HeightMap::getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
