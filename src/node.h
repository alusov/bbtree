#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <stdint.h>
#include <iomanip>
#include "def.h"
#include <memory>

namespace BBTree
{

  enum NodeState
  {
    Branched = 1,
    CandidateForProcessing,
    CandidateForBranching,
    Fathomed,
    Infeasible,
    Integer
  };

  struct NodeBase
  {
    std::string time;
    int id;
    int pid;
    short branch;  
    NodeState nodeState;
    std::string info;
   };

   struct Node : NodeBase
   {
     int lid;
     int rid;
     int fid;
     int y;
     double x;
     double prelim;
     double mod;
     int thread;
     int ancestor;
   };

   struct ShiftChange
   {
     ShiftChange(int node) : id(node){}
     int id;
     double shift = 0.0;
     double change = 0.0;
   };

   struct Id
   {
     Id(int id, bool isList, double x)
     {
       this->id = id;
       this->isList = isList;
       this->x=x;
     }
     Id()
     {
       this->id = 0;
       this->isList = false;
       this->x=0;
     }
     int id;
     bool isList;
     double x;
   };

   struct NumberNodes
   {
     NumberNodes(int allNodes, int listNodes)
     {
        this->AllNodes = allNodes;
        this->ListNodes = listNodes;
     }
     NumberNodes()
     {
        this->AllNodes = 0;
        this->ListNodes = 0;
     }
     int AllNodes;
     int ListNodes;
   };

   class NodeHelper
   {
   public:
     static std::shared_ptr<Node> GetNode(const std::string& record)
     {
       std::shared_ptr<Node> nodePtr(new Node());
       nodePtr->id = std::stoi(record.substr(ID_NODE_OFFSET, ID_SIZE), NULL);
       nodePtr->pid = std::stoi(record.substr(PID_NODE_OFFSET, ID_SIZE), NULL);
       nodePtr->branch = std::stoi(record.substr(BRANCH_OFFSET, ID_BRANCH_SIZE), NULL);
       nodePtr->nodeState = (NodeState)std::stoi(record.substr(STATE_OFFSET, STATE_SIZE), NULL);
       nodePtr->lid = std::stoi(record.substr(LID_NODE_OFFSET, ID_SIZE), NULL);
       nodePtr->rid = std::stoi(record.substr(RID_NODE_OFFSET, ID_SIZE), NULL);
       nodePtr->fid = std::stoi(record.substr(FID_NODE_OFFSET, ID_SIZE), NULL);
       nodePtr->x = std::stod(record.substr(X_OFFSET, X_SIZE), NULL);
       nodePtr->y = std::stoi(record.substr(Y_OFFSET, Y_SIZE), NULL);
       nodePtr->prelim = std::stod(record.substr(PRELIM_OFFSET, PRELIM_SIZE), NULL);
       nodePtr->mod = std::stod(record.substr(MODIF_OFFSET, MODIF_SIZE), NULL);
       nodePtr->thread = std::stoi(record.substr(THREAD_OFFSET, THREAD_SIZE), NULL);
       nodePtr->ancestor = std::stoi(record.substr(ANCESTOR_OFFSET, ANCESTOR_SIZE), NULL);
       return nodePtr;
     }    
   };
   
}

#endif /* EVENT_H */
