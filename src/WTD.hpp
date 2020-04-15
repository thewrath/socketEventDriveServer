#ifndef WTD_HPP
#define WTD_HPP

#include "Configuration.hpp"
#include <string.h>
#include <vector>
#include "json/json.h"

namespace  WTD
{
    
    struct Duck
    {
        std::string sound;
        float x;
        float y;
        float z;
    };

    std::vector<WTD::Duck> ducks;

    inline void LoadDucksFromJson(Json::Value root)
    {
        if(!root.isMember("ducks")){
            throw Configuration::ConfigurationReadException("Configuration file doesnt have ducks field.");
        }
        
        for (Json::Value::ArrayIndex i = 0; i != root["ducks"].size(); i++){
            ducks.push_back(Duck{
                root["ducks"][i]["sound"].asString(),
                root["ducks"][i]["position"]["x"].asFloat(),
                root["ducks"][i]["position"]["y"].asFloat(),
                root["ducks"][i]["position"]["z"].asFloat()
            });
        }
       
    }

    inline std::string SerializeDuck(Duck duck)
    {
        return duck.sound;
    }
};

#endif