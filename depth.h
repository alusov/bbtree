#ifndef DEPTH_H
#define DEPTH_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <functional> 
#include "event.h"
#include "id_gen.h"
 

#define FILE_PATH_DEPTH "./data/depth.data"
#define FILE_DEPTH_FOLDER "./data/"
#define FILE_DEPTH "depth.data"

#define MAX_COUNT 10*1024*1024


namespace BBTree
{
  class Depth
  {
    private:
    std::string file;
    std::map<int, std::vector<Id>> depthId;
    std::map<int, NumberNodes> depthNumber;
    int count = 0;
    
    public:

    Depth(const std::string& file)
    {
      this->file = file; 
    }

    void DoAnalysis()
    {
      int64_t streamsize = getFileSize();
      std::fstream fs;
      OpenRead(fs, file);
      CreateRecord rec;
      int64_t currPos=0;
      do
      {
        currPos = ReadRecord(fs, currPos, rec);
        ++count;
        depthId[rec.depth].push_back(Id(rec.id, rec.fid==0));
        if(count >=MAX_COUNT)
          SaveDepthId();
      }
      while(currPos<streamsize);
      SaveDepthId();
      fs.close();
      SaveDepthAndNumber();
    }

    private:

    void OpenRead(std::fstream &fs,const std::string &fileName)
    {
      fs.exceptions(std::ifstream::failbit | std::ifstream::badbit );
      fs.open(fileName.c_str(), std::fstream::in | std::fstream::binary);
    }

    void OpenWrite(std::fstream &fs,const std::string &fileName)
    {
      fs.exceptions(std::ifstream::failbit | std::ifstream::badbit );
      fs << std::right;
      fs.open(fileName.c_str(), std::fstream::ate | std::fstream::out | std::fstream::binary);
    }

    int64_t getFileSize() 
    {
      std::ifstream ifs(file.c_str(), std::ifstream::ate | std::ifstream::binary);
      return ifs.tellg();   
    }

    int64_t ReadRecord(std::fstream &fs, size_t pos, CreateRecord &rec)
    {
      fs.seekg(pos, std::ios_base::beg);
      char buf[INFO_OFFSET];
      fs.read(buf, INFO_OFFSET);
      std::string record = buf;
      rec.id = std::stoi(record.substr(ID_NODE_OFFSET, ID_SIZE), NULL);
      rec.fid = std::stoi(record.substr(FID_NODE_OFFSET, ID_SIZE), NULL);
      rec.depth = std::stoi(record.substr(DEPTH_OFFSET, DEPTH_SIZE), NULL);
      rec.infoSize = std::stoi(record.substr(INFO_SIZE_OFFSET, INFO_SIZE), NULL); 
      fs.seekg(rec.infoSize + sizeof('\n'), std::ios_base::cur);
      return fs.tellg();
    }

    void SaveDepthId()
    {
      std::for_each(depthId.begin(), depthId.end(), std::bind(&Depth::CopyDepthNumber, this, std::placeholders::_1));
      for(auto itDepth=depthId.begin(); itDepth!=depthId.end(); ++itDepth)
      {
        std::stringstream fileNameAllNodes, fileNameListNodes;
        fileNameAllNodes << FILE_DEPTH_FOLDER << itDepth->first << FILE_DEPTH;        
        std::fstream fsAll, fsList;
        OpenWrite(fsAll, fileNameAllNodes.str());
        bool isFileOpenListNodes = false;
        for(auto itNumber=itDepth->second.begin(); itNumber!=itDepth->second.end(); ++itNumber)
        {
          fsAll << std::setw(ID_SIZE) << itNumber->id << '\n';
          if(itNumber->isList)
          {
            if(!isFileOpenListNodes)
            {
              fileNameListNodes << FILE_DEPTH_FOLDER << 'l' << itDepth->first << FILE_DEPTH;
              OpenWrite(fsList, fileNameListNodes.str());
              isFileOpenListNodes = true;
            }
            fsList << std::setw(ID_SIZE) << itNumber->id << '\n';
          }         
        }
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
      std::fstream fs;
      std::string fileName(FILE_PATH_DEPTH);
      OpenWrite(fs, fileName);  
      for(auto it=depthNumber.begin(); it!=depthNumber.end(); ++it)
      {
        fs << std::setw(DEPTH_SIZE) << it->first << std::setw(D_R_SIZE) << D_R;       
        fs << std::setw(NUM_NODES_SIZE) << it->second.AllNodes << std::setw(D_R_SIZE) << D_R;        
        fs << std::setw(NUM_NODES_SIZE) << it->second.ListNodes << '\n'; 
      }  
    }
  };
}

#endif /* ID_DEPTH_H */
