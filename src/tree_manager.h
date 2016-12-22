#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include "node.h"
#include "utils.h"
#include "def.h"
#include "file_manager.h"
#include "json.hpp"

#define NOT_FOUND -1

using json = nlohmann::json;

namespace BBTree
{
  class TreeManager
  {
    private:
    std::string fileName;
    std::string fileNodeEndName;
    public:

    TreeManager(const std::string& file)
    {
      this->fileName = file;
    }
    TreeManager(const std::string& file, const std::string& fileNodeEnd)
    {
      this->fileName = file;
      this->fileNodeEndName = fileNodeEnd;
    }

    void WriteNodesAndEdges(int id, double width, double hight)
    {
      FileManager fm(fileName);
      fm.OpenRW();
      int64_t pos = fm.getFileSize() - RECORD_EXT_SIZE;
      NodePtr nodeLast = NodeHelper::GetNode(fm.Read(pos, RECORD_EXT_SIZE));
      if(id < 1 || id > nodeLast->id)
        throw std::invalid_argument("Invalid node's ID");
      pos = Utils::Position(id, RECORD_EXT_SIZE);
      NodePtr node = NodeHelper::GetNode(fm.Read(pos, RECORD_EXT_SIZE));
      fm.Close();
      WriteNodesAndEdges(node->x, node->y, width, hight); 
    }
    
    void ShowNodeEnd(int id_end, double width, double hight)
    {
      FileManager fm(fileNodeEndName);
      fm.OpenRW();
      int id = NodeHelper::GetIdFromNodeEnd(fm.Read(Utils::Position(id_end, ID_NODE_END_RECORD_SIZE), ID_NODE_END_RECORD_SIZE));     
      fm.Close();
      WriteNodesAndEdges(id, width, hight);
    }

    void WriteNodesAndEdges(double x, double y, double width, double hight)
    {
      NodeMap nodes = GetAllNodes(x, y, width, hight);  
      EdgeMap edges = GetEdges(nodes, y - hight/2, y + hight/2);
      Write(nodes, edges); 
    }
     
    private:
    typedef std::shared_ptr<Node> NodePtr;
    typedef std::map<int, NodePtr> NodeMap;
    typedef std::vector<NodePtr> NodeVec;
    typedef std::vector<Edge> EdgeVec;
    typedef std::map<Pair, Edge> EdgeMap;

    NodeMap GetAllNodes(double x, double y, double width, double hight)
    {
      int curLevel = y - hight/2;
      curLevel = curLevel < 0 ? 0 : curLevel;
      int maxLevel = GetMaxLevel();
      int downLevel = y + hight/2;
      if(downLevel > maxLevel)
        downLevel = maxLevel;      
      NodeMap nodes;
      while(curLevel <= downLevel)//it should be run ||
      {
        NodeVec levelNodes = GetLevelNodes(curLevel, x - width/2, x + width/2);
        for(auto it = levelNodes.begin(); it!= levelNodes.end(); ++it)
          nodes[(*it)->id] = *it;
        curLevel++;
      }
      return nodes;
    }

