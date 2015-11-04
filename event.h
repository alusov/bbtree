#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <stdint.h>
#include <ctime>

namespace BBTree
{
  enum EventType
  {
    CreateNode = 1,
    UpdateNodeState
  };

  enum NodeState
  {
    Branched = 1,
    CandidateForProcessing,
    CandidateForBranching,
    Fathomed,
    Infeasible,
    Integer
  };

  struct Event
  {
    int64_t id;
    int64_t pid;
    short branch;
    std::time_t time;
    EventType eventType;
    NodeState nodeState;
    int infoSize;
    std::string info;
   };

   struct CreateRecord : Event
   {
     int64_t lid;
     int64_t rid;
     int64_t fid;
     int depth;
   };
}

#endif /* EVENT_H */
