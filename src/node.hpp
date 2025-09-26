#ifndef NODE_H
#define NODE_H

#include "packet.hpp"
#include "exceptions.hpp"


#include <string>
#include <vector>


class Node
{
private:
    int id_; // Identifier for node
    int addr_; // Address of the node
    int max_load_; // Maximum number of packet in queue
    std::string queue_behaviour_; // Determines how the queue behaves
    std::vector<Packet*> queue; // Queue for packets waiting to be sent
    int load_ = queue.size(); // Current number of packets in queue
    int num_sent_; // Number of sent packages
    int num_dropped_; // Number of dropped packages
public:
    Node(int id, int addr, int max, std::string behaviour); // Constructor
    ~Node() {} // Destructor

    int GetID() const; // Get node identifier
    int GetAddress() const; // Get node address
    int GetLoad() const; // Get current load
    int GetMaxLoad() const; // Get max load
    int GetSent() const; // Get sent packages
    int GetDropped() const; // Get dropped packages
    std::vector<Packet*> GetQueue() const; // Get max queue
    std::string GetQueueBehaviour() const; // Get behaviour of the queue
    void IncrementSent(); // num_sent + 1
    void AddToQueue(Packet* packet); // Add a packet to the back of a queue
    void DeleteFromQueue(Packet* packet); // Delete a specific packet from the queue
    void UpdateLoad();
    void Drop(); // Increments the dropped packages;
    virtual void ReceivePacket(Packet* packet);
};

#endif //NODE_H
