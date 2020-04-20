#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <fstream>
#include <iostream>
#include <string.h>

namespace Message 
{
    class Base 
    {
        protected: 
            std::string type = "base";

        public:
            std::string SerializeToString();
            std::string DebugString();
            void ParseFromString(std::string);
    };

    class Duck : Base
    {
        private:
            std::string sound;
            float x;
            float y;
            float z;
            float ax;
            float ay;
            float az;

        public:
            Duck();
            std::string SerializeToString();
            std::string DebugString();
            void ParseFromString(std::string);

            void set_sound(std::string);
            void set_x(float);
            void set_y(float);
            void set_z(float);
            void set_ax(float);
            void set_ay(float);
            void set_az(float);
    };
};
#endif