#include "Configuration.hpp"
#include "Communication.hpp"
#include "WTD.hpp"

// Businness code for WTD packet processing
void WTDPacketProcessing(Communication::Packet packet)
{
    std::cout << "Inside WTD packet processing" << std::endl;
    std::cout << packet.data << std::endl;

    std::cout << WTD::SerializeDuck(WTD::ducks[0]) << std::endl;

    Communication::Socket::write(Communication::Packet{packet.description, WTD::SerializeDuck(WTD::ducks[0])});
}

// Program entry, start the server and catch exception from it
int main(int argc, char* argv[])
{
    try{
      
        // Load configuration file
        Configuration::JsonLoader jsonLoader(argv[1]);

        // Get all ducks from configurations file 
        WTD::LoadDucksFromJson(jsonLoader.getValue());

        // Start server 
        Communication::Server server(3333, std::thread::hardware_concurrency(), (Communication::processPacket) WTDPacketProcessing);
    
    } catch(Communication::SocketException& exception) {
        std::cout << exception.message << std::endl;
    } catch(Configuration::ConfigurationReadException& exception) {
        std::cout << exception.message << std::endl;
    }
    return 0;
}