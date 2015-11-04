#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <string>
#include <cstdio>
#include <fstream> 
#include <iomanip>
#include "event.h"
#include "ring_buffer.h"
#include "id_gen.h"

#define BUFFER_SIZE 2097152 //2^21


namespace BBTree
{
  class Storage
  {
    private:
    std::string file;
    RingBuffer<BUFFER_SIZE, Event> buf;
    char timebuf[TIME_SIZE+1];
    public:

    Storage(const std::string& file)
    {
      this->file = file;
      Clear(); 
    }

    void Write(const Event& event)
    {
      if(buf.IsFull())
        WriteToFile();
      buf.Write(event);  
    }

    void SaveBuf()
    {
      WriteToFile(); 
    }

    private:

    void Clear()
    {
      remove(file.c_str());
    }
    
    void WriteToFile()
    {
      std::fstream fs;
      Open(fs);
      while(buf.Count()>0)
      {        
        Event event;
        buf.Read(event);
        Write(fs, event);
      }
      fs.close();
    }

    void Open(std::fstream& fs)
    {
      fs.exceptions(std::ifstream::failbit | std::ifstream::badbit );
      fs << std::right;
      fs.open(file.c_str(), std::fstream::app | std::fstream::out | std::fstream::binary);
    }
 
    void Write(std::fstream& fs, const Event &event)
    {
       tm* timeinfo = localtime(&event.time);
       strftime(timebuf, TIME_SIZE + 1, "%I:%M:%S", timeinfo);
       int infosize = event.info.length();

       fs << std::setw(TIME_SIZE) << timebuf << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(ID_SIZE) << event.id << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(ID_SIZE) << event.pid << std::setw(D_R_SIZE) << D_R;  
       fs << std::setw(ID_BRANCH) << event.branch << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(ID_SIZE) << (int64_t)0 << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(ID_SIZE) << (int64_t)0 << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(ID_SIZE) << (int64_t)0 << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(STATE_SIZE) << event.nodeState << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(DEPTH_SIZE) << 0 << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(INFO_SIZE) << infosize << std::setw(D_R_SIZE) << D_R;
       fs << std::setw(infosize) << event.info.c_str() << "\n";
    }
  };
}

#endif /* STORAGE_H */
