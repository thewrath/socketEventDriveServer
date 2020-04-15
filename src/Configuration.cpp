#include "Configuration.hpp"

namespace Configuration 
{
    JsonLoader::JsonLoader(std::string filename)
    {
        std::ifstream ifs;
        ifs.open(filename);
        
        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        JSONCPP_STRING errs;
        if (!parseFromStream(builder, ifs, &this->root, &errs)) {
            throw ConfigurationReadException("Error when parsing configuration file, is your file correctly structured ?");
        }

        std::cout << "Current configuration file :" << std::endl;
        std::cout << this->root << std::endl;
    }

    Json::Value JsonLoader::getValue()
    {
        return this->root;
    }
}