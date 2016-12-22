/**
\brief The API file

This file contains API to log events from a solver. Now it is just two function to create tree's node or update its state.

*/
#ifndef ID_API_H
#define ID_API_H

#include <string>
#include "node.h"
#include "storage.h"
#include "id_gen.h"
#include "utils.h"
#include "def.h"

namespace BBTree
{
/// Storage object to write events in buffer and finally to save it in file
  Storage storage(FILE_NAME);
  int IdGen::curr_offset = 1;

/**
\brief API function to create node.

This is callback function. Solver or tree simulator can use it to generate events when a node is created.

\param[in] parentId Parent node's id. Note, use 0 for root node.
\param[in] Id node's id.
\param[in] state Node's state. We can fill enum with necessary states.
\param[in] branch It's number of node in linked list. Start number from 1.
\param[in] info It is node's data. It is any information connected with node.
*/
  void create_node(int parentId, int id, NodeState state, short branch, const std::string &info)
  {
     NodeBase node;
     node.id = id;
     node.idEnd = 0;
     node.pid = parentId;
     node.branch = branch;
     Utils::GetTime(node.time);
     node.nodeState = state;
     node.info = info;
     storage.Write(node);
  }

  
/**
\brief API function to update node's state.

This is callback function. Solver or tree simulator can use it to generate events when a state of a node is changed.
Note, it is not implemented yet. Please, don't use it.

\param[in] id Node's id.
\param[in] state Node's state.
\param[in] info It is node's data. It is any information connected with node.
*/
  void update_node(int id, int idEnd, NodeState state)
  {
     NodeBase node;
     node.id = id;
     node.idEnd = idEnd;
     node.nodeState = state;
     storage.Update(node);
  }

}

#endif /* ID_API_H */

