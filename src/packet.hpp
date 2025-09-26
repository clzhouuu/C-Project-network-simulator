#ifndef PACKET_H
#define PACKET_H
#include <string>


// Class for packets
class Packet
{
private:
    int dest_; // Destination address
    int src_; // Source address
    int data_; // Contained data
    int id_; // Identifier for packet
    int priority_; // Priority of the packet
    int timer_; // Timer to keep count of how long a packet has been in a link
    size_t size_ = 1; // Constant size of 1 bytes
    int travelTime_;
public:
    Packet(int dest, int src, int data, int id, int priority); //constructor
    ~Packet() {} //destructor
    
    int GetData() const; //get data
    int GetDestination() const; //get destination
    int GetSource() const; //get source
    int GetTime() const; //get timer status
    int GetTT() const; //get travel timer status
    int GetId() const; //get ID
    int GetPriority() const; //get priority
    size_t GetSize() const; //get size

    void TimerIncrement(); //increments the timer
    void TimerReset(); //resets the timer
    void TTIncrement(); //increments the travel timer

};

#endif // PACKET_H
