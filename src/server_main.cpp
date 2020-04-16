#include "Communication.hpp" 

// This function is called by multiple thread 
void PacketProcessing(Communication::Packet packet)
{
    std::cout << "Inside packet processing" << std::endl;
    std::cout << packet.data << std::endl;
}

int main()
{
    Communication::Server server(3333, std::thread::hardware_concurrency(), (Communication::processPacket) PacketProcessing);
    return 0;
}