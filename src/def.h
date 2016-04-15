#ifndef DEF_H
#define DEF_H

#define TIME_SIZE      12
#define ID_SIZE        9
#define ID_BRANCH_SIZE 2
#define STATE_SIZE     1
#define INFO_SIZE      15
#define D_R_SIZE       1
#define X_SIZE         12
#define Y_SIZE         5
#define PRELIM_SIZE    12
#define MODIF_SIZE     12
#define THREAD_SIZE    9
#define ANCESTOR_SIZE  9


#define TIME_OFFSET      0
#define ID_NODE_OFFSET   TIME_OFFSET + TIME_SIZE + D_R_SIZE
#define PID_NODE_OFFSET  ID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define BRANCH_OFFSET    PID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define STATE_OFFSET     BRANCH_OFFSET + ID_BRANCH_SIZE + D_R_SIZE
#define INFO_OFFSET      STATE_OFFSET + STATE_SIZE + D_R_SIZE

//extension offsets
#define LID_NODE_OFFSET  INFO_OFFSET + INFO_SIZE + D_R_SIZE
#define RID_NODE_OFFSET  LID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define FID_NODE_OFFSET  RID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define X_OFFSET         FID_NODE_OFFSET + ID_SIZE + D_R_SIZE
#define Y_OFFSET         X_OFFSET + X_SIZE + D_R_SIZE
#define PRELIM_OFFSET    Y_OFFSET + Y_SIZE + D_R_SIZE
#define MODIF_OFFSET     PRELIM_OFFSET + PRELIM_SIZE + D_R_SIZE
#define THREAD_OFFSET    MODIF_OFFSET + MODIF_SIZE + D_R_SIZE
#define ANCESTOR_OFFSET  THREAD_OFFSET + THREAD_SIZE + D_R_SIZE

#define RECORD_SIZE      (TIME_SIZE + 2*ID_SIZE + ID_BRANCH_SIZE + STATE_SIZE + INFO_SIZE + 5*D_R_SIZE + 1)
#define RECORD_EXT_SIZE  (RECORD_SIZE + 3*ID_SIZE + X_SIZE + Y_SIZE + PRELIM_SIZE + MODIF_SIZE + THREAD_SIZE + ANCESTOR_SIZE + 9*D_R_SIZE)//151

#define NUM_NODES_SIZE              9
#define DEPTH_RECORD_SIZE           (ID_SIZE + D_R_SIZE + X_SIZE + 1)
#define ALL_DEPTH_RECORD_SIZE       (Y_SIZE + 2*NUM_NODES_SIZE + 2*D_R_SIZE + 1)
#define DEPTH_X_OFFSET              ID_SIZE + D_R_SIZE
#define NUM_NODES_OFFSET            Y_SIZE + D_R_SIZE
#define NUM_LIST_NODES_OFFSET       NUM_NODES_OFFSET + NUM_NODES_SIZE + D_R_SIZE

#define D_R '|' //delimiter
#define NODE_DISTANCE 1

#define FILE_NAME "./../data/buf.data"
#define FILE_NAME_NEW "./../data/buf_new.data"
#define FILE_PATH_DEPTH "./../data/depth.data"
#define FILE_DEPTH_FOLDER "./../data/"
#define FILE_DEPTH "depth.data"

#endif /* DEF_H */
