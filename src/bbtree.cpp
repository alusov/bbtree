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

#include "node.h"
#include "storage.h"
#include "id_gen.h"
#include "utils.h"
#include "tree_simulator.h"
#include "node_analysis.h"
#include "depth_analysis.h"
#include "tree.h"
#include "tree_manager.h"

/// This is the path to file with solver's events
#define FILE_NAME "./../data/buf.data"
#define FILE_NAME_NEW "./../data/buf_new.data"

/// BBTree namespace is used for this project
namespace BBTree
{

/// Storage object to write events in buffer and finally to save it in file
  Storage storage(FILE_NAME);
  int IdGen::curr_offset = 1;

/**
\brief API function to create node.

This is callback function. Solver or tree simulator can use it to generate events when a node is created.

\param[in] parentId Parent node's id. Note, use 0 for root node.
\param[in] state Node's state. We can fill enum with necessary states.
\param[in] branch It's number of node in linked list. Start number from 1.
\param[in] info It is node's data. It is any information connected with node.
\return Node's id. Note, It's offset in file with solver's events.
*/
  int create_node(int parentId, NodeState state, short branch, const std::string &info)
  {
     NodeBase node;
     node.id = IdGen::GetId();
     node.pid = parentId;
     node.branch = branch;
     Utils::GetTime(node.time);
     node.nodeState = state;
     node.info = info;
     storage.Write(node);
     return node.id;
  }
/**
\brief API function to update node's state.

This is callback function. Solver or tree simulator can use it to generate events when a state of a node is changed.
Note, it is not implemented yet. Please, don't use it.

\param[in] id Node's id.
\param[in] state Node's state.
\param[in] info It is node's data. It is any information connected with node.
*/
  void update_node_state(int id, NodeState state, const std::string &info)
  {
      throw "Not implemented yet.";
  }

}

using namespace BBTree;

void Clear()
{
  system("rm -rf ./../data/readme ./../data/*.*");
}

/**
It's main function. It creates tree simulator object and simulate tree.
Node analysis module calculates node's left sibling, node's right sibling and 
first child. Tree layout module calculates X and Y coordinate. Notes Y coordinate
is tree's level now. Depth analysis module calculates number of nodes/list nodes for each level.
Also it prepares node's id/list nodes's id and X coordinate for each level.
*/
int main(int argv, char** arg)
{
  Clear();
  try
  {
    clock_t t = clock();

    TreeSimulator ts(4,5, 100, &create_node);

    //TreeSimulator ts(37,3, 1024*1024*10, &create_node); //about 1.6GB data
    ts.SimulateTree();
    storage.SaveBuf();

    t = clock() - t;
    std::cout << "tree simulator: " << ((float)t)/CLOCKS_PER_SEC << " c. \n";

    NodeAnalysis nodeAnalysis(FILE_NAME, FILE_NAME_NEW);
    nodeAnalysis.DoAnalysis();
    t = clock() - t;
    std::cout << "node analysis: " << ((float)t)/CLOCKS_PER_SEC << " c. \n";

    //Note treeLayout runs about 50 sec for 1000 000 nodes.
    Tree tree(FILE_NAME_NEW);
    tree.TreeLayout();
    t = clock() - t;
    std::cout << "tree layout: " << ((float)t)/CLOCKS_PER_SEC << " c. \n";

    DepthAnalysis depthAnalysis(FILE_NAME_NEW);
    depthAnalysis.DoAnalysis();
    t = clock() - t;
    std::cout << "depth analysis: " << ((float)t)/CLOCKS_PER_SEC << " c. \n";

  }
  catch (std::ifstream::failure e)
  {
    std::cerr << "Exception opening/reading/closing file " << e.what() << '\n';
  }
  return 0;
}



