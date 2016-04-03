
#include "chunkManagement.hpp"

using namespace std;

Voxel* const VOXEL_AIR_POINTER=(voxelList::Instance()->getPointer(AIR));

Chunk::Chunk() : m_x(0), m_y(0), m_z(0), m_numberOfVoxels(0)
{
    for (int i(0);i<CHUNK_DIMENSIONS;i++)
    {
        for (int j(0);j<CHUNK_DIMENSIONS;j++)
        {
            for (int k(0);k<CHUNK_DIMENSIONS;k++)
            {
                m_data[i][j][k] = VOXEL_AIR_POINTER;
            }
        }
    }
    glGenBuffers(1, &m_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    //glBufferData(GL_ARRAY_BUFFER, )
}

Chunk::Chunk(int x, int y, int z) : m_x(x), m_y(y), m_z(z), m_numberOfVoxels(0)
{
    for (int i(0);i<CHUNK_DIMENSIONS;i++)
    {
        for (int j(0);j<CHUNK_DIMENSIONS;j++)
        {
            for (int k(0);k<CHUNK_DIMENSIONS;k++)
            {
                m_data[i][j][k] = VOXEL_AIR_POINTER;
            }
        }
    }
}

Chunk::~Chunk()
{}

Voxel* Chunk::giveVoxel(int x, int y, int z) const
{
    if (x>=CHUNK_DIMENSIONS || y>=CHUNK_DIMENSIONS || z>=CHUNK_DIMENSIONS || x<0 || y<0 || z<0)
    {
        cerr << "Error : couldn't give voxel at coordinates (" << x << "," << y << "," << z
        << "). Reason : coordinates not in the chunk." << endl;
        exit(EXIT_FAILURE);
    }

    return m_data[z][y][x];
}

unsigned short Chunk::giveId(int x, int y, int z) const
{
    if (x>=CHUNK_DIMENSIONS || y>=CHUNK_DIMENSIONS || z>=CHUNK_DIMENSIONS || x<0 || y<0 || z<0)
    {
        cerr << "Error : couldn't give voxel at coordinates (" << x << "," << y << "," << z
        << "). Reason : coordinates not in the chunk." << endl;
        exit(EXIT_FAILURE);
    }

    return m_data[z][y][x]->blockId();
}

bool Chunk::isEmpty() const
{
    return (m_numberOfVoxels == 0);
}

void Chunk::changeVoxelId(int x, int y, int z, unsigned short newId)
{
    if (x>=CHUNK_DIMENSIONS || y>=CHUNK_DIMENSIONS || z>=CHUNK_DIMENSIONS || x<0 || y<0 || z<0)
    {
        cerr << "Error : couldn't place voxel of ID " << newId << "at coordinates (" << x << "," << y << "," << z
        << "). Reason : coordinates not in the chunk." << endl;
        exit(EXIT_FAILURE);
    }

    Voxel* toAdd = voxelList::Instance()->getPointer(newId);

    if (toAdd)
    {
        if (toAdd != m_data[z][y][x])//Useless to replace a voxel bu itself
        {
            if (toAdd == VOXEL_AIR_POINTER)
                m_numberOfVoxels--;
            else if (m_data[z][y][x] == VOXEL_AIR_POINTER)
                m_numberOfVoxels++;

            m_data[z][y][x] = toAdd;
        }
    }
    else
    {
        cerr << "Error : couldn't place voxel of ID " << newId << " at coordinates (" << x << "," << y << "," << z
        << "). Reason : voxel of ID " << newId << " not found/does not exist." << endl;
        exit(EXIT_FAILURE);
    }
}

void Chunk::changeVoxelPtr(int x, int y, int z, Voxel* toAdd)
{
    if (x>=CHUNK_DIMENSIONS || y>=CHUNK_DIMENSIONS || z>=CHUNK_DIMENSIONS || x<0 || y<0 || z<0)
    {
        cerr << "Error : couldn't place voxel pointer at coordinates (" << x << "," << y << "," << z
        << "). Reason : coordinates not in the chunk." << endl;
        exit(EXIT_FAILURE);
    }

    if (toAdd)
    {
        if (toAdd != m_data[z][y][x])//Useless to replace a voxel bu itself
        {
            if (toAdd == VOXEL_AIR_POINTER)
                m_numberOfVoxels--;
            else if (m_data[z][y][x] == VOXEL_AIR_POINTER)//It replaces air
                m_numberOfVoxels++;

            m_data[z][y][x] = toAdd;
        }
    }
    else
    {
        cerr << "Error : couldn't place voxel pointer at coordinates (" << x << "," << y << "," << z
        << "). Reason : null pointer sent" << endl;
        exit(EXIT_FAILURE);
    }
}

void Chunk::fillChunkId(unsigned short newId)
{
    Voxel* toAdd = voxelList::Instance()->getPointer(newId);

    if (toAdd)
    {
        int i,j,k;
        for (i=0;i<CHUNK_DIMENSIONS;i++)
        {
            for (j=0;j<CHUNK_DIMENSIONS;j++)
            {
                for (k=0;k<CHUNK_DIMENSIONS;k++)
                {
                    if (toAdd != m_data[i][j][k])//Useless to replace a voxel bu itself
                    {
                        if (toAdd == VOXEL_AIR_POINTER)
                            m_numberOfVoxels--;
                        else if (m_data[i][j][k] == VOXEL_AIR_POINTER)//It replaces air
                            m_numberOfVoxels++;

                        m_data[i][j][k] = toAdd;
                    }
                }
            }
        }
    }
    else
    {
        cerr << "Error : could not fill chunk with voxels of ID " << newId
        << ". Reason : voxel of ID " << newId << " not found/does not exist" << endl;
        exit(EXIT_FAILURE);
    }
}

void Chunk::fillChunkVoxel(Voxel* toAdd)
{
    if (toAdd)
    {
        int i,j,k;
        for (i=0;i<CHUNK_DIMENSIONS;i++)
        {
            for (j=0;j<CHUNK_DIMENSIONS;j++)
            {
                for (k=0;k<CHUNK_DIMENSIONS;k++)
                {
                    if (toAdd != m_data[i][j][k])//Useless to replace a voxel bu itself
                    {
                        if (toAdd == VOXEL_AIR_POINTER)
                            m_numberOfVoxels--;
                        else if (m_data[i][j][k] == VOXEL_AIR_POINTER)//It replaces air
                            m_numberOfVoxels++;

                        m_data[i][j][k] = toAdd;
                    }
                }
            }
        }
    }
    else
    {
        cerr << "Error : couldn't fill chunk. Reason : null pointer sent" << endl;
        exit(EXIT_FAILURE);
    }
}

void Chunk::setChunkPosition(int x, int y, int z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void Chunk::writeInFile(string const& file)
{
    ofstream saveFile(file.c_str());

    unsigned short lastId(m_data[0][0][0]->blockId());
    int occurenceCount(0);

    if (saveFile)
    {
        //cout << "Ouverture du fichier réussie; sauvegarde..." << endl;
        saveFile << "Number of voxels : " << m_numberOfVoxels << '\n';
        for (int i(0);i<CHUNK_DIMENSIONS;i++)
        {
            for (int j(0);j<CHUNK_DIMENSIONS;j++)
            {
                for (int k(0);k<CHUNK_DIMENSIONS;k++)
                {
                    if (m_data[i][j][k]->blockId() == lastId)//While the block is the same, we count the number of repetitions
                    {
                        occurenceCount++;
                    }
                    else//When it does not repeat anymore, we write the number of occurencies
                    {
                        saveFile << lastId;

                        if (occurenceCount > 1)//Block has been repeated
                        {
                            saveFile << "x" << occurenceCount << " ";
                        }
                        else
                        {
                            saveFile << " ";
                        }
                        lastId = m_data[i][j][k]->blockId();
                        occurenceCount = 1;
                    }
                }
            }
        }
        saveFile << lastId;
        if (occurenceCount > 1)//Block has been repeated
        {
            saveFile << "x" << occurenceCount << " ";
        }
    }
}

void Chunk::readChunk(string const& filePath)
{
    cout << "Write the reading chunk code dumbass\n";
}

void Chunk::randomizeChunk()
{
    int maxId = (voxelList::Instance()->getMaxId())+1;
    for (int i(0);i<CHUNK_DIMENSIONS;i++)
    {
        for (int j(0);j<CHUNK_DIMENSIONS;j++)
        {
            for (int k(0);k<CHUNK_DIMENSIONS;k++)
            {
                changeVoxelId(k,j,i,rand()%maxId);
            }
        }
    }
}

void Chunk::showChunk(glm::mat4& projection, glm::mat4& modelview, Texture const& textureAtlas, Shader const& shaderTexture)
{
    glm::mat4 tempMatrix;
    tempMatrix = modelview;
    modelview = glm::scale(modelview, glm::vec3(SINGLE_BLOCK_SIZE,SINGLE_BLOCK_SIZE,SINGLE_BLOCK_SIZE));

    glUseProgram(shaderTexture.getProgramID());

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices.data());

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, m_texturesCoords.data());

        glUniformMatrix4fv(glGetUniformLocation(shaderTexture.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderTexture.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));

        glTexCoordPointer(2, GL_FLOAT, 0, m_texturesCoords.data());
        glVertexPointer(3, GL_FLOAT, 0, m_vertices.data());

        glBindTexture(GL_TEXTURE_2D, textureAtlas.getID());
        glDrawArrays(GL_QUADS, 0, m_vertices.size()/3);
        glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);

    modelview = tempMatrix;
}

