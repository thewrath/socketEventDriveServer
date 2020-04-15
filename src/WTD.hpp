#ifndef WTD_HPP
#define WTD_HPP

#include "Configuration.hpp"
#include <string.h>
#include <vector>
#include "json/json.h"

namespace  WTD
{
    /**
     * Est utilisé pour représenté un canard (avec les informations nécessaire)
     */
    class Duck
    {
        public:
            std::string sound;
            float x;
            float y;
            float z;

            float aX;
            float aY;
            float aZ;

            Duck(std::string, float, float, float, float, float, float);
            Duck(const Duck&);
            std::string serialize();
    };

    inline std::vector<Duck> LoadDucksFromJson(Json::Value root)
    {
        if(!root.isMember("ducks")){
            throw Configuration::ConfigurationReadException("Configuration file doesnt have ducks field.");
        }

        std::vector<Duck> ducks;
        
        for (Json::Value::ArrayIndex i = 0; i != root["ducks"].size(); i++){
            Duck duck(
                root["ducks"][i]["sound"].asString(),
                root["ducks"][i]["position"]["x"].asFloat(),
                root["ducks"][i]["position"]["y"].asFloat(),
                root["ducks"][i]["position"]["z"].asFloat(),
                root["ducks"][i]["orientation"]["x"].asFloat(),
                root["ducks"][i]["orientation"]["y"].asFloat(),
                root["ducks"][i]["orientation"]["z"].asFloat()
                
            );
            ducks.push_back(duck);
        }

        return ducks;
    }
    
};

#endif