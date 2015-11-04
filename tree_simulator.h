#ifndef TREE_SIMULATOR_H
#define TREE_SIMULATOR_H

#include <stdlib.h>
#include <time.h> 

namespace BBTree
{
  typedef int64_t (*T_CREATE_NODE)(int64_t pid, NodeState state, short branch, const std::string &data);

  class TreeSimulator
  {
    private:
    int height;
    int maxBranch;
    int maxNodes;
    int curNum = 0;
    T_CREATE_NODE createNode;
    public:  
    TreeSimulator(int treeHeight, int maxBranch, int maxNodes , T_CREATE_NODE createNode)
    {
      this->height = treeHeight;
      this->maxBranch = maxBranch;
      this->maxNodes = maxNodes;
      this->createNode = createNode;
    }

    void SimulateTree()
    {   
      srand(time(NULL));
      int64_t id = createNode(0, CandidateForBranching, 1, "Root node");
      curNum++;
      CreateChildNodes(id, 0);
    }

    private:
    void CreateChildNodes(int64_t pid, int depthNode)
    {
      curNum++;
      short branchNum=0;
      int err = 0;
      int64_t id;
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
      int range = rand() % treeHeight + 1;//from 1 to treeHeight
      return range > depthNode;
    }
  };
}

#endif /* TREE_SIMULATOR_H */
