#include "WTD.hpp"

namespace WTD
{
    Duck::Duck(std::string sound, float x, float y, float z)
    {
        this->sound = sound;
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Duck::Duck(const Duck& d)
    {
        this->sound = d.sound;
        this->x = d.x;
        this->y = d.y;
        this->z = d.z;
    }

    std::string Duck::serialize()
    {
        return this->sound;
    }
}