#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include "node.h"
#include "utils.h"
#include "def.h"
#include "file_manager.h"

#define NOT_FOUND -1

namespace BBTree
{
  class TreeManager
  {
    private:
    std::string fileName;
    public:

    TreeManager(const std::string& file)
    {
      this->fileName = file;
    }
     
    private:
    typedef std::shared_ptr<Node> NodePtr;

    std::vector<NodePtr> GetNodes(int level, double leftBound, double rightBound)
    {     
      std::vector<int> nodesId = GetNodesId(level, leftBound, rightBound);
      std::vector<NodePtr> vec(nodesId.size());
      FileManager fm(fileName);
      fm.Open();
      for(auto it = nodesId.begin(); it!=nodesId.end(); ++it)
      {
        NodePtr node = NodeHelper::GetNode(fm.Read(Utils::Position(*it, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
        vec.push_back(node);
      }
      fm.Close();
      return vec;  
    }

    std::vector<int> GetNodesId(int level, double leftBound, double rightBound)
    {
       std::vector<int> vec;
       FileManager fm(Utils::GetLevelFileName(level));
       fm.Open();
       int64_t streamsize = fm.getFileSize();
       int64_t pos = BinarySearch(fm, streamsize, leftBound, rightBound);     
       if(pos!=NOT_FOUND)
       {
         while(pos < streamsize)
         {
           std::shared_ptr<Id> nodeId = NodeHelper::GetId(fm.Read(pos, DEPTH_RECORD_SIZE));
           if(nodeId->x <= rightBound)
             vec.push_back(nodeId->id);
           else
             break;
           pos+=DEPTH_RECORD_SIZE;
         }      
       }
       return vec;
    }

    int64_t BinarySearch(FileManager& fm, int64_t streamsize, double leftBound, double rightBound)
    {
      std::shared_ptr<Id> firstId = NodeHelper::GetId(fm.Read(0, DEPTH_RECORD_SIZE));
      std::shared_ptr<Id> lastId = NodeHelper::GetId(fm.Read((streamsize/DEPTH_RECORD_SIZE)-1, DEPTH_RECORD_SIZE));
      if(rightBound < firstId->x || leftBound > lastId->x)
        return NOT_FOUND;
      else if(leftBound <= firstId->x)
        return 0;
      else
      {
        int64_t low, hight, mid;
        low = 0;
        hight = streamsize - DEPTH_RECORD_SIZE;
        while(low <= hight)
        {
          mid = (low + hight)/2;
          std::shared_ptr<Id> id = NodeHelper::GetId(fm.Read(mid, DEPTH_RECORD_SIZE));
          if(leftBound < id->x)
            hight = mid - DEPTH_RECORD_SIZE;
          else if(leftBound > id->x)
            low = mid + DEPTH_RECORD_SIZE;
          else return mid;
        }
      }      
      return NOT_FOUND;
    }
  };
}

#endif /* TREE_MANAGER_H */
