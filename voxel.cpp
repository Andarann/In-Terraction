
#include "voxel.hpp"

// Global static pointer used to ensure a single instance of the class.
voxelList* voxelList::allVoxels = NULL;


Voxel::Voxel() : m_Id(0)
{
    setTextureCoordinates(0.0f, 0.0f, 0.0f, 0.0f);
}

Voxel::Voxel(unsigned short Id) : m_Id(Id)
{
    setTextureCoordinates(0.0f, 0.0f, 0.0f, 0.0f);
}

Voxel::Voxel(unsigned short Id, float xA, float yA, float xB, float yB) : m_Id(Id)
{
    setTextureCoordinates(xA, yA, xB, yB);
}

Voxel::~Voxel()
{}

unsigned short Voxel::blockId() const
{
    return m_Id;
}

void Voxel::setTextureCoordinates(float xA, float yA, float xB, float yB)
{
    m_xA = xA;
    m_yA = yA;
    m_xB = xB;
    m_yB = yB;
}

float Voxel::getxA() const
{
    return m_xA;
}

float Voxel::getyA() const
{
    return m_yA;
}

float Voxel::getxB() const
{
    return m_xB;
}

float Voxel::getyB() const
{
    return m_yB;
}
voxelList::voxelList()
{
    m_textureAtlas.loadTexture("Textures/terrain.png");
    voxelCreated.push_back(new Voxel(AIR, 0,0,0,0));
    voxelCreated.push_back(new Voxel(1, 80, 48, 96, 64));
    voxelCreated.push_back(new Voxel(2, 96, 48, 112, 64));
    voxelCreated.push_back(new Voxel(3, 80, 32, 96, 48));
    voxelCreated.push_back(new Voxel(4, 96, 80, 112, 96));
    voxelCreated.push_back(new Voxel(5, 128, 16, 144, 32));

    for (int i(0);i<voxelCreated.size();i++)
    {
        voxelCreated[i]->setTextureCoordinates((voxelCreated[i]->getxA())/m_textureAtlas.getWidth(),
                                               (voxelCreated[i]->getyA())/m_textureAtlas.getHeight(),
                                               (voxelCreated[i]->getxB())/m_textureAtlas.getWidth(),
                                               (voxelCreated[i]->getyB())/m_textureAtlas.getHeight());

    }
}

voxelList::~voxelList()
{
    for (int i(0);i<voxelCreated.size();i++)
    {
        delete voxelCreated[i];
        voxelCreated[i] = 0;
    }
    //Erase the vector
}

voxelList* voxelList::Instance()
{
    if (!allVoxels)//Since we want one instance of allVoxels, it's created once and only once
        allVoxels = new voxelList;

    return allVoxels;
}

int voxelList::getMaxId() const
{
    return (voxelCreated.size()-1);
}

Texture voxelList::getTexture() const
{
    return m_textureAtlas;
}

Voxel* voxelList::getPointer(unsigned short Id) const
{
    for (unsigned int i(0);i<voxelCreated.size();i++)
    {
        if (voxelCreated[i]->blockId() == Id)
            return voxelCreated[i];
    }

    return 0;//Voxel not found :/
}

void voxelList::addVoxel(Voxel* toAdd)
{
    voxelCreated.push_back(toAdd);
}
