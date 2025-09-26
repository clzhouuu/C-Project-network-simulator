#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <climits>
#include <algorithm>
#include <stdlib.h> 





/**
 * @brief Adds a node to the graph
 * 
 * @param id 
 * @param addr 
 * @param maxLoad 
 * @param behaviour 
 */
void Graph::AddNode(int id, int addr, int max_load, std::string behaviour) {
    try {
        // Check if a node with the same ID already exists
        for (const auto& node : nodes) {
            if (node->GetID() == id) {
                throw Exceptions::NodeExists();
            }
        }

        // If no node with the same ID exists, add the new node
        nodes.push_back(new Node(id, addr, max_load, behaviour));
    } catch (const Exceptions::NodeExists& e) {
        std::cerr << "ERROR: Unable to add packet. " << id << e.what() << std::endl;
    }
}

/**
 * @brief Adds a link to the graph
 * 
 * @param srcNode 
 * @param destNode 
 * @param speed 
 * @param max_load 
 * @param delay 
 */
void Graph::AddLink(int srcID, int destID, int speed, int max_load, int delay) {
    try {  
        bool srcFound = false;
        bool destFound = false;

        bool linkExists = false;


        // Check if the source node exists
        for (const auto& node : nodes) {
            if (node->GetID() == srcID) {
                srcFound = true;
                break;
            }
        }

        // Check if the destination node exists
        for (const auto& node : nodes) {
            if (node->GetID() == destID) {
                destFound = true;
                break;
            }
        }


        // Check if the link exists
        for (const auto& link : links) {
            if ((link->GetFirst() == srcID && link->GetSecond() == destID)) {
                linkExists = true;
                break;
            }
        }

        if (linkExists) {
            throw Exceptions::LinkExists();
        } else if (!(srcFound == destFound)) {
            throw Exceptions::SameNode();
        }
        else if (srcFound && destFound && !linkExists) {
            // Both source and destination nodes exist, add the link
            links.push_back(new Link(srcID, destID, speed, max_load, delay));
        } else {
            throw Exceptions::NodeNotFoundException();
        }
    } catch (const Exceptions::NodeNotFoundException& e) {
        std::cerr << "ERROR: Unable to add link between " << srcID << " and " << destID << ". " << e.what() << std::endl;
    } catch (const Exceptions::LinkExists& e) {
        std::cerr << "ERROR: Unable to add link between " << srcID << " and " << destID << ". " << e.what() << std::endl;
    }
}

/**
 * @brief Adds a packet to the graph
 * 
 * @param dest 
 * @param src 
 * @param data 
 * @param id 
 * @param priority 
 */
void Graph::AddPacket(int dest, int src, int data, int id, int priority) {
    try {
        // Check if the packet already exists
        for (const auto& packet : packets) {
            if ((packet->GetId() == id)) {
                throw Exceptions::PacketExists();
            }
        }

        bool srcFound = false;
        int srcIndex = -1;

        // Check if the source node exists
        for (long unsigned int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->GetAddress() == src) {
                srcFound = true;
                srcIndex = static_cast<int>(i);
                break;
            }
        }

        if (!srcFound) {
            throw Exceptions::NodeNotFoundException();
        }

        // Check if the destination node exists
        bool destFound = false;
        for (long unsigned int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->GetAddress() == dest) {
                destFound = true;
                break;
            }
        }

        if (!destFound) {
            throw Exceptions::NodeNotFoundException();
        }

        // If both source and destination nodes exist, add the packet
        Packet* packet = new Packet(dest, src, data, id, priority);
        nodes[srcIndex]->AddToQueue(packet);
        nodes[srcIndex]->IncrementSent();
        packets.push_back(packet);

    } catch (const Exceptions::NodeNotFoundException& e) {
        std::cerr << "ERROR: Unable to add packet. " << e.what() << std::endl;
    } catch (const Exceptions::PacketExists& e) {
        std::cerr << "ERROR: Unable to add packet. " << e.what() << std::endl;
    }
}


/**
 * @brief Returns packets that have been sent so far.
 * 
 * @return int 
 */
int Graph::GetSent() const {return sent;}


/**
 * @brief Returns packets that have been delivered so far.
 * 
 * @return int 
 */
int Graph::GetDelivered() const {return delivered;}

