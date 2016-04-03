#ifndef VOXEL_HPP_INCLUDED
#define VOXEL_HPP_INCLUDED

#define VOXEL_MAX_SIZE 32
#define VOXEL_MIN_SIZE 16

#define AIR 0

#include "textures.h"
#include <iostream>
#include <vector>

using namespace std;

class Voxel{
public:
    //Constructors/destructors
    Voxel();
    Voxel(unsigned short Id);
    Voxel(unsigned short Id, float xA, float yA, float xB, float yB);
    ~Voxel();
    //Accessors
    unsigned short blockId() const;
    float getxA() const;
    float getyA() const;
    float getxB() const;
    float getyB() const;
    //Mutators
    void setTextureCoordinates(float xA, float yA, float xB, float yB);
private:
    unsigned short m_Id;//Just to test if the whole thing works, name isn't important here
    float m_xA, m_yA, m_xB, m_yB;
};

class Fluid : public Voxel{
//Mettre dans un autre fichier
};

/**
Fonctionnement du singleton : le singleton est une classe possédant une seule et unique instance.
En d'autres termes, cette classe n'a qu'une seule existence possible.
Ceci est particulièrement utile ici, car il n'y a qu'une seule liste de voxels possible
De plus, une classe de ce type est globale, donc son accès à travers le fichier est très aisé

Néanmoins, il y a quelques ennuis, le plus gros étant l'utilisation des méthodes du singleton :
Tout les données importantes sont dans le pointeur de singleton. Néanmoins, il doit être privé pour
empêcher la création d'autres singletons et dans un respect de la POO.

Le singleton s'utilise comme cela de fait:

voxelList::Instance()->uneMéthode(Type argument);

voxelList est le nom de la classe, Instance() est une méthode publique donnant un pointeur,
qu'on utilise pour accéder aux méthodes. Simple, rapide et global.

*/
class voxelList //Is a singleton, since there's only one list of voxels                                                                                                                 (Minecraft does that too right?)
{
private:
    voxelList();
    voxelList(voxelList&){}//Shall not be used, so private
    voxelList& operator= (const voxelList&){return *this;}//Same
    ~voxelList();

    static voxelList* allVoxels;//The unique instance ever created

public:
    static voxelList* Instance();
    Texture getTexture() const;
    void addVoxel(Voxel* toAdd);
    int getMaxId() const;
    Voxel* getPointer(unsigned short Id) const;
private:
    vector< Voxel*> voxelCreated;//The whole voxel list, initialized in the constructor
    Texture m_textureAtlas;
    /**This is a Voxel* list for the sake of memory management, but also
    because this allows any inherited voxel to be added to the list. Which
    is nice, trust me.*/
};

#endif // VOXEL_HPP_INCLUDED
