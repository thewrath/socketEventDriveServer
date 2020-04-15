#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <fstream>
#include <iostream>
#include <string.h>

#include "json/json.h"

namespace Configuration 
{
    struct ConfigurationReadException : public std::exception
    {
        std::string message;
        ConfigurationReadException(std::string m) : message(m) {}
        ~ConfigurationReadException() throw () {}
        const char* what() const throw() { return message.c_str(); }
    };

    class JsonLoader
    {
        private:
            Json::Value root;
        public: 
            JsonLoader(std::string filename);
            Json::Value getValue();
    };
};

#endif