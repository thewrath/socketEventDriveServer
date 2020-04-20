#include "Configuration.hpp"
#include "Communication.hpp"
#include "Message.hpp"

std::vector<Message::Duck> ducks;

// Businness code for WTD packet processing
void WTDPacketProcessing(Communication::Packet packet)
{
    // print received packet 
    std::cout << "Inside packet processing" << std::endl;
    std::cout << packet.data << std::endl;

    // Send protobuffer message test
    for(auto& duck : ducks){
        Communication::Packet response{packet.description, duck.SerializeToString()};
        std::cout << duck.DebugString() << std::endl;
        try {
            Communication::Socket::write(response);
        } catch(Communication::SocketException& e) {
            std::cout << "Client disconnected during socket writing" << std::endl;
            break;
        }
    }
}
// function to load duck messages from json configuration
std::vector<Message::Duck> LoadDucksFromJson(Json::Value root)
{
    if(!root.isMember("ducks")){
        throw Configuration::ConfigurationReadException("Configuration file doesnt have ducks field.");
    }

    std::vector<Message::Duck> ducks;
    
    for (Json::Value::ArrayIndex i = 0; i != root["ducks"].size(); i++){
        Message::Duck duck;
        duck.set_sound(root["ducks"][i]["sound"].asString());
        duck.set_x(root["ducks"][i]["position"]["x"].asFloat());
        duck.set_y(root["ducks"][i]["position"]["y"].asFloat());
        duck.set_z(root["ducks"][i]["position"]["z"].asFloat());
        duck.set_ax(root["ducks"][i]["orientation"]["x"].asFloat());
        duck.set_ay(root["ducks"][i]["orientation"]["y"].asFloat());
        duck.set_az(root["ducks"][i]["orientation"]["z"].asFloat());
        ducks.push_back(duck);
    }

    return ducks;
}

// Program entry, start the server and catch exception from it
int main(int argc, char* argv[])
{
      
    // Load configuration file
    if(argv[1]) {
        Configuration::JsonLoader jsonLoader(argv[1]);
        // Get all ducks from configurations file 
        ducks = LoadDucksFromJson(jsonLoader.getValue());

        // Start server 
        Communication::Server server(3333, std::thread::hardware_concurrency(), (Communication::processPacket) WTDPacketProcessing);
    } else {
        std::cerr << "Cannot start server without configuration file" << std::endl;
        return 1;
    }
    return 0;
}