/**
 * @brief Returns a vector of all nodes in the graph.
 * 
 * @return std::vector<Node> 
 */
std::vector<Node*>& Graph::GetNodes() {return nodes;}
/**
 * @brief Returns a vector of all links in the graph.
 * 
 * @return std::vector<Link> 
 */
std::vector<Link*>& Graph::GetLinks() {return links;}

/**
 * @brief Returns the current tick.
 * 
 * @return int 
 */
int Graph::GetTick() const {return tick;}

/**
 * @brief Returns the the total TT.
 * 
 * @return int 
 */
int Graph::GetTT() const {return TT;}

/**
 * @brief Function to increase one tick
 */
void Graph::TickIncrease(){
    try {
        // Tick goes up and so does every package's travel time
        tick++;

        // Increments every package's travel time
        for (auto& packet : packets) {
            packet->TTIncrement();
        }

        // Increments every link's and package's (that is in a link) timers
        for (int i = 0; i < GetLinkSize(); i++) {
            links[i]->tbtIncrement();
            for (long unsigned int i_1 = 0; i_1 < links[i]->GetInTransmission().size(); i_1++) {
                Packet* pack = links[i]->GetInTransmission()[i_1];
                pack->TimerIncrement();
            }
        }

        // Packets in links are being checked and delivered
        for (int i = 0; i < GetLinkSize(); i++) {
            std::vector<Packet*> inTransmission = links[i]->GetInTransmission();

            for (auto it = inTransmission.begin(); it != inTransmission.end();) {
                Packet* a = *it; // The packet being checked
                int trTime = a->GetSize() * links[i]->GetSpeed(); // The time that it takes for a packet to be delivered

                // First checks if the packet has been in the link for long enough.
                // If it's been there long enough, it resets the packet timer and sends it to a node.
                if (a->GetTime() >= trTime) {
                    int destID = links[i]->GetSecond(); // Other end of this specific link

                    a->TimerReset();

                    // If it's at its final destination, it'll be added into the endHost vector
                    if (FindNode(destID)->GetAddress() == a->GetDestination()) {
                        delivered++; // Increments the counter
                        TT = TT + a->GetTT();
                        FindNode(links[i]->GetSecond())->ReceivePacket(a); // Sends the packet to the endHost queue
                        links[i]->delQueue(a); // Deletes the packet from inTransmission queue
                        ++it;
                    }
                    // Adds the packet into the node's queue if it's not full
                    else if (!isQueueFull(destID)) {
                        FindNode(destID)->AddToQueue(a); // Adds the packet into the node's queue
                        links[i]->delQueue(a); // Deletes the packet from inTransmission queue
                        ++it;
                    }
                    // Error handling for if the queue is full and the packet disappears
                    else {
                        throw Exceptions::QueueFullException();
                        ++it;
                    }
                } else {
                    ++it;
                }
            }
        }

        // Sending packets from nodes to links
        for (int i = 0; i < GetNodeSize(); i++) {
            Node* node = nodes[i]; // The node we're handling
            std::string behaviour = node->GetQueueBehaviour(); // Node's behaviour
            std::vector<Packet*> queue = node->GetQueue(); // Node's queue

            // FIFO sends the first packet in the queue
            if (behaviour == "FIFO") {
                if (!queue.empty()) {
                    TransmitPacket(node, queue.front());
                }
            } 
            // FILO sends the last packet in the queue
            else if (behaviour == "FILO") {
                if (!queue.empty()) {
                    TransmitPacket(node, queue.back());
                }
            } 
            // Priority sends the packet of highest priority
            else if (behaviour == "priority") {
                Packet* packet = FindPriority(node); // Finds the packet of highest priority
                if (packet) {
                    TransmitPacket(node, packet);
                }
            }
        }

        // Generate random packets
        if(nodes.size() != 0){
            int num = rand() % nodes.size();
            int index = std::min(num, 3);
            for(int i = 0; i <= index; i++){
                int src = rand() % nodes.size();
                int dest = rand() % nodes.size();
                // Check if source and destination are end hosts. If they are add a packet.
                if( (dynamic_cast<EndHost*>(FindNode(src)) != nullptr) &&
                (dynamic_cast<EndHost*>(FindNode(dest)) != nullptr)){
                    int data = rand() % 1024;
                    int priority = 1;
                    AddPacket(dest + 1001, src + 1001, data, packets.size() + 1, priority);
                }
            }
        }
    } catch (const Exceptions::QueueFullException& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}

/**
 * @brief inserts x at bottom
 * 
 * @param st 
 * @param x 
 */
void insert_at_bottom(std::stack<Node*>& st, Node* x){
 
    if (st.size() == 0) {
        st.push(x);
    }
    else {
 
        Node* a = st.top();
        st.pop();
        insert_at_bottom(st, x);
        st.push(a);
    }
}


/**
 * @brief reverses a stack
 * 
 * @param st
 */
void reverse(std::stack<Node*>& st){
    if(st.size()>0){
            Node* x = st.top();
            st.pop();
            reverse(st);
            insert_at_bottom(st, x);
        } return;
}

/**
 * @brief Sends a packet from node to link
 * 
 * @param node 
 * @param packet 
 * 
 */
void Graph::TransmitPacket(Node* node, Packet* packet) {
     try {
        Node* destination = FindNodeAd(packet->GetDestination()); // Destination of the packet
        if (!destination) {
            throw Exceptions::DestNodeNotFoundException();
        }

        std::stack<Node*> path = GetPath(node->GetID(), destination->GetID()); // Packets shortest path to its destination
        // path.pop(); // starting node is not taken into account
        reverse(path);
        path.pop();

        if (path.empty()) {
            throw Exceptions::PathNotFoundException();
        }

        Link* link = FindLink(node, path.top()); // Finds the link that the packet needs to be sent to so that it can get to the destination the fastest

        if (!link){
             node->DeleteFromQueue(packet); // Deletes packet from the queue
            std::cout << "The link between " << node->GetID() << " and " << path.top()->GetID() << " is not found." << std::endl;
        } else if (!(link->GetTBT() >= link->GetDelay())){
             node->DeleteFromQueue(packet); // Deletes packet from the queue
            std::cout << "The link between " << link->GetFirst() << " and " << link->GetSecond() << " is not ready to receive another packet." << std::endl;
        }
        // If the link is found and the link is ready to receive another node, send the packet
        else if (link && (link->GetTBT() >= link->GetDelay())) { 
            link->addQueue(packet); // Adds the packet into link
            link->resetTBT(); // Resets time in between transmissions counter
            node->DeleteFromQueue(packet); // Deletes packet from the queue

            // Increment 'sent' if the node is the source
            if (packet->GetSource() == node->GetAddress()) {
                sent++;
            }
        } 
        
    } catch (const Exceptions::DestNodeNotFoundException& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    } catch (const Exceptions::PathNotFoundException& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    } catch (const Exceptions::LinkFailedException& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}


/**
 * @brief Finds the packet of highest priority within a node's queue
 * 
 * @param node  
 * 
 * @return Packet* 
 */
Packet* Graph::FindPriority(Node* a) {
    std::vector<Packet*> queue = a->GetQueue();
    int size = queue.size();
    int priority = 0;
    int index = -1;  // Initialize to an invalid index

    if (size > 0) {
        for (int i = 0; i < size; i++) {
            if (queue[i]->GetPriority() > priority) {
                index = i;
                priority = queue[i]->GetPriority();
            }
        }

        if (index != -1) {
            Packet* packetPtr = queue[index];
            return packetPtr;
        }
    }

    return nullptr;
}

/**
 * @brief Finds the link between two nodes
 * 
 * @param node_1  
 * @param node_2
 * 
 * @return Link*
 */
Link* Graph::FindLink(Node* node_1, Node* node_2) {
    for(size_t i = 0; i < links.size(); i++) {
        // if (((links[i].GetFirst() == node_1->GetID()) && (links[i].GetSecond() == node_2->GetID())) || 
        // ((links[i].GetFirst() == node_2->GetID()) && (links[i].GetSecond() == node_1->GetID()))) 
        if ((links[i]->GetFirst() == node_1->GetID()) && (links[i]->GetSecond() == node_2->GetID())){
            return links[i];
        }
    }
    return nullptr;
}

/**
 * @brief Checks if the queue of a node is full or not
 * 
 * @param ID 
 * 
 * @return bool
 */
bool Graph::isQueueFull(int ID) {
    if (FindNode(ID)->GetMaxLoad() != FindNode(ID)->GetLoad()) {
        return false;
    } else {return true;}
}

/**
 * @brief Reads a nodes and links from the given file and adds them to the graph.
 * 
 * @param filename 
 */
void Graph::ReadGraph(const std::string& filename) {
    
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ERROR: Unable to open file " << filename << std::endl;
        return;
    }

    // Read the header line
    std::string header;
    std::getline(file, header);

    // Check if the header line has the correct format
    if (header.empty() || header != "#Network simulator parameters") {
        std::cerr << "ERROR: Wrong type of file or missing header line\n"<< std::endl;
        file.close();
        return;
    }

    std::string line;
    int lineCount = 2;  // Start counting from the second line

    while (std::getline(file, line)) {


        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // int numNodes, numLinks;
        // iss >> numNodes >> numLinks;
        if (type == "Packet"){
            int dest, src, data, id, priority;

            // Check if all expected values are present
            if (!(iss >> dest >> src >> data >> id >> priority)) {
                std::cerr << "ERROR: Incorrect format on line " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }

            // Check if there are no extra values on the line
            if (iss.rdbuf()->in_avail() != 0) {
                std::cerr << "ERROR: Extra values on line " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }

            AddPacket(dest, src, data, id, priority);


        } else if (type == "Router") {
            int id, addr, maxLoad;
            std::string behaviour;

            // Check if all expected values are present
            if (!(iss >> id >> addr >> maxLoad >> behaviour)) {
                std::cerr << "ERROR: Incorrect format on line " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }
            
            if (!(behaviour == "FIFO" || behaviour == "FILO" || behaviour == "priority")) {
                std::cerr << "ERROR: Behavior is unknown " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }
            // Check if there are no extra values on the line
            if (iss.rdbuf()->in_avail() != 0) {
                std::cerr << "ERROR: Extra values on line " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }

            // Read and add router
            AddNode(id, addr, maxLoad, behaviour);

        } else if (type == "EndHost") {
            int id, addr, maxLoad;
            std::string behaviour;

            // Check if all expected values are present
            if (!(iss >> id >> addr >> maxLoad >> behaviour)) {
                std::cerr << "ERROR: Incorrect format on line " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }

            // Check if there are no extra values on the line
            if (iss.rdbuf()->in_avail() != 0) {
                std::cerr << "ERROR: Extra values on line " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }

            // Read and add endHost 
            nodes.push_back(new EndHost(id, addr, maxLoad, behaviour));

        } else if (type == "Link") {
            int srcID, destID, delay, speed, max_load;

            // Check if all expected values are present
            if (!(iss >> srcID >> destID >> delay >> speed >> max_load)) {
                std::cerr << "ERROR: Incorrect format on line " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }

            // Check if there are no extra values on the line
            if (iss.rdbuf()->in_avail() != 0) {
                std::cerr << "ERROR: Extra values on line " << lineCount << ": " << line << std::endl;
                skippedLines.push_back(line);
            }

            // Find source and destination nodes
            int srcIndex = -1, destIndex = -1;

            for (long unsigned int i = 0; i < nodes.size(); ++i) {
                if (nodes[i]->GetID() == srcID) {
                    srcIndex = i;
                    break;
                }
            }

            for (long unsigned int i = 0; i < nodes.size(); ++i) {
                if (nodes[i]->GetID() == destID) {
                    destIndex = i;
                    break;
                }
            }

            // Check if both source and destination nodes were found
            if (srcIndex != -1 && destIndex != -1) {
                AddLink(nodes[srcIndex]->GetID(), nodes[destIndex]->GetID(), speed, max_load, delay);
            } else {
                std::cerr << "ERROR: Source or destination node not found/initialized.\n" << std::endl;
                skippedLines.push_back(line);
                continue;
            }
        

        } else if (type == "") {
            skippedLines.push_back(line);
            lineCount++;
            continue;
        } else {
            std::cerr << "ERROR: Skipping line with unknown type on line " << lineCount << ": " << line << std::endl;
            skippedLines.push_back(line);
            continue;
        }
        lineCount++;
        
    }
    file.close();
}


/**
 * @brief Finds node based on its address
 * 
 * @param id 
 * @return Node* 
 */
Node* Graph::FindNodeAd(int addr){
    for(size_t i = 0; i < nodes.size(); i++){
        if(nodes[i]->GetAddress() == addr) return nodes[i];
    }
    return nullptr;
}

/**
 * @brief Finds node based on its id
 * 
 * @param id 
 * @return Node* 
 */
Node* Graph::FindNode(int id){
    for(size_t i = 0; i < nodes.size(); i++){
        if(nodes[i]->GetID() == id) return nodes[i];
    }
    return nullptr;
}


/**
 * @brief Finds the path based on parents vector and puts it into a stack
 * 
 * @param currentVertex 
 * @param parents 
 * @param S 
 */
std::stack<Node*> Graph::Path(int currentVertex, std::vector<int> parents, std::stack<Node*> S){
    // Base case : Source node has
    // been processed
    if (currentVertex == -1) {
        return S;
    }

    if (parents[currentVertex] != currentVertex){
        S = Path(parents[currentVertex], parents, S);
    }

    S.push(FindNode(currentVertex));
    return S;
}


/**
 * @brief Gets the weight of a link between two nodes. If no link exists returns 0.
 * 
 * @param first 
 * @param second 
 * @return int 
 */
int Graph::GetWeight(Node* first, Node* second) const{
    for (size_t i = 0; i < links.size(); i++){
        Link* link = links[i];
        if(link->GetFirst() == first->GetID() && link->GetSecond() == second->GetID()){
            return link->GetSpeed();
        } // else if (link.GetFirst() == second.GetID() && link.GetSecond() == first.GetID()){
          //   return link.GetSpeed();
        // }
    } return 0;
}

/**
 * @brief Picks the minimum distance vertex from the set of vertices.
 * 
 * @param dist 
 * @param sptSet 
 * @return int 
 */
int Graph::minDistance(std::vector<int> dist, std::vector<bool> sptSet) const{
    // Initialize min value
    int min = INT_MAX, min_index;
    for (long unsigned int v = 0; v < nodes.size(); v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

/**
 * @brief Returns the shortest path between two nodes.
 * 
 * @param src_id 
 * @param dest_id 
 * @return std::stack<Node *> 
 */
std::stack<Node *> Graph::GetPath(int src_id, int dest_id) {

    int n = nodes.size();
    std::vector<int> dist(n);
    std::vector<bool> sptSet(n);
    
    for (size_t i = 0; i < nodes.size(); i++){
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src_id] = 0;
    std::vector<int> parents(nodes.size());
    parents[src_id] = -1;

    for (long unsigned int count = 0; count < nodes.size() - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.
        int u = minDistance(dist, sptSet);
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (long unsigned int v = 0; v < nodes.size(); v++)
            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            // int edgeDistance = adjacencyMatrix[nearestVertex][vertexIndex];
            if (!sptSet[v] && GetWeight(nodes[u], nodes[v])
                && dist[u] != INT_MAX
                && dist[u] + GetWeight(nodes[u], nodes[v]) < dist[v]){
                    parents[v] = u;
                    dist[v] = dist[u] + GetWeight(nodes[u], nodes[v]);
                }
    } 
    std::stack<Node*> S;
    S = Path(dest_id, parents, S);
    return S;
}


/**
 * @brief Prints links
 * 
 */
void Graph::PrintLinks(){
    for (auto it : links) {
        std::cout << "Link between " << it->GetFirst() << " and " << it->GetSecond() << ":\n"
          << "Speed " << it->GetSpeed() << "\n"
          << "Current load " << it->GetLoad() << "\n"
          << "Delay " << it->GetDelay() << "\n" << std::endl;
    }
} 


/**
 * @brief Prints nodes
 * 
 */
void Graph::PrintNodes() {
    for (auto it : nodes) {
        std::cout << "Node " << it->GetID() << ":\n"
          << "Address " << it->GetAddress() << "\n"
          << "Maximum load " << it->GetMaxLoad() << "\n"
          << "Load " << it->GetLoad() << "\n"
          << "Behaviour " << it->GetQueueBehaviour() << "\n"
          << std::endl;
    }
}


/**
 * @brief Erase packet
 * 
 * @param packet 
 */
void Graph::ErasePacket(Packet* packet) {
    auto it = std::find_if(packets.begin(), packets.end(),
                           [packet](const Packet* p) {
                               return p->GetId() == packet->GetId();
                           });

    if (it != packets.end()) {
        packets.erase(it);
    }

}