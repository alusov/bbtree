/**
\brief The main file

This file contains API to log events from a solver. Now it is just two function to create tree's node or update its state.
These events are processed to create b&b tree.
*/

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "event.h"
#include "storage.h"
#include "id_gen.h"
#include "tree_simulator.h"
#include "tree_analysis.h"
#include "depth.h"

/// This is the path to file with solver's events
#define FILE_NAME "./data/buf.data"

/// BBTree namespace is used for this project
namespace BBTree
{

/// Storage object to write events in buffer and finally to save it in file
  Storage storage(FILE_NAME);
  int64_t IdGen::curr_offset = 0;

/**
\brief API function to create node.

This is callback function. Solver or tree simulator can use it to generate events when a node is created.

\param[in] pid Parent node's id. Note, use 0 for root node.
\param[in] state Node's state. We can fill enum with necessary states.
\param[in] branch It's number of node in linked list. Start number from 1.
\param[in] info It is node's data. It is any information connected with node.
\return Node's id. Note, It's offset in file with solver's events.
*/
  int64_t create_node(int64_t pid, NodeState state, short branch, const std::string &info)
  {  
     Event event;   
     event.id = IdGen::GetId(info.size());
     event.pid = pid;
     event.branch = branch;
     event.time = std::time(NULL);
     event.eventType = EventType::CreateNode;
     event.nodeState = state;
     event.info = info;   
     storage.Write(event);
     return event.id; 
  }
/**
\brief API function to update node's state.

This is callback function. Solver or tree simulator can use it to generate events when a state of a node is changed.
Note, it is not implemented yet. Please, don't use it.

\param[in] id Node's id.
\param[in] state Node's state.
\param[in] info It is node's data. It is any information connected with node.
*/
  void update_node_state(int64_t id, NodeState state, const std::string &info)
  {
     Event event;
     event.id = IdGen::GetId(info.size());
     event.pid = 0;
     event.branch = 0;
     event.time = std::time(NULL);
     event.eventType = EventType::UpdateNodeState;
     event.nodeState = state;
     event.info = info;
     storage.Write(event);
  }

}

using namespace BBTree;

void Clear()
{
  system("rm -rf readme data/*.*");
}

/**
It's main function. Now creates tree simulator object and simulate tree. 
Then tree analysis is done. 

*/
int main(int argv, char** arg)
{
  Clear();
  try
  {
    clock_t t = clock();
   
    TreeSimulator ts(4,3, 16, &create_node); 
    //TreeSimulator ts(37,3, 1024*1024*10, &create_node); //about 1GB data
    ts.SimulateTree();     
    storage.SaveBuf(); 
    
    t = clock() - t;
    std::cout << "tree simulator: " << ((float)t)/CLOCKS_PER_SEC << " c. \n";

    TreeAnalysis treeanalysis(FILE_NAME);
    treeanalysis.DoAnalysis();
    t = clock() - t;
    std::cout << "tree analysis: " << ((float)t)/CLOCKS_PER_SEC << " c. \n"; 

    Depth depth(FILE_NAME);
    depth.DoAnalysis();
    t = clock() - t;
    std::cout << "depth analysis: " << ((float)t)/CLOCKS_PER_SEC << " c. \n"; 
  }
  catch (std::ifstream::failure e) 
  {
    std::cerr << "Exception opening/reading/closing file\n" ;
  }
  return 0;
}