bool Chunk::isBlockSurrounded(int x, int y, int z) const
{
    if (x>=0 && x<=CHUNK_DIMENSIONS && y>=0 && y<=CHUNK_DIMENSIONS && z>=0 && z<=CHUNK_DIMENSIONS)
    {
    //Back
    if (y>0)
    {
        if (m_data[z][y-1][x] == VOXEL_AIR_POINTER)
        {
            return false;
        }
    }
    else
        return false;

    //Top
    if (y<CHUNK_DIMENSIONS-1)
    {
        if (m_data[z][y+1][x] == VOXEL_AIR_POINTER)
        {
            return false;
        }
    }
    else
        return false;

    //Left
    if (z>0)
    {
        if (m_data[z-1][y][x] == VOXEL_AIR_POINTER)
        {
            return false;
        }
    }
    else
        return false;

    //Right
    if (z<CHUNK_DIMENSIONS-1)
    {
        if (m_data[z+1][y][x] == VOXEL_AIR_POINTER)
        {
            return false;
        }
    }
    else
        return false;

    //Back
    if (x>0)
    {
        if (m_data[z][y][x-1] == VOXEL_AIR_POINTER)
        {
            return false;
        }
    }
    else
        return false;

    //Front
    if (x<CHUNK_DIMENSIONS-1)
    {
        if (m_data[z][y][x+1] == VOXEL_AIR_POINTER)
        {
            return false;
        }
    }
    else
        return false;
    }
    else
        std::cout << "Bad coordinates sent (" << x << "," << y << "," << z << ") to function isBlockSurrounded(int x, int y, int z)" << endl;

    return true;
}

