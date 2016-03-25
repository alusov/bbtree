#ifndef NODE_ANALYSIS_H
#define NODE_ANALYSIS_H

#include <string>
#include <iostream>
#include <fstream> 
#include <iomanip>
#include "def.h"
#include "node.h"
#include "utils.h"
#include "file_manager.h"

namespace BBTree
{
///Node analysis class calculates node's left sibling, node's right sibling and first child.
class NodeAnalysis
{
private:
  std::string fileName;
  FileManager fm;
public:
/**
\brief Constructor.

\param[in] fileName File name to store tree's node. We use buf.data here.
\param[in] newFileName We us buf_new.data to extend buf.data.
*/
  NodeAnalysis(const std::string& fileName, const std::string& newFileName) : fm(newFileName)
  {
    this->fileName = fileName; 
  }


///Main function. It copies data from buf.data file to buf_new.data. It adds new fields, see field's description in readme. Then it calculates
///node's left sibling, node's right sibling and node's first child.  
  int DoAnalysis()
  {
    CopyAndExtendFile(); 
    fm.Open();
    int64_t streamsize = fm.getFileSize();
    int64_t currPos = 0;
    NodePtr node = NodeHelper::GetNode(fm.Read(currPos, RECORD_EXT_SIZE));
    currPos+=RECORD_EXT_SIZE;
    CheckRoot(node);
    do
    {
      node = NodeHelper::GetNode(fm.Read(currPos, RECORD_EXT_SIZE));
      currPos+=RECORD_EXT_SIZE;
      UpdateParentRecord(node);
    }
    while(currPos<streamsize);
    fm.Close();
    return 0;
  }

private:
  typedef std::shared_ptr<Node> NodePtr;

  void CopyAndExtendFile()
  {  
      
    FileManager fmOld(fileName);
    fmOld.Open();
    FileManager fmExt(fm.GetFile());       
    fmExt.OpenW();
    
    int64_t streamsize = fmOld.getFileSize();
    int64_t currPos = 0;
    std::string record;
    while(currPos < streamsize)
    {
       record = fmOld.Read(currPos, RECORD_SIZE-1);
       fmExt.Write(RECORD_SIZE-1, record); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(ID_SIZE, 0); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(ID_SIZE, 0); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(ID_SIZE, 0); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(X_SIZE, 0.0); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(Y_SIZE, 0); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(PRELIM_SIZE, 0.0); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(MODIF_SIZE, 0.0); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(THREAD_SIZE, 0); fmExt.Write(D_R_SIZE, D_R);
       fmExt.Write(ANCESTOR_SIZE, record.substr(ID_NODE_OFFSET, ID_SIZE)); fmExt.Write(1, '\n');
       currPos+=RECORD_SIZE;
    }   
    fmOld.Close();
    fmExt.Close();
  }

  void UpdateParentRecord(const NodePtr& child)
  {
    NodePtr parent = NodeHelper::GetNode(fm.Read(Utils::Position(child->pid, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
    if(parent->fid==0)//first child node
      fm.Write(Utils::Position(child->pid, RECORD_EXT_SIZE, FID_NODE_OFFSET), ID_SIZE, child->id);
    else
      AddNodeToList(parent->fid, child->id);
  }

  void AddNodeToList(int head, int newnode)
  {
    NodePtr node = NodeHelper::GetNode(fm.Read(Utils::Position(head, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
    while(node->rid!=0)
      node = NodeHelper::GetNode(fm.Read(Utils::Position(node->rid, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
    fm.Write(Utils::Position(node->id, RECORD_EXT_SIZE, RID_NODE_OFFSET), ID_SIZE, newnode);
    fm.Write(Utils::Position(newnode, RECORD_EXT_SIZE, LID_NODE_OFFSET), ID_SIZE, node->id);   
  }

  void CheckRoot(const NodePtr& node)
  {   
    if(node->id!=1 || node->pid!=0)
    {
      std::cerr << "TreeAnalysis: root node is not valid\n";
      exit(EXIT_FAILURE);
    }   
  }

};

}

#endif /* NODE_ANALYSIS_H */
