#ifndef DEPTH_ANALYSIS_H
#define DEPTH_ANALYSIS_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <functional> 
#include "node.h"
#include "def.h"
#include "file_manager.h"

#define FILE_PATH_DEPTH "./../data/depth.data"
#define FILE_DEPTH_FOLDER "./../data/"
#define FILE_DEPTH "depth.data"

#define MAX_COUNT 10*1024*1024


namespace BBTree
{
  class DepthAnalysis
  {
    private:
    FileManager fm;
    std::map<int, std::vector<Id>> depthId;
    std::map<int, NumberNodes> depthNumber;
    int count = 0;
    
    public:

    DepthAnalysis(const std::string& file) : fm(file)
    { 
    }

    void DoAnalysis()
    {
      fm.Open();
      NodePtr node = NodeHelper::GetNode(fm.Read(0, RECORD_EXT_SIZE));
      TreeWalk(node, 1);
      fm.Close();
      SaveDepthId();    
      SaveDepthAndNumber();
    }

    private:
    typedef std::shared_ptr<Node> NodePtr;

     void TreeWalk(const NodePtr& node, int count)
     {
        depthId[node->y].push_back(Id(node->id, node->fid==0, node->x));
        if(count >=MAX_COUNT)
          SaveDepthId(); 
        int childId = node->fid;
        while(childId)
        {
          NodePtr child = NodeHelper::GetNode(fm.Read(Utils::Position(childId, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
          TreeWalk(child, count+1);
          childId = child->rid;
        }
     }
    void SaveDepthId()
    {
      std::for_each(depthId.begin(), depthId.end(), std::bind(&DepthAnalysis::CopyDepthNumber, this, std::placeholders::_1));
      for(auto itDepth=depthId.begin(); itDepth!=depthId.end(); ++itDepth)
      {
        std::stringstream fileNameAllNodes, fileNameListNodes;
        fileNameAllNodes << FILE_DEPTH_FOLDER << itDepth->first << FILE_DEPTH;        
        FileManager fsAll(fileNameAllNodes.str());
        FileManager fsList;
        fsAll.OpenW();       
        for(auto itNumber=itDepth->second.begin(); itNumber!=itDepth->second.end(); ++itNumber)
        {
          fsAll.Write(ID_SIZE, itNumber->id); fsAll.Write(D_R_SIZE, D_R);        
          fsAll.Write(X_SIZE, itNumber->x); fsAll.Write(1, '\n');
          if(itNumber->isList)
          {
            if(!fsList.IsOpen())
            {
              fileNameListNodes << FILE_DEPTH_FOLDER << 'l' << itDepth->first << FILE_DEPTH;
              fsList.SetFile(fileNameListNodes.str());
              fsList.OpenW();
            }
            fsList.Write(ID_SIZE, itNumber->id);  fsList.Write(1, '\n');
          }         
        }
        fsAll.Close();
        fsList.Close();
      }   
      depthId.clear();
      count=0;
    }

    void CopyDepthNumber(const std::pair<int,const std::vector<Id>> &pair)
    {
      depthNumber[pair.first].AllNodes+=pair.second.size();
      depthNumber[pair.first].ListNodes+=std::count_if(
      pair.second.begin(), pair.second.end(), [](const Id& id) { return id.isList; });
    }

    void SaveDepthAndNumber()
    {
      FileManager fm(FILE_PATH_DEPTH);
      fm.OpenW();
      for(auto it=depthNumber.begin(); it!=depthNumber.end(); ++it)
      {
        fm.Write(Y_SIZE, it->first); fm.Write(D_R_SIZE, D_R);
        fm.Write(NUM_NODES_SIZE, it->second.AllNodes); fm.Write(D_R_SIZE, D_R);
        fm.Write(NUM_NODES_SIZE, it->second.ListNodes); fm.Write(1, '\n');
      }
      fm.Close();        
    }
  };
}

#endif /* ID_DEPTH_ANALYSIS_H */