    NodeVec GetLevelNodes(int level, double leftBound, double rightBound)
    { 
      std::vector<int> nodesId = GetLevelNodesId(level, leftBound, rightBound);
      NodeVec vec;
      FileManager fm(fileName);
      fm.OpenRW();
      for(auto it = nodesId.begin(); it!=nodesId.end(); ++it)
      {
        NodePtr node = NodeHelper::GetNode(fm.Read(Utils::Position(*it, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
        vec.push_back(node);
      }
      fm.Close();
      return vec;  
    }

    std::vector<int> GetLevelNodesId(int level, double leftBound, double rightBound)
    {
       std::vector<int> vec;
       FileManager fm(Utils::GetLevelFileName(level));
       fm.OpenRW();
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
      std::shared_ptr<Id> lastId = NodeHelper::GetId(fm.Read(streamsize - DEPTH_RECORD_SIZE, DEPTH_RECORD_SIZE));   
      if(rightBound < firstId->x || leftBound > lastId->x)
        return NOT_FOUND;
      else
      {
        int low, hight, mid;
        low = 0;
        hight = (streamsize - DEPTH_RECORD_SIZE)/DEPTH_RECORD_SIZE;
        while(hight - low > 0)
        {
          mid = (low + hight)/2;
          std::shared_ptr<Id> id = NodeHelper::GetId(fm.Read(mid*DEPTH_RECORD_SIZE, DEPTH_RECORD_SIZE));
          if(id->x < leftBound)
            low = mid + 1;
          else if(id->x > leftBound)
            hight = mid;
          else
          {
            hight = mid;
            break;
          }
        }
        return hight * DEPTH_RECORD_SIZE;
      }
    }      
    
    EdgeMap GetEdges(const NodeMap &nodes, int firstLevel, int downLevel)
    {
      EdgeMap edges;
      FileManager fm(fileName);
      fm.OpenRW();     
      for(auto it = nodes.begin(); it != nodes.end(); ++it)
      {

        NodePtr start = it->second;
        NodePtr end; 
        int pid = start->pid;
        if(pid!=0)
        {
          auto parent = nodes.find(pid);
          if(parent == nodes.end())
            end = NodeHelper::GetNode(fm.Read(Utils::Position(pid, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
          else
            end = parent->second;
          const Pair pair(end->id, start->id);
          auto temp = edges.find(pair);
          if(temp==edges.end())
            edges[pair] = Edge(end->id, start->id, end->x, end->y, start->x, start->y);
        }

        if(start->y == downLevel)
          continue;
   
        int fid = it->second->fid;
        int siblingId = fid;
        while(siblingId != 0)
        {
          auto sibling = nodes.find(siblingId);
          if(sibling==nodes.end())
            end = NodeHelper::GetNode(fm.Read(Utils::Position(siblingId, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
          else
            end = sibling->second;
          const Pair pair(start->id, end->id);
          auto temp = edges.find(pair);
          if(temp==edges.end())
            edges[pair] = Edge(start->id, end->id, start->x, start->y, end->x, end->y);
          siblingId = end->rid;
        }
      }
      fm.Close();
      return edges;
    }

    int GetMaxLevel()
    {
      FileManager fm(FILE_PATH_DEPTH);
      fm.OpenRW();
      int64_t pos = fm.getFileSize() - ALL_DEPTH_RECORD_SIZE;
      std::string level = fm.Read(pos, Y_SIZE);
      fm.Close();
      return std::stoi(level, NULL);
    }

    void Write(NodeMap &nodes, EdgeMap &edges)
    {
        json js;
        js["status"] = 0; //success       
        for(auto it = nodes.begin(); it != nodes.end(); ++it)
        {
          NodePtr node = it->second;
          json jnode;
          jnode["Time"] = node->time;
          jnode["ID"] = node->id;
          jnode["IDEnd"] = node->idEnd;
          jnode["ParenID"] = node->pid;
          jnode["ChildID"] = node->branch;
          jnode["State"] = node->nodeState;
          jnode["Info"] = node->info;
          jnode["FirstChildId"] = node->fid;
          jnode["LeftSibling"] = node->lid;
          jnode["RightSibling"] = node->rid;
          jnode["X"] = node->x;
          jnode["Y"] = node->y;
          js["Nodes"].push_back(jnode);
        }
        for(auto it = edges.begin(); it != edges.end(); ++it)
        {
          Edge edge = it->second;
          json jedge;
          jedge["StartId"] = edge.pair.startId;
          jedge["EndId"] = edge.pair.endId;
          jedge["BeginX"] = edge.start.x;
          jedge["BeginY"] = edge.start.y;
          jedge["EndX"] = edge.end.x;
          jedge["EndY"] = edge.end.y;
          js["Edges"].push_back(jedge);
        }
        std::cout << js.dump(4) << '\n';
    }      

  };
}

#endif /* TREE_MANAGER_H */
