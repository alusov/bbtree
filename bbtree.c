#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <ctime>
#include <string>
#include "event.h"
#include "storage.h"
#include "id_gen.h"
#include "tree_simulator.h"
#include "tree_analysis.h"



#define FILE_NAME "buf.data"

namespace BBTree
{

  Storage storage(FILE_NAME);
  int64_t IdGen::curr_offset = 0;

  //callback function
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
  //callback function
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

int main(int argv, char** arg)
{

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
    
  }
  catch (std::ifstream::failure e) 
  {
    std::cerr << "Exception opening/reading/closing file\n" ;
  }
  return 0;
}


