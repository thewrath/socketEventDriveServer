#include "Message.hpp"

namespace Message 
{
    std::string Base::SerializeToString()
    {
        return this->type+";";
    }

    std::string Base::DebugString()
    {
        return this->SerializeToString();
    }

    void Base::ParseFromString(std::string data)
    {
        
    }

    Duck::Duck()
    {
        this->type = "duck";
    }

    std::string Duck::SerializeToString()
    {
        return Base::SerializeToString();
    }

    std::string Duck::DebugString()
    {
        return Base::DebugString();
    }

    void Duck::ParseFromString(std::string data)
    {
        Base::ParseFromString(data);
    }

    void Duck::set_sound(std::string sound){this->sound = sound;}
    void Duck::set_x(float x){this->x = x;}
    void Duck::set_y(float y){this->y = y;}
    void Duck::set_z(float z){this->z = z;}
    void Duck::set_ax(float ax){this->ax = ax;}
    void Duck::set_ay(float ay){this->ay = ay;}
    void Duck::set_az(float az){this->az = az;}
}