#include "Communication.hpp" 

void WTDPacketProcessing(Communication::Packet packet)
{
    std::cout << "Inside WTD packet processing" << std::endl;
    std::cout << packet.data << std::endl;
}

int main()
{
    Communication::Server server(3333, std::thread::hardware_concurrency(), (Communication::processPacket) WTDPacketProcessing);
    return 0;
}