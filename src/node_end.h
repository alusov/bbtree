/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   node_end.h
 * Author: Alexander Usov
 *
 * Created on December 21, 2016, 8:03 PM
 */

#ifndef NODE_END_H
#define NODE_END_H

#include <string>
//#include <sstream>
#include "def.h"
#include "node.h"
#include "utils.h"
#include "file_manager.h"

namespace BBTree
{
    class NodeEnd
    {
        private:
        std::string fileName;
        std::string fileNodeEndName;
        
        public:
        NodeEnd(const std::string& fileName, const std::string& fileNodeEndName)
        {
            this->fileName = fileName; 
            this->fileNodeEndName = fileNodeEndName;
        }
        
        void CreateRelations()
        {
            FileManager fm(fileName), fm_e(fileNodeEndName);
            fm.OpenRW();
            fm_e.OpenW();
            int64_t streamsize = fm.getFileSize();
            int64_t currPos = 0;
            
            while(currPos < streamsize)
            {
                auto node = NodeHelper::GetNode(fm.Read(currPos, RECORD_EXT_SIZE));
                currPos+=RECORD_EXT_SIZE;
                if(node->idEnd != 0)
                {
                    fm_e.Write(Utils::Position(node->idEnd, ID_NODE_END_RECORD_SIZE), ID_SIZE, node->idEnd);
                    fm_e.Write(D_R_SIZE, D_R);
                    fm_e.Write(ID_SIZE, node->id);
                    fm_e.Write(1, '\n');
                }
            }
            fm.Close();
            fm_e.Close();
        }
    };
}

#endif /* NODE_END_H */

