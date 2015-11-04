#ifndef TREE_ANALYSIS_H
#define TREE_ANALYSIS_H

#include <string>
#include <iostream>
#include <fstream> 
#include <iomanip>

#include "id_gen.h"
#include "event.h"

namespace BBTree
{

class TreeAnalysis
{
private:
  std::string file;
public:
  TreeAnalysis(const std::string& file)
  {
    this->file = file; 
  }

  int DoAnalysis()
  { 
    int64_t streamsize = getFileSize();
    std::fstream fs;
    Open(fs);
    CreateRecord rec;
    int64_t currPos = ReadRecord(fs, 0, rec);
    CheckRoot(rec);
    do
    {
      currPos = ReadRecord(fs, currPos, rec);
      UpdateParentRecord(fs, rec);
      UpdateChildRecord(fs, rec);
    }
    while(currPos<streamsize);
    fs.close();
    return 0;
  }

private:

  void Open(std::fstream& fs)
  {
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit );
    fs << std::right;
    fs.open(file.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
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
    rec.time = 0;
    rec.id = std::stoi(record.substr(ID_NODE_OFFSET, ID_SIZE), NULL);
    rec.pid = std::stoi(record.substr(PID_NODE_OFFSET, ID_SIZE), NULL);
    rec.branch = std::stoi(record.substr(BRANCH_OFFSET, ID_BRANCH), NULL);
    rec.lid = std::stoi(record.substr(LID_NODE_OFFSET, ID_SIZE), NULL);
    rec.rid = std::stoi(record.substr(RID_NODE_OFFSET, ID_SIZE), NULL);
    rec.fid = std::stoi(record.substr(FID_NODE_OFFSET, ID_SIZE), NULL);
    rec.nodeState = (NodeState)std::stoi(record.substr(STATE_OFFSET, STATE_SIZE), NULL);
    rec.depth = std::stoi(record.substr(DEPTH_OFFSET, DEPTH_SIZE), NULL);
    rec.infoSize = std::stoi(record.substr(INFO_SIZE_OFFSET, INFO_SIZE), NULL); 
    fs.seekg(rec.infoSize + sizeof('\n'), std::ios_base::cur);
    return fs.tellg();
  }

  void UpdateParentRecord(std::fstream &fs, CreateRecord &child)
  {
    CreateRecord record;
    ReadRecord(fs, child.pid, record);
    child.depth = record.depth + 1;
    if(record.fid==0)//first child node
    {
      fs.seekg(child.pid + FID_NODE_OFFSET, std::ios_base::beg);
      fs << std::setw(ID_SIZE) << child.id;
    }
    else
      AddNodeToList(fs, record.fid, child.id);
  }

  void AddNodeToList(std::fstream &fs, int64_t head, int64_t newnode)
  {
    CreateRecord record;
    ReadRecord(fs, head, record);
    while(record.rid!=0)
      ReadRecord(fs, record.rid, record);   
    fs.seekg(record.id + RID_NODE_OFFSET, std::ios_base::beg);
    fs << std::setw(ID_SIZE) << newnode;
    fs.seekg(newnode + LID_NODE_OFFSET, std::ios_base::beg);
    fs << std::setw(ID_SIZE) << record.id;   
  }

  void UpdateChildRecord(std::fstream &fs, const CreateRecord &child)
  {
    fs.seekg(child.id + DEPTH_OFFSET, std::ios_base::beg);
    fs << std::setw(DEPTH_SIZE) << child.depth;
  }

  void CheckRoot(const CreateRecord &rec)
  {   
    if(rec.id!=0 || rec.pid!=0)
    {
      std::cerr << "TreeAnalysis: root node is not valid\n";
      exit(EXIT_FAILURE);
    }   
  }

};

}

#endif /* TREE_ANALYSIS_H */
