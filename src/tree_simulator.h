#ifndef TREE_SIMULATOR_H
#define TREE_SIMULATOR_H

#include <stdlib.h>
#include <time.h> 

namespace BBTree
{
///This is pointer to the callback function which creates node, stores node in storage and returns node's id.
  typedef int (*T_CREATE_NODE)(int pid, NodeState state, short branch, const std::string &data);

///Tree simulator class simulates tree. 
  class TreeSimulator
  {
    private:
    int height;
    int maxBranch;
    int maxNodes;
    int curNum = 0;
    T_CREATE_NODE createNode;
    public:
/**
\brief Constructor
\param[in] treeHeight Tree height
\param[in] maxBranch Max number of child nodes
\param[in] maxNodes Max number of nodes in the tree
\param[in] createNode Callback function which generates event when a node is created.
*/  
    TreeSimulator(int treeHeight, int maxBranch, int maxNodes , T_CREATE_NODE createNode)
    {
      this->height = treeHeight;
      this->maxBranch = maxBranch;
      this->maxNodes = maxNodes;
      this->createNode = createNode;
    }
///Main function to simulate tree
    void SimulateTree()
    {  
      srand(time(NULL));
      int id = createNode(0, CandidateForBranching, 1, "Root node");
      curNum++;
      CreateChildNodes(id, 0);
    }

    private:
    void CreateChildNodes(int pid, int depthNode)
    {
      curNum++;
      short branchNum=0;
      int err = 0;
      int id;
      for(int i = 0; i < maxBranch && !err; i++)
      {
        if(IsCreateBranch(height, depthNode) && curNum <= maxNodes) 
        {     
          id = createNode(pid, CandidateForBranching, ++branchNum, "Some node");
          CreateChildNodes(id, depthNode + 1);
        } 
      }  
    }

    bool IsCreateBranch(int treeHeight, int depthNode)
    {
      //from 1 to treeHeight
      int range = rand() % treeHeight + 1;
      return range > depthNode;
    }
  };

}

#endif /* TREE_SIMULATOR_H */