void Chunk::deleteMesh()
{
    m_vertices.clear();
    m_texturesCoords.clear();
}

void Chunk::addVertex(float x, float y, float z)
{
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);
}

void Chunk::addTextureVertex(float x, float y)
{
    m_texturesCoords.push_back(x);
    m_texturesCoords.push_back(y);
}

void ChunkManager::createMesh(Chunk& toMesh)
{
    toMesh.deleteMesh();

    int const chunkX(toMesh.getX()*CHUNK_DIMENSIONS), chunkY(toMesh.getY()*CHUNK_DIMENSIONS), chunkZ(toMesh.getZ()*CHUNK_DIMENSIONS);

    if (!toMesh.isEmpty())
    {
    for (int i(0);i<CHUNK_DIMENSIONS;i++)//z
    {
        for (int j(0);j<CHUNK_DIMENSIONS;j++)//y
        {
            for (int k(0);k<CHUNK_DIMENSIONS;k++)//x
            {
                if (toMesh.giveVoxel(k,j,i) != VOXEL_AIR_POINTER)
                {
                    //Bottom
                    if (j>0)
                    {
                        if (toMesh.giveVoxel(k,j-1,i) == VOXEL_AIR_POINTER )
                        {
                            toMesh.addVertex(k+chunkX,j+chunkY,i+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+chunkY,i+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+chunkX,j+chunkY,i+1+chunkZ);

                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());

                        }
                    }
                    else
                    {
                        toMesh.addVertex(k+chunkX,j+chunkY,i+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+chunkY,i+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+chunkX,j+chunkY,i+1+chunkZ);

                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                    }

                    //Top
                    if (j<CHUNK_DIMENSIONS-1)
                    {

                        if (toMesh.giveVoxel(k,j+1,i) == VOXEL_AIR_POINTER)
                        {
                            toMesh.addVertex(k+chunkX,j+1+chunkY,i+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+chunkX,j+1+chunkY,i+1+chunkZ);

                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                        }
                    }
                    else
                    {
                        toMesh.addVertex(k+chunkX,j+1+chunkY,i+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+chunkX,j+1+chunkY,i+1+chunkZ);

                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                    }

                    //Left
                    if (i>0)
                    {
                        if (toMesh.giveVoxel(k,j,i-1) == VOXEL_AIR_POINTER)
                        {
                            toMesh.addVertex(k+chunkX,j+chunkY,i+chunkZ);
                            toMesh.addVertex(k+chunkX,j+1+chunkY,i+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+chunkY,i+chunkZ);

                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                        }
                    }
                    else
                    {
                        toMesh.addVertex(k+chunkX,j+chunkY,i+chunkZ);
                        toMesh.addVertex(k+chunkX,j+1+chunkY,i+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+chunkY,i+chunkZ);

                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                    }

                    //Right
                    if (i<CHUNK_DIMENSIONS-1)
                    {
                        if (toMesh.giveVoxel(k,j,i+1) == VOXEL_AIR_POINTER)
                        {
                            toMesh.addVertex(k+chunkX,j+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+chunkX,j+1+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+chunkY,i+1+chunkZ);

                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                        }
                    }
                    else
                    {
                        toMesh.addVertex(k+chunkX,j+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+chunkX,j+1+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+chunkY,i+1+chunkZ);

                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                    }

                    if (k<CHUNK_DIMENSIONS-1)
                    {
                        if (toMesh.giveVoxel(k+1,j,i) == VOXEL_AIR_POINTER)
                        {
                            toMesh.addVertex(k+1+chunkX,j+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+chunkZ);
                            toMesh.addVertex(k+1+chunkX,j+chunkY,i+chunkZ);

                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                        }
                    }
                    else
                    {
                        toMesh.addVertex(k+1+chunkX,j+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+1+chunkY,i+chunkZ);
                        toMesh.addVertex(k+1+chunkX,j+chunkY,i+chunkZ);

                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                    }

                    if (k>0)
                    {
                        if (toMesh.giveVoxel(k-1,j,i) == VOXEL_AIR_POINTER )
                        {
                            toMesh.addVertex(k+chunkX,j+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+chunkX,j+1+chunkY,i+1+chunkZ);
                            toMesh.addVertex(k+chunkX,j+1+chunkY,i+chunkZ);
                            toMesh.addVertex(k+chunkX,j+chunkY,i+chunkZ);

                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                            toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                        }
                    }
                    else
                    {
                        toMesh.addVertex(k+chunkX,j+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+chunkX,j+1+chunkY,i+1+chunkZ);
                        toMesh.addVertex(k+chunkX,j+1+chunkY,i+chunkZ);
                        toMesh.addVertex(k+chunkX,j+chunkY,i+chunkZ);

                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyA());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxA(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyB());
                        toMesh.addTextureVertex(toMesh.giveVoxel(k,j,i)->getxB(), toMesh.giveVoxel(k,j,i)->getyA());
                    }
                }
            }
        }
    }
    }
}

