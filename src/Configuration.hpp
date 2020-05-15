/**
 * \file Configuration.hpp
 * \brief Package de configuration
 * \author thewrath
 * \version 1.0
 * \date 20/04/2020
 *
 * Package utile à la lecture de fichier de configuration.
 *
 */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "json/json.h"

#include <fstream>
#include <iostream>
#include <string.h>

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