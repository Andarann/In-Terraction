#ifndef CHUNKMANAGEMENT_HPP_INCLUDED
#define CHUNKMANAGEMENT_HPP_INCLUDED

#define CHUNK_DIMENSIONS 16
#define SINGLE_BLOCK_SIZE 100
#define SINGLE_CHUNK_SIZE CHUNK_DIMENSIONS*SINGLE_BLOCK_SIZE

#include "voxel.hpp"
#include "OpenGL_scene.hpp"
#include "Shader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <stack>
#include <queue>

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

using namespace std;

class Chunk{
public:
    //Constructors
    Chunk();
    Chunk(int x, int y, int z);
    //Destructors
    ~Chunk();
    //Accessors
    Voxel* giveVoxel(int x, int y, int z) const;//Gives voxel's address
    unsigned short giveId(int x, int y, int z) const;////Gives the voxel ID
    bool isEmpty() const;
    bool isBlockSurrounded(int x, int y, int z) const;
    int getX() const;
    int getY() const;
    int getZ() const;
    //Mutators
    void changeVoxelId(int x, int y, int z, unsigned short newId);
    void changeVoxelPtr(int x, int y, int z, Voxel* toAdd);
    void fillChunkId(unsigned short newId);
    void fillChunkVoxel(Voxel* toAdd);
    void setChunkPosition(int x, int y, int z);
    void randomizeChunk();
    void removeFace(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD);
    //File I/O
    void writeInFile(string const& file);
    void readChunk(string const& filePath);
    //OpenGL part
    void showChunk(glm::mat4& projection, glm::mat4& modelview, Texture const& textureAtlas, Shader const& shaderTexture);
    void deleteMesh();
    void addVertex(float x, float y, float z);
    void addTextureVertex(float x, float y);
private:
    int m_x, m_y, m_z;
    Voxel* m_data[CHUNK_DIMENSIONS][CHUNK_DIMENSIONS][CHUNK_DIMENSIONS];
    unsigned int m_numberOfVoxels;//Useful to know if whether or not the chunk is empty.

    //Mesh data
    vector<float> m_vertices;
    vector<float> m_texturesCoords;
};

class ChunkManager{//This class with handle all the chunks loading/saving/displaying etc.
public:
    //Constructors
    ChunkManager(int range, glm::vec3 cameraPosition);
    ~ChunkManager();
    //Accessors
    Voxel* checkVoxelPtr(int x, int y, int z) const;
    unsigned short checkVoxelId(int x, int y, int z) const;
    unsigned short checkVoxelIdDEBUG(int x, int y, int z) const;
    //Mutators
    void setVoxelId(int x, int y, int z, unsigned short toPut);
    void setVoxelPtr(int x, int y, int z, Voxel* toPut);
    void createMesh(Chunk& toMesh);
    void loadChunks();

    void update(glm::vec3 cameraPosition, glm::vec3 lookedPoint);
    void displayChunks(glm::mat4& projection, glm::mat4& modelview, Texture const& textureAtlas);
private:
    vector<Chunk> m_allChunks;//All the loaded chunks. Unloaded when the players gets too away
    stack<glm::vec3> m_toLoad;//Both these stacks will be used in a multithreaded loop to smoothly control chunk-loading/unloading

    int m_numberLoadedChunks;
    queue<Chunk> m_loadedChunk;
    //stack< vector<Chunk>* > m_toUnload;

    int m_range;//For loading chunks
    int m_viewRange;//For viewing chunks
    Shader m_shader;
    boost::thread loadChunksThread;

    int m_centerX, m_centerY, m_centerZ;//Coordinates of the bottom right back chunk
};

void performanceTest_CHUNK();

#endif // CHUNKMANAGEMENT_HPP_INCLUDED
