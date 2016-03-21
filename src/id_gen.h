#ifndef ID_GEN_H
#define ID_GEN_H

namespace BBTree
{
  class IdGen
  {
     private:
     static int curr_offset;

     public: 
     static int GetId()
     {
        return curr_offset++;
     }
  };
}

#endif /* ID_GEN_H */