void Chunk::removeFace(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD)
{
    for (int i(0);i<m_vertices.size();i+=12)
    {
        if (m_vertices[i] == pointA.x && m_vertices[i+1] == pointA.y && m_vertices[i+2] == pointA.z &&
            m_vertices[i+3] == pointB.x && m_vertices[i+4] == pointB.y && m_vertices[i+5] == pointB.z &&
            m_vertices[i+6] == pointC.x && m_vertices[i+7] == pointC.y && m_vertices[i+8] == pointC.z &&
            m_vertices[i+9] == pointD.x && m_vertices[i+10] == pointD.y && m_vertices[i+11] == pointD.z)
        {
            m_vertices.erase(m_vertices.begin()+i,m_vertices.begin()+i+11);
            m_texturesCoords.erase(m_texturesCoords.begin()+i,m_texturesCoords.begin()+i+11);
            break;
        }
    }
}

int Chunk::getX() const
{
    return m_x;
}

int Chunk::getY() const
{
    return m_y;
}

int Chunk::getZ() const
{
    return m_z;
}

void performanceTest_CHUNK()
{
    Chunk testDummy(0,0,0);
    ofstream performanceLog("performanceChunk.log");

    if (performanceLog)
    {
        performanceLog << "Let's begin the tests (CLOCK_PER_SECONDS = " << CLOCKS_PER_SEC << ")" << endl;
        performanceLog << "Chunk size : " << CHUNK_DIMENSIONS << "x" << CHUNK_DIMENSIONS << "x" << CHUNK_DIMENSIONS << " voxels\n\n";

        int i, timer, getEnd;
        int testNumber(0);

        /**<  Test beginning */
        testNumber++;
        performanceLog << "Test #" << testNumber << " : Save empty chunk" << endl;
        i = 0;
        cout << "Beginning test #" << testNumber << "...This may take a while !" << endl;
        timer = clock();

        for (i=0;i<1000;i++)
        {
            /**< Test what you need to test here */
            testDummy.writeInFile("testDummy.txt");
        }

        getEnd = clock();
        cout << "Test #" << testNumber << " over !" << endl;
        performanceLog << "    Time for " << i << " writes : " << getEnd-timer << " ms" << endl;
        performanceLog << "    Average time for test #"<< testNumber <<" : " << (double)(getEnd-timer)/i << " ms\n\n";
        /**<  Test end */

        /**<  Test beginning */
        testNumber++;
        performanceLog << "Test #" << testNumber << " : Fill a chunk (manual)" << endl;
        i = 0;
        cout << "Beginning test #" << testNumber << "...This may take a while !" << endl;
        timer = clock();

        for (i=0;i<1000;i++)
        {
            /**< Test what you need to test here */
            for (int j(0);j<CHUNK_DIMENSIONS;j++)
            {
                for (int k(0);k<CHUNK_DIMENSIONS;k++)
                {
                    for (int l(0);l<CHUNK_DIMENSIONS;l++)
                    {
                        testDummy.changeVoxelId(j,k,l,5);
                    }
                }
            }
        }

        getEnd = clock();
        cout << "Test #" << testNumber << " over !" << endl;
        performanceLog << "    Time for " << i << " filling : " << getEnd-timer << " ms" << endl;
        performanceLog << "    Average time for test #"<< testNumber <<" : " << (double)(getEnd-timer)/i << " ms\n\n";
        /**< Test end */

        /**<  Test beginning */
        testNumber++;
        performanceLog << "Test #" << testNumber << " : Fill a chunk (auto)" << endl;
        i = 0;
        cout << "Beginning test #" << testNumber << "...This may take a while !" << endl;
        timer = clock();

        for (i=0;i<1000;i++)
        {
            /**< Test what you need to test here */
            testDummy.fillChunkId(4);
        }

        getEnd = clock();
        cout << "Test #" << testNumber << " over !" << endl;
        performanceLog << "    Time for " << i << " filling : " << getEnd-timer << " ms" << endl;
        performanceLog << "    Average time for test #"<< testNumber <<" : " << (double)(getEnd-timer)/i << " ms\n\n";
        /**< Test end */

        cout << "Filling a chunk for test #" << testNumber+1 << ", this can take a while" << endl;
        for (int j(0);j<CHUNK_DIMENSIONS;j++)
        {
            for (int k(0);k<CHUNK_DIMENSIONS;k++)
            {
                for (int l(0);l<CHUNK_DIMENSIONS;l++)
                {
                    testDummy.changeVoxelId(j,k,l,(j*CHUNK_DIMENSIONS*CHUNK_DIMENSIONS+k*CHUNK_DIMENSIONS+l)%6);
                }
            }
        }
        cout << "Filling done !\n";
        /**<  Test beginning */
        testNumber++;
        performanceLog << "Test #" << testNumber << " : Save a full chunk" << endl;
        i = 0;
        cout << "Beginning test #" << testNumber << "...This may take a while !" << endl;
        timer = clock();

        for (i=0;i<1000;i++)
        {
            testDummy.writeInFile("testDummy.txt");
        }

        getEnd = clock();
        cout << "Test #" << testNumber << " over !" << endl;
        performanceLog << "    Time for " << i << " saving : " << getEnd-timer << " ms" << endl;
        performanceLog << "    Average time for test #"<< testNumber <<" : " << (double)(getEnd-timer)/i << " ms\n\n";
        /**< Test end */
    }
    else
    {
        cerr << "Error : could not open file for chunk performance test" << endl;
        exit(EXIT_FAILURE);
    }
}

