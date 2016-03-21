#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sys/time.h>
#include <stdio.h>

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
       //std::cout << "node " << node << " recordSize " << recordSize << " offset " << offset << '\n';
       return (int64_t)(node-1)*recordSize+offset;
     }

     static int64_t Position(int node, int recordSize)
     {
       return Position(node,recordSize,0);
     }
  };
}

#endif /* UTILS_H */
