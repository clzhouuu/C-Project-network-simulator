#include "end-host.hpp"

/**
 * @brief Construct a new End Host:: End Host object
 * 
 * @param id 
 * @param addr 
 * @param max 
 * @param behaviour 
 */
EndHost::EndHost(int id, int addr, int max, std::string behaviour) :
    Node(id, addr, max, behaviour) {}

void EndHost::GenerateData() {}

/**
 * @brief Returns delivered packets
 * 
 * @return std::vector<Packet*> 
 */
std::vector<Packet*> EndHost::GetDelivered() {
    return delivered;
}

/**
 * @brief Receives a packet
 * 
 * @param packet 
 */
void EndHost::ReceivePacket(Packet* packet) {
    delivered.push_back(packet);
}

/**
 * @brief Clears the delivered vector
 * 
 */
void EndHost::ClearDelivered() {
    delivered.clear();
}

