#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <stdint.h>
#include <stdlib.h> 
#include <iomanip>
#include "def.h"
#include <memory>

namespace BBTree
{

  enum NodeState
  {
    NewBorn = 1,
    GiveBirth = 2,
    EndUp = 3
  };

  struct NodeBase
  {
    std::string time;
    int id;
    int idEnd;
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

   struct Point
   {
     Point()
     {
     }
     Point(double x_, double y_) : x(x_), y(y_)
     {
     }
     double x;
     double y;
   };

   struct Pair
   {
     Pair()
     {
     }
     Pair(int start, int end) : startId(start), endId(end)
     {
     }
     bool operator < (const Pair& pair) const
     {
       return startId < pair.startId ? true : startId==pair.startId ? endId < pair.endId : false;   
     }
     int startId;
     int endId;
     
   };
   
   struct Edge
   {
     Edge()
     {
     }
     Edge(int startId, int endId, double startX, double startY, double endX, double endY) : pair(startId, endId), start(startX, startY), end(endX,endY)
     {
     }
     Pair pair;
     Point start;
     Point end;
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
       nodePtr->time = record.substr(TIME_OFFSET, TIME_SIZE);
       nodePtr->id = std::stoi(record.substr(ID_NODE_OFFSET, ID_SIZE), NULL);
       nodePtr->idEnd = std::stoi(record.substr(ID_END_OFFSET, ID_SIZE), NULL);
       nodePtr->pid = std::stoi(record.substr(PID_NODE_OFFSET, ID_SIZE), NULL);
       nodePtr->branch = std::stoi(record.substr(BRANCH_OFFSET, ID_BRANCH_SIZE), NULL);
       nodePtr->nodeState = (NodeState)std::stoi(record.substr(STATE_OFFSET, STATE_SIZE), NULL);
       nodePtr->info = record.substr(INFO_OFFSET, INFO_SIZE);    
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

     static std::shared_ptr<Id> GetId(const std::string& record)
     {
       std::shared_ptr<Id> idPtr(new Id());
       idPtr->id = std::stoi(record.substr(0, ID_SIZE), NULL);
       idPtr->x = std::stod(record.substr(DEPTH_X_OFFSET, X_SIZE), NULL);
       return idPtr;
     }
     
     static int GetIdFromNodeEnd(const std::string& record)
     {
       return std::stoi(record.substr(ID_NODE_OFFSET_END, ID_SIZE), NULL);
     }  
   };
   
}

#endif /* EVENT_H */