ChunkManager::ChunkManager(int range, glm::vec3 cameraPosition) : m_range(range), m_shader(Shader("Shaders/texture.vert", "Shaders/texture.frag")),
                                                                  loadChunksThread(boost::thread(boost::bind(&ChunkManager::loadChunks, this)))
{
    //Here, we create a new chunk manager, loading chunk for a given radius
    int x = static_cast<int>(cameraPosition.x)/(CHUNK_DIMENSIONS*SINGLE_BLOCK_SIZE);
    int y = static_cast<int>(cameraPosition.y)/(CHUNK_DIMENSIONS*SINGLE_BLOCK_SIZE);
    int z = static_cast<int>(cameraPosition.z)/(CHUNK_DIMENSIONS*SINGLE_BLOCK_SIZE);

    m_centerX = x;
    m_centerY = y;
    m_centerZ = z;

    int waitIterator(0);
    //Now, let's generate ((2*m_range)+1)^3 chunks
    for (int i(-m_range);i<=m_range;i++)//z
    {
        for (int j(-m_range);j<=m_range;j++)//y
        {
            for (int k(-m_range);k<=m_range;k++)//x
            {
                //m_allChunks.push_back(Chunk());
                m_toLoad.push(glm::vec3(x+k,y+j,z+i));
                //cout << m_toLoad.size() << '\n';
                //m_allChunks[waitIterator].setChunkPosition(x+k,y+j,z+i);
                /*if (y+j>=0)
                {
                    m_allChunks[waitIterator].fillChunkId(0);
                }
                else
                {
                    m_allChunks[waitIterator].fillChunkId(3);
                }

                //cout << "(" << m_allChunks[waitIterator].getX() << "," << m_allChunks[waitIterator].getY() << "," << m_allChunks[waitIterator].getZ() << ")\n";

                waitIterator++;*/
            }
        }
    }
    /*for (int i(0);i<m_allChunks.size();i++)
    {
        createMesh(m_allChunks[i]);
    }*/
}

