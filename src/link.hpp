#ifndef LINK_H
#define LINK_H

#include <vector>
#include "node.hpp"

class Link 
{
private:
    int srcID_; // Source node
    int destID_; // Destination node
    int speed_; // The speed at which the packets are sent
    int max_load_; // Max load
    int delay_; // The time in between transmissions
    int load_; // Current load
    int tbt; // Current time between transmissions
    std::vector<Packet*> inTransmission; // Packets in transmission

public:
    Link(int srcID, int destID, int speed, int max_load, int delay); // Constructor
    ~Link() {} // Destructor

    int GetLoad() const; // Get load
    int GetSpeed() const; // Get speed
    int GetFirst() const; // Get source node
    int GetSecond() const; // Get destination node
    int GetDelay() const;  // Get delay
    int GetTBT() const;  // Get the time between transmission
    std::vector<Packet*> GetInTransmission(); // Get the packets in transmission

    void tbtIncrement(); // Increment the tbt timer
    void delQueue(Packet* packet); // Delete a specific packet from the queue
    void addQueue(Packet* a); // Add a packet into the queue
    void resetTBT(); // Resets the tbt timer
    void UpdateLinkLoad(); // Updates the load
};




#endif // LINK_H