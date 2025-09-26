#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include "node.hpp"
#include "link.hpp"
#include "end-host.hpp"
#include "exceptions.hpp"
#include <stack>

class Graph
{
private:
    std::vector<Node*> nodes; // Store nodes in a vector
    std::vector<Link*> links; 
    std::vector<Packet*> packets; 
    int sent = 0; // Counter for sent packets
    int delivered = 0; // Counter for delivered packets
    int tick = 0; //Keep count of ticks
    int TT = 0; //All packets' time of travel total
    std::vector<std::string> skippedLines;  // Keep track of lines with incorrect format

public:
    Graph() {} // constructor
    ~Graph() {} // destructor
    void AddNode(int id, int addr, int maxLoad, std::string behaviour);
    void AddLink(int srcNode, int destNode, int speed, int max_load, int delay);
    void AddPacket(int dest, int src, int data, int id, int priority);

    void ReadGraph(const std::string& filename);
    void TickIncrease();
    void TransmitPacket(Node* node, Packet* packet);
    std::stack<Node*> Path(int currentVertex, std::vector<int> parents, std::stack<Node*> S);
    void ErasePacket(Packet* packet);

    int GetTick() const;
    int GetTT() const;
    int GetSent() const;
    int GetDelivered() const;
    std::vector<Node*>& GetNodes();
    std::vector<Link*>& GetLinks();
    int GetNodeSize() {return nodes.size();}
    int GetLinkSize() {return links.size();}
    int GetPacketSize() {return packets.size();}
    int GetWeight(Node* first, Node* second) const;
    std::stack<Node *> GetPath(int src_id, int dest_id);
    void PrintLinks();
    void PrintNodes();
    
    bool isQueueFull(int destID);
    Node* FindNode(int id);
    Node* FindNodeAd(int address);
    Link* FindLink(Node* a, Node* b);
    Packet* FindPriority(Node* a);
    
    int minDistance(std::vector<int> dist, std::vector<bool> sptSet) const;
    const std::vector<std::string>& GetSkippedLines() const {return skippedLines;}
    
};

#endif // GRAPH_H