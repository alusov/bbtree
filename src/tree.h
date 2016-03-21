#ifndef TREE_H
#define TREE_H

#include "file_manager.h"
#include "node.h"
#include "def.h"
#include "utils.h"
#include <string>
#include <fstream> 
#include <memory>
#include <vector>

namespace BBTree
{
  class Tree
  {
     private:
     FileManager fm;
     
     public: 
     Tree(const std::string& file): fm(file)
     {
     }
     void TreeLayout()
     {
        fm.Open();
        NodePtr root = RootNode();
        FirstWalk(root);
        SecondWalk(root, -root->prelim, 0);
        fm.Close();  
     }

     private:
     typedef std::shared_ptr<Node> NodePtr;

     NodePtr RootNode()
     {
       return NodeHelper::GetNode(fm.Read(0, RECORD_EXT_SIZE));
     }

     void SetPrelim(const NodePtr& node, double value)
     {
       node->prelim = value;
       fm.Write(Utils::Position(node->id, RECORD_EXT_SIZE, PRELIM_OFFSET), PRELIM_SIZE, value);
     }

     void SetMod(const NodePtr& node, double value)
     {
       node->mod = value;
       fm.Write(Utils::Position(node->id, RECORD_EXT_SIZE, MODIF_OFFSET), MODIF_SIZE, value);       
     }

     void SetX(const NodePtr& node, double value)
     {
       node->x = value;
       fm.Write(Utils::Position(node->id, RECORD_EXT_SIZE, X_OFFSET), X_SIZE, value);
     }

     void SetY(const NodePtr& node, int value)
     {
       node->y = value;
       fm.Write(Utils::Position(node->id, RECORD_EXT_SIZE, Y_OFFSET), Y_SIZE, value);
     }

     void SetAncestor(const NodePtr& node, int value)
     {
       node->ancestor = value;
       fm.Write(Utils::Position(node->id, RECORD_EXT_SIZE, ANCESTOR_OFFSET), ANCESTOR_SIZE, value);
     }

     void SetThread(const NodePtr& node, int value)
     {
       node->thread = value;
       fm.Write(Utils::Position(node->id, RECORD_EXT_SIZE, THREAD_OFFSET), THREAD_SIZE, value);
     }


     void FirstWalk(const NodePtr& node)
     {
       if(node->fid==0)//list node
       {
         if(node->lid)//there is left sibling
           SetPrelim(node, GetNode(node->lid)->prelim + NODE_DISTANCE);
       }
       else
       {
         NodePtr child = GetNode(node->fid);  
         NodePtr defaultAncestor = child;
         std::vector<ShiftChange> vec;
	 vec.push_back(ShiftChange(node->fid));            
	 FirstWalk(child);
         while(child->rid)
         {
           child = GetNode(child->rid);
           FirstWalk(child);
           vec.push_back(ShiftChange(child->id));
           Apportion(child, defaultAncestor, vec);
         }		 
         ExecuteShifts(vec);
	 double midPoint = (GetNode(node->fid)->prelim + GetNode(child->id)->prelim)/2;
         if(node->lid)
	 {
           NodePtr leftSibling = GetNode(node->lid);
           SetPrelim(node, leftSibling->prelim + NODE_DISTANCE);
           SetMod(node, leftSibling->prelim + NODE_DISTANCE - midPoint);
	 }
	 else
           SetPrelim(node, midPoint);
	}
     }     

     void SecondWalk(const NodePtr& node, double mod, int y)
     {
        SetX(node, node->prelim + mod);  
        SetY(node, y); 
        int childId = node->fid;
        while(childId)
        {
          NodePtr child = GetNode(childId);
          SecondWalk(child, mod + node->mod, y+1);
          childId = child->rid;
        }
     }

     NodePtr GetNode(int id)
     {
       if(id!=0)
         return NodeHelper::GetNode(fm.Read(Utils::Position(id, RECORD_EXT_SIZE), RECORD_EXT_SIZE));
       else
         return std::shared_ptr<Node>();
     }

     NodePtr NextLeft(const NodePtr& node)
     {
        if(node->fid)
          return GetNode(node->fid);
        else if(node->thread)
          return GetNode(node->thread);
        else
          return NodePtr();
     }

     NodePtr NextRight(const NodePtr& node)
     {
        if(node->fid)
        {
          NodePtr child = GetNode(node->fid);
          while(child->rid)
            child = GetNode(child->rid);
          return child;
        }
        else if(node->thread)
          return GetNode(node->thread);
        else
          return NodePtr();
     }