void ChunkManager::loadChunks()
{
    glm::vec3 tempToLoad(0,0,0);
    int currentChunkSize(0);
    int y(0);
    Chunk chunkToLoad;
    m_numberLoadedChunks = 0;


    while (1)
    {
        if (!m_toLoad.empty())
        {
            tempToLoad = m_toLoad.top();
            m_toLoad.pop();

            if (abs(tempToLoad.x-m_centerX)>m_range);
            else if (abs(tempToLoad.y-m_centerY)>m_range);
            else if (abs(tempToLoad.z-m_centerZ)>m_range);
            else
            {
                chunkToLoad = Chunk();
                chunkToLoad.setChunkPosition(tempToLoad.x, tempToLoad.y, tempToLoad.z);
                if (tempToLoad.y>=0)
                {
                    chunkToLoad.fillChunkId(0);
                }
                else
                {
                    chunkToLoad.fillChunkId(3);
                }
                createMesh(chunkToLoad);

                //while (m_chunkReady);//Wait for the loaded chunk to be loaded in the true list
                m_loadedChunk.push(chunkToLoad);
                m_numberLoadedChunks++;
                //m_loadedChunk = chunkToLoad;
                //m_chunkReady = true;
            }
        }
    }
}

ChunkManager::~ChunkManager()
{

}

