#include "node.hpp"
#include <algorithm>
#include <iostream>
#include <exception>

/**
 * @brief Construct a new Node:: Node object
 * 
 * @param id 
 * @param addr 
 * @param max 
 * @param behaviour 
 */
Node::Node(int id, int addr, int max, std::string behaviour) :
    id_(id), addr_(addr), max_load_(max), queue_behaviour_(behaviour), load_(0), num_sent_(0), num_dropped_(0) {}


/**
 * @brief Return nodes id.
 * 
 * @return int 
 */
int Node::GetID() const {return id_;}

/**
 * @brief Return nodes address.
 * 
 * @return int 
 */
int Node::GetAddress() const{return addr_;}

/**
 * @brief Return nodes load.
 * 
 * @return int 
 */
int Node::GetLoad() const {return load_;}

/**
 * @brief Return the maximum load the node can withstand before starting to drop packets.
 * 
 * @return int 
 */
int Node::GetMaxLoad() const {return max_load_;}

/**
 * @brief Return nodes behaviour.
 * 
 * @return std::string 
 */
std::string Node::GetQueueBehaviour() const {return queue_behaviour_;}

/**
 * @brief Return the queue of packets.
 * 
 * @return std::vector<Packet> 
 */
std::vector<Packet*> Node::GetQueue() const {return queue;}

/**
 * @brief Return the number of sent packets.
 * 
 * @return int
 */
int Node::GetSent() const {return num_sent_;}
/**
 * @brief Return the number of dropped packets.
 * 
 * @return int 
 */
int Node::GetDropped() const {return num_dropped_;}

/**
 * @brief Increments number of packets sent.
 * 
 */
void Node::IncrementSent() {num_sent_++;}

/**
 * @brief Deletes a specific packet from the queue
 * 
 * @param a
 */
void Node::DeleteFromQueue(Packet* packet) {
    try {
        // std::remove_if moves packets that have the same ID as the parameter packet to the end of the vector
        // std::remove_if returns an iterator to the first element that was not moved and erase erases the end of the queue
        auto newEnd = std::remove_if(
            queue.begin(), 
            queue.end(),
            // Lambda function to check if the ID of a packet matches the given packet's ID
            [packet](const Packet* p) { return p->GetId() == packet->GetId(); }
        );

        // Check if any element was removed
        if (newEnd == queue.end()) {
            throw Exceptions::PacketNotFoundException();
        }

        // Erase the removed elements
        queue.erase(newEnd, queue.end());

        // Update the load
        UpdateLoad();
    } catch (const Exceptions::PacketNotFoundException& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}



/**
 * @brief Updates the load
 */
void Node::UpdateLoad() {
    load_ = queue.size();
}

/**
 * @brief Adds a packet into the queue
 * 
 * @param packet
 */
void Node::AddToQueue(Packet* packet) {
    try {
        // checks if the queue is full and adds it
        if (load_ < max_load_) {
            queue.emplace_back(packet);

            // update the load
            UpdateLoad();
        } else {
            throw Exceptions::QueueFullException();
        }
    } catch (const Exceptions::QueueFullException& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}


void Node::ReceivePacket(Packet* packet) {
    queue.push_back(packet);
}