     NodePtr LeftMostSibling(const NodePtr& node)
     {
       NodePtr leftSibling = node;
       while(leftSibling->lid)
         leftSibling = GetNode(leftSibling->lid);
       return leftSibling;
     }
 
     NodePtr Ancestor(const NodePtr& leftIndide, const NodePtr& node, const NodePtr& defaultAncestor)
     {
       NodePtr ancestor = GetNode(leftIndide->ancestor);
       if(ancestor->pid==node->pid)
         return ancestor;
       else
         return defaultAncestor;
     }

     void Apportion(const NodePtr& v, NodePtr& defaultAncestor, std::vector<ShiftChange>& shiftChange)
     {
       if(v->lid)
       {
         NodePtr vRightInside = v;
         NodePtr vRightOutside = v;
         NodePtr vLeftInside = GetNode(v->lid);
         NodePtr vLeftOutside = LeftMostSibling(vRightInside);

         double sRightInside = vRightInside->mod;
         double sRightOutside = vRightOutside->mod;
         double sLeftInside = vLeftInside->mod;
         double sLeftOutside = vLeftOutside->mod;

         while(NextRight(vLeftInside) !=NULL && NextLeft(vRightInside) !=NULL)
         {
           vLeftInside = NextRight(vLeftInside);     
           vLeftOutside = NextLeft(vLeftOutside);
           vRightInside = NextLeft(vRightInside);
           vRightOutside = NextRight(vRightOutside);

           SetAncestor(vRightOutside, v->id);
           double shift = (vLeftInside->prelim + sLeftInside)-(vRightInside->prelim + sRightInside) + NODE_DISTANCE;
           if(shift > 0)
           {
             MoveSubTree(Ancestor(vLeftInside,v,defaultAncestor),v,shift,shiftChange);
             sRightInside += shift;
             sRightOutside += shift;
           }
           sLeftInside += vLeftInside->mod;
           sLeftOutside += vLeftOutside->mod;
           sRightInside += vRightInside->mod;
           sRightOutside += vRightOutside->mod;
         }
         if((vLeftInside = NextRight(vLeftInside))!=NULL && NextRight(vRightOutside)==NULL)
         {
           SetThread(vRightOutside, vLeftInside->id);
           SetMod(vRightOutside, vRightOutside->mod + sLeftInside - sRightOutside);
         }
         if((vRightInside = NextLeft(vRightInside))!=NULL && NextLeft(vLeftOutside)==NULL)
         {
           SetThread(vLeftOutside, vRightInside->id);
           SetMod(vLeftOutside, vLeftOutside->mod + sRightInside - sLeftOutside);
           defaultAncestor = v;
         }
         
       }
     }

     void MoveSubTree(const NodePtr& leftRoot, const NodePtr& rightRoot, double shift, std::vector<ShiftChange>& shiftChange)
     {
       int subtrees = rightRoot->branch - leftRoot->branch;
       int id = rightRoot->id;
       auto it = std::find_if(shiftChange.begin(), shiftChange.end(), [id](const ShiftChange& sh){return sh.id==id;});
       if(it!=shiftChange.end())
       {
         it->change = it->change - shift/(double)subtrees;
         it->shift = it->shift + shift;
       }
       else
       {
         std::cerr << "Right root was not found.\n";
         exit(EXIT_FAILURE);
       }

       id = leftRoot->id;
       it = std::find_if(shiftChange.begin(), shiftChange.end(), [id](const ShiftChange& sh){return sh.id==id;});
       if(it!=shiftChange.end())
         it->change = it->change + shift/(double)subtrees;
       else
       {
         std::cerr << "Left root was not found.\n";
         exit(EXIT_FAILURE);
       }
       SetPrelim(rightRoot, rightRoot->prelim + shift);
       SetMod(rightRoot, rightRoot->mod + shift);
     }

     void ExecuteShifts(const std::vector<ShiftChange>& shiftChange)
     {
       double shift = 0;
       double change = 0;
       for(auto it = shiftChange.rbegin(); it!=shiftChange.rend(); ++it)
       {
         NodePtr node = GetNode(it->id);
         SetPrelim(node, node->prelim + shift);
         SetMod(node, node->mod + shift);
         change += it->change;
         shift += (it->shift + change);
       }
     }
  };
}

#endif /* TREE_H */
