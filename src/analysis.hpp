#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <vector>
#include "graph.hpp"

class Analysis 
{
public:
   int delivered(Graph network); // returns the total delivered packets
   int sent(Graph network); // returns the total number of sent packets
   int delivery_mean(Graph network); // returns mean of all packets' delivery times
   int delivery_variance(Graph network); // returns variance of all packets' delivery times

   int speed_mean(Graph network); // the average of all links' speeds
   std::vector<int> node_utilization_Sp(Graph network); // returns node specific utilization in percentages
   std::vector<int> droppedSp(Graph network); // node specific amount of packets dropped in percentages
   std::vector<int> sentSp(Graph network); // node specific amount of packets sent in percentages


};


#endif // ANALYSIS_H
