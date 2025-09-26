#include "graph.hpp"
#include "analysis.hpp"
#include <iostream>
#include <filesystem>


int main() {
    std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;
    // Create a graph
    Graph network;

    Analysis nA;

    // Add nodes
    // network.AddNode(0, 1001, 10, "FIFO");
    // network.AddNode(1, 1002, 5, "FILO");
    // /root/cpp-project/tests/
    network.ReadGraph("/home/knerv/cpp-project/tests/files.txt");
    network.PrintNodes();

    // Add links
    // network.AddLink(0, 1, 10, 20, 5);
    network.PrintLinks();

    // Add packets



    // Simulate the network
    for (int i = 0; i < 500; ++i) {
        network.TickIncrease();
        
        if(nA.node_utilization_Sp(network)[0] == 100){
            std::cout << i << std::endl;
        }

        // Print the state of nodes and links 
        std::cout << "\nTick " << i + 1 << ":" << std::endl;
        network.PrintNodes();
        network.PrintLinks();
        std::cout << network.GetSent() << std::endl;
        std::cout << network.GetDelivered() << std::endl;
        std::cout << nA.delivered(network) << std::endl;
        std::cout << nA.delivery_mean(network) << std::endl;
        std::cout << nA.delivery_variance(network) << std::endl;
        std::cout << nA.sent(network) << std::endl;
        std::cout << nA.speed_mean(network) << std::endl;
        std::cout << "------------------------------" << std::endl;
    }


    return 0;
}