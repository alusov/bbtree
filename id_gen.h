#ifndef ID_GEN_H
#define ID_GEN_H

#define ID_SIZE 12
#define ID_BRANCH 2
#define STATE_SIZE 1
#define DEPTH_SIZE 5
#define TIME_SIZE 8
#define INFO_SIZE 2
#define D_R_SIZE 1

#define ID_NODE_OFFSET   TIME_SIZE + D_R_SIZE
#define PID_NODE_OFFSET  ID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define BRANCH_OFFSET    PID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define LID_NODE_OFFSET  BRANCH_OFFSET + ID_BRANCH + D_R_SIZE
#define RID_NODE_OFFSET  LID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define FID_NODE_OFFSET  RID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define STATE_OFFSET     FID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define DEPTH_OFFSET     STATE_OFFSET + STATE_SIZE + D_R_SIZE
#define INFO_SIZE_OFFSET DEPTH_OFFSET + DEPTH_SIZE + D_R_SIZE
#define INFO_OFFSET      INFO_SIZE_OFFSET + INFO_SIZE + D_R_SIZE

#define NUM_NODES_SIZE 12
#define DEPTH_RECORD_SIZE DEPTH_SIZE + D_R_SIZE + NUM_NODES_SIZE + D_R_SIZE + NUM_NODES_SIZE
#define NUM_NODES_OFFSET DEPTH_SIZE + D_R_SIZE
#define NUM_LIST_NODES_OFFSET NUM_NODES_OFFSET + NUM_NODES_SIZE + D_R_SIZE

#define D_R '|' //delimiter

namespace BBTree
{
  class IdGen
  {
     private:
     static int64_t curr_offset;

     public: 
     static int64_t GetId(size_t info)
     {
        int64_t ret = curr_offset;
        curr_offset+=(INFO_OFFSET + info + sizeof('\n'));
        return ret;
     }
  };
}

#endif /* ID_GEN_H */


