

#include <iostream>
#include <ctime>
#include <string>

#include "api.h"
#include "tree_simulator.h"
#include "node_analysis.h"
#include "depth_analysis.h"
#include "tree.h"
#include "def.h"

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

    TreeSimulator ts(15,3,1000, &create_node);
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





