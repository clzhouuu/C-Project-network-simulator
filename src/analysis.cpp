#include "analysis.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

int Analysis::delivered(Graph network) {
    return network.GetDelivered();
}

int Analysis::sent(Graph network) {
    return network.GetSent();
}

int Analysis::delivery_mean(Graph network){
    try {
        int totalTransmissionTime = network.GetTT();
        int numDeliveredPackets = network.GetDelivered();

        // Avoid dividing by zero
        if (numDeliveredPackets > 0) {
            return totalTransmissionTime / numDeliveredPackets;
        } else {
            throw Exceptions::NoDeliveredException();
        }
    } catch (const Exceptions::NoDeliveredException& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 0;
    }
}



int Analysis::delivery_variance(Graph network) {
    int totalSquaredDifference = 0;
    int numDeliveredPackets = 0;

    try {
        // mean transmission time
        int meanTransmissionTime = delivery_mean(network);

        // iterate through all end hosts
        for (size_t i = 0; i < network.GetNodes().size(); ++i) {
            Node* node = network.GetNodes()[i];
            

                // check if the node is an instance of EndHost
        if (EndHost* endHost = dynamic_cast<EndHost*>(node)) {
            
                if (endHost) {
                    // get the delivered packets from the end host
                    const std::vector<Packet*>& deliveredPackets = endHost->GetDelivered();

                    // iterate through the delivered packets
                    for (const auto& packet : deliveredPackets) {
                        int squaredDifference = std::pow(packet->GetTime() - meanTransmissionTime, 2);
                        totalSquaredDifference += squaredDifference;
                        numDeliveredPackets++;
                    }
                }  
            }
        }

        // Avoid dividing by zero
        if (numDeliveredPackets > 1) {
            return totalSquaredDifference / (numDeliveredPackets - 1);
        } else {
            std::cerr << "ERROR: Not enough delivered packets to calculate variance.\n";
        }
    } catch (const Exceptions::NoDeliveredException& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    return 0;
}


int Analysis::speed_mean(Graph network) {
    int totalS = 0;
    int total = 0;

    for (auto it : network.GetLinks()) {
        totalS = total + it->GetSpeed();
        total++;
    }

    return total > 0 ? totalS/total : 0;
}

std::vector<int> Analysis::node_utilization_Sp(Graph network) {
    std::vector<int> vec;

    for (int i = 0; i < network.GetNodeSize(); i++) {
        int utilization = (network.GetNodes()[i]->GetLoad()/network.GetNodes()[i]->GetMaxLoad()) * 100;
        vec.push_back(utilization);
    }

    return vec;
}

std::vector<int> Analysis::droppedSp(Graph network) {
    std::vector<int> vec;

    for (int i = 0; i < network.GetNodeSize(); i++) {
        int dropped = network.GetNodes()[i]->GetDropped();
        vec.push_back(dropped);
    }

    return vec;
}

std::vector<int> Analysis::sentSp(Graph network) {
    std::vector<int> vec;

    for (int i = 0; i < network.GetNodeSize(); i++) {
        int sent = network.GetNodes()[i]->GetSent();
        vec.push_back(sent);
    }

    return vec;
}
