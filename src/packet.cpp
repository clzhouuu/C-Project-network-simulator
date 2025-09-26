#include "packet.hpp"


/**
 * @brief Construct a new Packet:: Packet object
 * 
 * @param dest 
 * @param src 
 * @param data 
 * @param id 
 * @param priority
 */
Packet::Packet(int dest, int src, int data, int id, int priority):
    dest_(dest), src_(src), data_(data), id_(id), priority_(priority), timer_(0), travelTime_(0) {}

/**
 * @brief Returns the data inside the packet.
 * 
 * @return int 
 */
int Packet::GetData() const {return data_;}

/**
 * @brief Return the destination of the packet.
 * 
 * @return int 
 */
int Packet::GetDestination() const {return dest_;}

/**
 * @brief Returns the source of the packet.
 * 
 * @return int 
 */
int Packet::GetSource() const {return src_;}

/**
 * @brief Return how many ticks the packet has traveled for.
 * 
 * @return int 
 */
int Packet::GetTime() const {return timer_;}

/**
 * @brief Return packets id.
 * 
 * @return int 
 */
int Packet::GetId() const {return id_;}

/**
 * @brief Returns the packet's size.
 * 
 * @return size_t
 */
size_t Packet::GetSize() const {return size_;}

/**
 * @brief Returns the packet's priority.
 * 
 * @return int 
 */
int Packet::GetPriority() const {return priority_;};

/**
 * @brief Resets the timer 
 */
void Packet::TimerReset() {timer_ = 0;};

/**
 * @brief Increments the timer by one
 */
void Packet::TimerIncrement() {timer_=timer_+1;};

/**
 * @brief Increments travel time by one
 * 
 */
void Packet::TTIncrement() {travelTime_++;};

/**
 * @brief Returns travel time
 * 
 * @return int 
 */
int Packet::GetTT() const {return travelTime_;}