Voxel* ChunkManager::checkVoxelPtr(int x, int y, int z) const
{
    int ChunkX(x/CHUNK_DIMENSIONS-((x%CHUNK_DIMENSIONS)<0)),
        ChunkY(y/CHUNK_DIMENSIONS-((y%CHUNK_DIMENSIONS)<0)),
        ChunkZ(z/CHUNK_DIMENSIONS-((z%CHUNK_DIMENSIONS)<0));

    int inChunkX(x%CHUNK_DIMENSIONS+(((x%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkY(y%CHUNK_DIMENSIONS+(((y%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkZ(z%CHUNK_DIMENSIONS+(((z%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS));

    for (int i(0);i<m_allChunks.size();i++)
    {
        if ((m_allChunks[i].getX()==ChunkX) && (m_allChunks[i].getY()==ChunkY) && (m_allChunks[i].getZ()==ChunkZ))
        {
            return m_allChunks[i].giveVoxel(inChunkX, inChunkY, inChunkZ);
        }
    }

    return VOXEL_AIR_POINTER;//if the block isn't loaded, return air
}

unsigned short ChunkManager::checkVoxelId(int x, int y, int z) const
{
    int ChunkX(x/CHUNK_DIMENSIONS-((x%CHUNK_DIMENSIONS)<0)),
        ChunkY(y/CHUNK_DIMENSIONS-((y%CHUNK_DIMENSIONS)<0)),
        ChunkZ(z/CHUNK_DIMENSIONS-((z%CHUNK_DIMENSIONS)<0));

    int inChunkX(x%CHUNK_DIMENSIONS+(((x%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkY(y%CHUNK_DIMENSIONS+(((y%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkZ(z%CHUNK_DIMENSIONS+(((z%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS));

    /*cout << "Chunk (" << ChunkX << "," << ChunkY << "," << ChunkZ << ")\n";
    cout << "Block (" << inChunkX << "," << inChunkY << "," << inChunkZ << ")\n";*/

    for (int i(0);i<m_allChunks.size();i++)
    {
        if ((m_allChunks[i].getX()==ChunkX) && (m_allChunks[i].getY()==ChunkY) && (m_allChunks[i].getZ()==ChunkZ))
        {
            return m_allChunks[i].giveId(inChunkX, inChunkY, inChunkZ);
        }
    }

    return AIR;//if the block isn't loaded, return air
}

unsigned short ChunkManager::checkVoxelIdDEBUG(int x, int y, int z) const
{
    int ChunkX(x/CHUNK_DIMENSIONS-((x%CHUNK_DIMENSIONS)<0)),
        ChunkY(y/CHUNK_DIMENSIONS-((y%CHUNK_DIMENSIONS)<0)),
        ChunkZ(z/CHUNK_DIMENSIONS-((z%CHUNK_DIMENSIONS)<0));

    int inChunkX(x%CHUNK_DIMENSIONS+(((x%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkY(y%CHUNK_DIMENSIONS+(((y%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkZ(z%CHUNK_DIMENSIONS+(((z%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS));

    cout << "Chunk (" << ChunkX << "," << ChunkY << "," << ChunkZ << ")\n";
    cout << "Block (" << inChunkX << "," << inChunkY << "," << inChunkZ << ")\n";

    for (int i(0);i<m_allChunks.size();i++)
    {
        if ((m_allChunks[i].getX()==ChunkX) && (m_allChunks[i].getY()==ChunkY) && (m_allChunks[i].getZ()==ChunkZ))
        {
            return m_allChunks[i].giveId(inChunkX, inChunkY, inChunkZ);
        }
    }

    return AIR;//if the block isn't loaded, return air
}

void ChunkManager::setVoxelId(int x, int y, int z, unsigned short toPut)
{
    int ChunkX(x/CHUNK_DIMENSIONS-((x%CHUNK_DIMENSIONS)<0)),
        ChunkY(y/CHUNK_DIMENSIONS-((y%CHUNK_DIMENSIONS)<0)),
        ChunkZ(z/CHUNK_DIMENSIONS-((z%CHUNK_DIMENSIONS)<0));

    int inChunkX(x%CHUNK_DIMENSIONS+(((x%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkY(y%CHUNK_DIMENSIONS+(((y%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkZ(z%CHUNK_DIMENSIONS+(((z%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS));

    /*cout << "Get (chunk) : (" << ChunkX << "," << ChunkY << "," << ChunkZ << ")\n";
    cout << "In chunk : (" << inChunkX << "," << inChunkY << "," << inChunkZ << ")\n";*/

    for (int i(0);i<m_allChunks.size();i++)
    {
        if ((m_allChunks[i].getX()==ChunkX) && (m_allChunks[i].getY()==ChunkY) && (m_allChunks[i].getZ()==ChunkZ))
        {
            m_allChunks[i].changeVoxelId(inChunkX, inChunkY, inChunkZ, toPut);
            createMesh(m_allChunks[i]);
            break;
        }
    }
}

void ChunkManager::setVoxelPtr(int x, int y, int z, Voxel* toPut)
{
    int ChunkX(x/CHUNK_DIMENSIONS-((x%CHUNK_DIMENSIONS)<0)),
        ChunkY(y/CHUNK_DIMENSIONS-((y%CHUNK_DIMENSIONS)<0)),
        ChunkZ(z/CHUNK_DIMENSIONS-((z%CHUNK_DIMENSIONS)<0));

    int inChunkX(x%CHUNK_DIMENSIONS+(((x%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkY(y%CHUNK_DIMENSIONS+(((y%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS)),
        inChunkZ(z%CHUNK_DIMENSIONS+(((z%CHUNK_DIMENSIONS)<0)*CHUNK_DIMENSIONS));

    for (int i(0);i<m_allChunks.size();i++)
    {
        if ((m_allChunks[i].getX()==ChunkX) && (m_allChunks[i].getY()==ChunkY) && (m_allChunks[i].getZ()==ChunkZ))
        {
            m_allChunks[i].changeVoxelPtr(inChunkX, inChunkY, inChunkZ, toPut);
            createMesh(m_allChunks[i]);
            break;
        }
    }
}

void ChunkManager::update(glm::vec3 cameraPosition, glm::vec3 lookedPoint)
{
    int saveX(m_centerX), saveY(m_centerY), saveZ(m_centerZ);

    int x(cameraPosition.x/SINGLE_BLOCK_SIZE), y(cameraPosition.y/SINGLE_BLOCK_SIZE), z(cameraPosition.z/SINGLE_BLOCK_SIZE);

    m_centerX = (x/CHUNK_DIMENSIONS-((x%CHUNK_DIMENSIONS)<0));
    m_centerY = (y/CHUNK_DIMENSIONS-((y%CHUNK_DIMENSIONS)<0));
    m_centerZ = (z/CHUNK_DIMENSIONS-((z%CHUNK_DIMENSIONS)<0));


    vector<glm::vec3> chunksNeededPositions;
    int l(0);
    for (int i(m_centerZ-m_range);i<=m_centerZ+m_range;i++)
    {
        for (int j(m_centerY-m_range);j<=m_centerY+m_range;j++)
        {
            for (int k(m_centerX-m_range);k<=m_centerX+m_range;k++)
            {
                chunksNeededPositions.push_back(glm::vec3(k,j,i));
            }
        }
    }

    if (m_numberLoadedChunks)
    {
        m_allChunks.push_back(m_loadedChunk.front());
        m_loadedChunk.pop();
        m_numberLoadedChunks--;
        /*m_allChunks.push_back(m_loadedChunk);
        m_chunkReady = false;*/
    }

    for (int i(0);i<m_allChunks.size();i++)
    {
        if (abs(m_allChunks[i].getX()-m_centerX)>m_range)
        {
            m_allChunks.erase(m_allChunks.begin()+i);
        }
        else if (abs(m_allChunks[i].getY()-m_centerY)>m_range)
        {
            m_allChunks.erase(m_allChunks.begin()+i);
        }
        else if (abs(m_allChunks[i].getZ()-m_centerZ)>m_range)
        {
            m_allChunks.erase(m_allChunks.begin()+i);
        }
    }
}

void ChunkManager::displayChunks(glm::mat4& projection, glm::mat4& modelview, Texture const& textureAtlas)
{
    for (int i(0);i<m_allChunks.size();i++)
    {
        if (!(m_allChunks[i].isEmpty()))//don't render empty chunks (emptyness = all blocks are air)
        {
            //Let's do the frustum culling test
            m_allChunks[i].showChunk(projection, modelview, textureAtlas, m_shader);
        }
    }
}
