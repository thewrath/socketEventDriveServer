#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

#include "Configuration.hpp"
#include "Communication.hpp"
#include "Message.hpp"

typedef struct
{
    int fd;
    int score;
    std::vector<int> foundDuckIds;
} WTDClient;

std::vector<WTDClient> WTDClients;
std::mutex WTDClientsMutex;
std::vector<Message::Duck> ducks;


// Never call this function alone (is not thread safe)
bool WTDCheckDuckAlreadyFound(WTDClient& client, int duckId)
{
    bool find = false;
    for(auto &id : client.foundDuckIds) {
        if(id == duckId) {
            find = true;
            break;
        }
    }
    return find;
}

void WTDUpdateClientScore(int fd, int duckId, int point)
{
    std::lock_guard<std::mutex> lock(WTDClientsMutex);
    for (auto &client : WTDClients)
    {
        if(client.fd == fd && !WTDCheckDuckAlreadyFound(client, duckId)) {
            client.score = client.score + point;
            std::cout << "Client score is now : " << std::to_string(client.score) << std::endl;
            client.foundDuckIds.push_back(duckId);
            break;
        } 
    }
}

void WTDAddClient(int fd)
{
    std::unique_lock<std::mutex> lock(WTDClientsMutex);
    bool find = false;
    for (auto &client : WTDClients)
    {
        if(client.fd == fd) {
            find = true;
        } 
    }

    if (!find) {
        std::cout << "Ajout du client " << std::to_string(fd) << std::endl;
        WTDClients.push_back(WTDClient{fd});
    }
}

int WTDGetClientScore(int fd)
{
    std::unique_lock<std::mutex> lock(WTDClientsMutex);
    for (auto &client : WTDClients)
    {
        if(client.fd == fd) {
            return client.score;
        } 
    }

    return 0;
}

// Businness code for WTD packet processing
void WTDPacketProcessing(Communication::Packet packet)
{
    // Check message type
    switch (Message::Base::GetType(packet.data))
    {
        case Message::MessageType::connection:
            // It's a connection message
            // Add client into client vector
            WTDAddClient(packet.description);
            // Send ducks to initialize
            for(auto& duck : ducks){
                Communication::Packet response{packet.description, duck.SerializeToString()};
                std::cout << duck.DebugString() << std::endl;
                try {
                    Communication::Socket::write(response);
                } catch(Communication::SocketException& e) {
                    std::cout << "Cannot send message to client" << std::endl;
                    break;
                }

                // Sleep for 300ms before resend another packet
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
            break;

        case Message::MessageType::found:
            {
                Message::Found message;
                message.ParseFromString(packet.data);
                std::cout << "Found duck message" << std::endl;
                
                std::cout << "Client ID" << std::to_string(packet.description) << std::endl;
                WTDUpdateClientScore(packet.description, message.id, 1);
                int client_score = WTDGetClientScore(packet.description);
                // TODO : find the maximum score with configuration file 
                if(client_score >= 3) {
                    Message::Win win(packet.description);
                    Communication::Packet response{packet.description, win.SerializeToString()};
                    std::unique_lock<std::mutex> lock(WTDClientsMutex);
                    for(auto &client : WTDClients) {
                        packet.description = client.fd;
                        Communication::Socket::write(response);
                    }
                }
            }
            break;
        
        case Message::MessageType::deconnection:
            // It's a deconnection message
            break;

        default:
            // Unknow message type
            std::cout << "package type " << std::to_string(Message::Base::GetType(packet.data)) << std::endl;
            break;
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
        duck.set_id(i);
        duck.set_x(root["ducks"][i]["position"]["x"].asFloat());
        duck.set_y(root["ducks"][i]["position"]["y"].asFloat());
        duck.set_z(root["ducks"][i]["position"]["z"].asFloat());
        duck.set_ax(root["ducks"][i]["direction"]["x"].asFloat());
        duck.set_ay(root["ducks"][i]["direction"]["y"].asFloat());
        duck.set_az(root["ducks"][i]["direction"]["z"].asFloat());
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
        Communication::Server server(3333, 6666, std::thread::hardware_concurrency(), (Communication::processPacket) WTDPacketProcessing);
    } else {
        std::cerr << "Cannot start server without configuration file" << std::endl;
        return 1;
    }
    return 0;
}