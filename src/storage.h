#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <string>
#include <cstdio>
#include <fstream> 
#include <iomanip>
#include "node.h"
#include "ring_buffer.h"
#include "def.h"
#include "file_manager.h"

#define BUFFER_SIZE 2097152 //2^21


namespace BBTree
{
  class Storage
  {
    private:
    FileManager fm;
    RingBuffer<BUFFER_SIZE, NodeBase> buf;
    public:

    Storage(const std::string& file) : fm(file)
    {
    }

    void Write(const NodeBase& node)
    {
      if(buf.IsFull())
        WriteToFile();
      buf.Write(node);  
    }

    void SaveBuf()
    {
      WriteToFile(); 
    }

    private:
    
    void WriteToFile()
    {
      fm.OpenW();
      while(buf.Count()>0)
      {        
        NodeBase node;
        buf.Read(node);
        fm.Write(TIME_SIZE, node.time); fm.Write(D_R_SIZE, D_R);
        fm.Write(ID_SIZE, node.id); fm.Write(D_R_SIZE, D_R);
        fm.Write(ID_SIZE, node.pid); fm.Write(D_R_SIZE, D_R);
        fm.Write(ID_BRANCH_SIZE, node.branch); fm.Write(D_R_SIZE, D_R);
        fm.Write(STATE_SIZE, node.nodeState); fm.Write(D_R_SIZE, D_R);
        fm.Write(INFO_SIZE, node.info.c_str()); fm.Write(1, '\n');
      }
      fm.Close();
    }
  };
}

#endif /* STORAGE_H */
