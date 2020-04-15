#include "WTD.hpp"

namespace WTD
{
    Duck::Duck(std::string sound, float x, float y, float z, float aX, float aY, float aZ)
    {
        this->sound = sound;
        this->x = x;
        this->y = y;
        this->z = z;
        this->aX = aX;
        this->aY = aY;
        this->aZ = aZ;
    }

    Duck::Duck(const Duck& d)
    {
        this->sound = d.sound;
        this->x = d.x;
        this->y = d.y;
        this->z = d.z;
        this->aX = d.aX;
        this->aY = d.aY;
        this->aZ = d.aZ;
    }

    std::string Duck::serialize()
    {
        return this->sound;
    }
}