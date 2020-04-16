#include "protobuffer/build/base.pb.h"
#include "Communication.hpp" 

// This function is called by multiple thread 
void PacketProcessing(Communication::Packet packet)
{
    // print received packet 
    std::cout << "Inside packet processing" << std::endl;
    std::cout << packet.data << std::endl;

    // Send protobuffer message test
    Base::Connection connection;
    connection.set_nickname("Thomas");

    std::string responseData;
    connection.SerializeToString(&responseData);
    Communication::Packet response{packet.description, responseData};

    Communication::Socket::write(response);
}

int main()
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    Communication::Server server(3333, std::thread::hardware_concurrency(), (Communication::processPacket) PacketProcessing);

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}