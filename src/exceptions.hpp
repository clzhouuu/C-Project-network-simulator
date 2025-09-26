#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

namespace Exceptions {

    // Packet not found exception
    class PacketNotFoundException : public std::runtime_error {
    public:
        PacketNotFoundException() : std::runtime_error("Packet not found in the queue") {}
    };

    // Queue full exception
    class QueueFullException : public std::runtime_error {
    public:
        QueueFullException() : std::runtime_error("Queue is full. Cannot add more packets.") {}
    };


    // Object with the same ID already exists
    class NodeExists : public std::runtime_error {
    public:
        NodeExists() : std::runtime_error("A node with the same ID already exists.") {}
    };

    // Node is not found
    class NodeNotFoundException : public std::runtime_error {
    public:
        NodeNotFoundException() : std::runtime_error("Node not found.") {}
    };



    // Destination node is not found
    class DestNodeNotFoundException : public std::runtime_error {
    public:
        DestNodeNotFoundException() : std::runtime_error("Destination node not found.") {}
    };

    // Path is not found
    class PathNotFoundException : public std::runtime_error {
    public:
        PathNotFoundException() : std::runtime_error("Path not found.") {}
    };

    
    // Link is either not ready to receive a packet or is not found
    class LinkFailedException : public std::runtime_error {
    public:
        LinkFailedException() : std::runtime_error("Link is either not ready to receive a packet or is not found.") {}
    };

    // There were no packets delivered
    class NoDeliveredException : public std::runtime_error {
    public:
        NoDeliveredException() : std::runtime_error("There were no packets delivered.") {}
    };

    // Link already exists
    class LinkExists : public std::runtime_error {
    public:
        LinkExists() : std::runtime_error("A link with the same ID already exists.") {}
    };

    // Link cannot be created between the same nodes
    class SameNode : public std::runtime_error {
    public:
        SameNode() : std::runtime_error("Link cannot be created between the same nodes.") {}
    };

    // Packet not found exception
    class PacketExists : public std::runtime_error {
    public:
        PacketExists() : std::runtime_error("A packet with the same ID already exists.") {}
    };


}

#endif // EXCEPTIONS_HPP