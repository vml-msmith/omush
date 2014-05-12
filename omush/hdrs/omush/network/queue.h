///
/// queue.h
///
/// Copyright 2014 Michael Smith
///

#ifndef OMUSH_HDRS_OMUSH_NETWORK_QUEUE_H_
#define OMUSH_HDRS_OMUSH_NETWORK_QUEUE_H_

#include <string>
#include <queue>

namespace omush {
  namespace network {

    class NetworkQueue {
     protected:
      std::queue<std::string> buffer;
    };


    class InputQueue : public NetworkQueue {
     public:
      void addMessage(std::string str) { this->buffer.push(str); }
      std::string popMessage() { 
        std::string buf = this->buffer.back();
        this->buffer.pop();
        return buf;
      }
      bool hasMessages() { return !this->buffer.empty(); }
    };

    class OutputQueue {
    };
  }  // namespace network
}  // namespace omush


#endif  // OMUSH_HDRS_OMUSH_NETWORK_QUEUE_H_
