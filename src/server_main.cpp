#include "Configuration.hpp"
#include "Communication.hpp"
#include "protobuffer/build/base.pb.h"

std::vector<Base::Duck> ducks;

// Businness code for WTD packet processing
void WTDPacketProcessing(Communication::Packet packet)
{
    // print received packet 
    std::cout << "Inside packet processing" << std::endl;
    std::cout << packet.data << std::endl;

    // Send protobuffer message test
    Base::Welcome welcome;
    welcome.set_uuid("uuid here");

    Communication::Packet response{packet.description, welcome.SerializeAsString()};

    Communication::Socket::write(response);
}
// function to load duck messages from json configuration
std::vector<Base::Duck> LoadDucksFromJson(Json::Value root)
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

// Program entry, start the server and catch exception from it
int main(int argc, char* argv[])
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    try{
      
        // Load configuration file
        Configuration::JsonLoader jsonLoader(argv[1]);

        // Get all ducks from configurations file 
        ducks = WTD::LoadDucksFromJson(jsonLoader.getValue());

        // Start server 
        Communication::Server server(3333, std::thread::hardware_concurrency(), (Communication::processPacket) WTDPacketProcessing);
    } catch(Communication::SocketException& exception) {
        std::cout << exception.message << std::endl;
    } catch(Configuration::ConfigurationReadException& exception) {
        std::cout << exception.message << std::endl;
    }
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}