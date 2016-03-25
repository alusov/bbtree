#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sys/time.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "def.h"

namespace BBTree
{
  class Utils
  {
     public: 
     static void GetTime(std::string& time)
     {
        timeval tp;
        gettimeofday(&tp, 0);
        tm* t = localtime(&tp.tv_sec);
        char timebuf[TIME_SIZE+1];
        sprintf(timebuf, "%02d:%02d:%02d:%03d", t->tm_hour, t->tm_min, t->tm_sec, (int)tp.tv_usec/1000);
        time = timebuf;
     }

     static int64_t Position(int node, int recordSize, int offset)
     {
       return (int64_t)(node-1)*recordSize+offset;
     }

     static int64_t Position(int node, int recordSize)
     {
       return Position(node,recordSize,0);
     }

     static std::string GetLevelFileName(int level)
     {
       std::stringstream fileNameAllNodes;
       fileNameAllNodes << FILE_DEPTH_FOLDER << level << FILE_DEPTH;
       return fileNameAllNodes.str();
     }

     static std::string GetListLevelFileName(int level)
     {
       std::stringstream fileNameListNodes;
       fileNameListNodes << FILE_DEPTH_FOLDER << 'l' << level << FILE_DEPTH;
       return fileNameListNodes.str();
     }
  };
}

#endif /* UTILS_H */
