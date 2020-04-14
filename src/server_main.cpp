#include "Communication.hpp" 

void WTDPacketProcessing(Communication::Packet packet)
{
    std::cout << "Inside WTD packet processing" << std::endl;
    std::cout << packet.data << std::endl;

    Communication::Socket::write(Communication::Packet{packet.description, "Response from server"});
}

int main()
{
    try{
        Communication::Server server(3333, std::thread::hardware_concurrency(), (Communication::processPacket) WTDPacketProcessing);
    } catch(Communication::SocketException& exception) {
        std::cout << exception.message << std::endl;
    }
    return 0;
}