#include "link.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <iostream>
#include <exception>


/**
 * @brief Construct a new Link:: Link object
 * 
 * @param n1 
 * @param n2 
 * @param speed 
 * @param max_load 
 * @param delay 
 */
Link::Link(int srcID, int destID, int speed, int max_load, int delay) :
    srcID_(srcID), destID_(destID), speed_(speed), max_load_(max_load), delay_(delay), load_(0), tbt(delay-1) {}


/**
 * @brief Return links load.
 * 
 * @return int 
 */
int Link::GetLoad() const {return load_;}

/**
 * @brief Return links speed.
 * 
 * @return int 
 */
int Link::GetSpeed() const {return speed_;}

/**
 * @brief Return the time between packet transmissions.
 * 
 * @return int 
 */
int Link::GetTBT() const {return tbt;}

/**
 * @brief Return first node of the link.
 * 
 * @return Node 
 */
int Link::GetFirst() const {return srcID_;}

/**
 * @brief Return second node of the link.
 * 
 * @return Node 
 */
int Link::GetSecond() const {return destID_;}

/**
 * @brief Return links delay.
 * 
 * @return int 
 */
int Link::GetDelay() const {return delay_;}

/**
 * @brief Increments the time between transmissions.
 */
void Link::tbtIncrement() {
        tbt++;
}

/**
 * @brief Return the packets in transmission queue.
 * 
 * @return std::vector<Packet>
 */
std::vector<Packet*> Link::GetInTransmission() {
        return inTransmission;
}


/**
 * @brief Deletes a packet from the packets in transmission queue
 */
void Link::delQueue(Packet* packet) {
    try {
        // std::remove_if moves packets that have the same ID as the parameter packet to the end of the vector
        // std::remove_if returns an iterator to the first element that was not moved and erase erases the end of the queue
        auto newEnd = std::remove_if(
            inTransmission.begin(), 
            inTransmission.end(),
            // Lambda function to check if the ID of a packet matches the given packet's ID
            [packet](const Packet* p) { return p->GetId() == packet->GetId(); }
        );

        //If the vector stays the same, throw an exception
        if (newEnd == inTransmission.end()) {
            throw Exceptions::PacketNotFoundException();
        }
        
        //Erase the found packet
        inTransmission.pop_back();
        UpdateLinkLoad();
    } catch (const Exceptions::PacketNotFoundException& e) {
        // Packet not found
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

}

/**
 * @brief Adds a packet into the packets in transmission queue
 */
void Link::addQueue(Packet* a) {
    try {
        if (static_cast<int>(inTransmission.size()) >= max_load_) {
            throw Exceptions::QueueFullException();
        }

        inTransmission.push_back(a);
        UpdateLinkLoad();
    } catch (const Exceptions::QueueFullException& e) {
        // The queue was full
        std::cerr << "ERROR: " << e.what() << std::endl;
    } 
}

/**
 * @brief Resets the time in between transmissions timer
 */
void Link::resetTBT() {
    tbt = 0;
}

/**
 * @brief Updates the load
 */
void Link::UpdateLinkLoad() {
    load_ = inTransmission.size();
}