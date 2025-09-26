#ifndef ENDHOST_H
#define ENDHOST_H

#include "node.hpp"


class EndHost : public Node
{
private:
    std::vector<Packet*> delivered;

public:
    EndHost(int id, int addr, int max, std::string behaviour);
    void GenerateData();  
    void ReceivePacket(Packet* packet) override;
    void ClearDelivered(); 
    std::vector<Packet*> GetDelivered();
};

#endif // ENDHOST_H