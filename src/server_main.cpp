#include "Configuration.hpp"
#include "Communication.hpp"
#include "WTD.hpp"

std::vector<WTD::Duck> ducks;

// Businness code for WTD packet processing
void WTDPacketProcessing(Communication::Packet packet)
{
    std::cout << "Inside WTD packet processing" << std::endl;
    std::cout << packet.data << std::endl;

    Communication::Socket::write(Communication::Packet{packet.description, ducks[0].serialize()});
}

// Program entry, start the server and catch exception from it
int main(int argc, char* argv[])
{
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
    return 0;
}