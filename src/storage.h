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

///It is size of ring buffer
#define BUFFER_SIZE 2097152 //2^21


namespace BBTree
{
///Storage class stores tree's nodes in buffer and finally saves it in file buf.data
  class Storage
  {
    private:
///FileManager object writes data in file.
    FileManager fm;
///RingBuffer object is ring buffer to store tree's nodes befor saving in file.
    RingBuffer<BUFFER_SIZE, NodeBase> buf;
    public:
/**
\brief Constructor. It initialize file manager.
\param[in] file buf.data
*/
    Storage(const std::string& file) : fm(file)
    {
    }
/**
\brief It writes a node in the ring buffer. If buffer is full it saves data in the file buf.data.
\param[in] node Tree's node to save.
*/
    void Write(const NodeBase& node)
    {
      if(buf.IsFull())
        WriteToFile();
      buf.Write(node);  
    }

///It saves data in file. Note you should call it at the end of tree creation to free ring buffer.
    void SaveBuf()
    {
      if(buf.Count()>0)
        WriteToFile(); 
    }

    ~Storage()
    {
      SaveBuf();
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
