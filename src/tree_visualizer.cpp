
#include <iostream>
#include <ctime>
#include "tree_manager.h"
#include "def.h"
#include "json.hpp"

using namespace BBTree;


void WriteUsage();

int main(int argc , char *argv[])
{
  try
  {
    clock_t t = clock();
    if(argc == 1)
      WriteUsage();
    else if (std::string(argv[1]) == "--h")
      WriteUsage();
    else if (std::string(argv[1]) == "-n" && argc > 4)
    {
      int id = std::stoi(argv[2], NULL);
      double width = std::stod(argv[3], NULL);
      double hight = std::stod(argv[4], NULL);
      TreeManager tree(FILE_NAME_NEW);
      tree.WriteNodesAndEdges(id, width, hight); 
    }
    else if (std::string(argv[1]) == "-p" && argc > 5)
    {
      double x = std::stod(argv[2], NULL);
      double y = std::stod(argv[3], NULL);
      double width = std::stod(argv[4], NULL);
      double hight = std::stod(argv[5], NULL);
      TreeManager tree(FILE_NAME_NEW);
      tree.WriteNodesAndEdges(x, y, width, hight); 
    }
    else
      WriteUsage(); 

    t = clock() - t;
    //std::cout << "tree visualizer: " << ((float)t)/CLOCKS_PER_SEC << " c. \n";     
  }
  catch(std::exception& e)
  {
    json js;
    js["status"] = 1; //error
    js["error"] = std::string("exception caught: ") + e.what(); 
    std::cerr << js.dump(4) << '\n';
    return 1;
  }
  return 0;
}

void WriteUsage()
{
  std::cout << "usage:\n";
  std::cout << "treevis -n [node's id] [window's width] [window's hight]\n";
  std::cout << "treevis -p [x coordinate] [y coordinate] [window's width] [window's hight]\n";
}
