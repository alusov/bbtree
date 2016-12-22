#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <fstream> 

namespace BBTree
{
  class FileManager
  {
     private:
     std::string fileName;
     std::fstream fs;
     public: 
     FileManager(const std::string& file)
     {
       this->fileName=file;
     }

     FileManager()
     {
     }

     std::string GetFile()
     {
       return fileName;
     }

     void SetFile(const std::string& file)
     {
       fileName = file;
     }

     bool IsOpen()
     {
       return fs.is_open();
     }

     void OpenRW()
     {
       fs.exceptions(std::ifstream::failbit | std::ifstream::badbit );
       fs << std::right;
       fs << std::setprecision(2);
       fs << std::fixed;
       fs.open(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
     }
     
     void OpenW()
     {
       fs.exceptions(std::ifstream::failbit | std::ifstream::badbit );
       fs << std::right;
       fs << std::setprecision(2);
       fs << std::fixed;
       fs.open(fileName.c_str(), std::fstream::out | std::fstream::binary);
     }
     
     void OpenWA()
     {
       fs.exceptions(std::ifstream::failbit | std::ifstream::badbit );
       fs << std::right;
       fs << std::setprecision(2);
       fs << std::fixed;
       fs.open(fileName.c_str(), std::fstream::app | std::fstream::out | std::fstream::binary);
     } 

     void Close()
     {
       if(fs.is_open())
         fs.close();
     }

     int64_t getFileSize() 
     {
       std::ifstream ifs(fileName.c_str(), std::ifstream::ate | std::ifstream::binary);
       return ifs.tellg();   
     }

     std::string Read(int64_t pos, int size)
     {
       fs.seekg(pos, std::ios_base::beg);
       char buf[size+1];
       fs.read(buf, size);
       buf[size] = '\0';
       return std::string(buf);
     }

     void Write(int64_t pos, int size, const std::string& value)
     {
     	fs.seekg(pos, std::ios_base::beg);
      	fs << std::setw(size) << value;
     }
     void Write(int64_t pos, int size, int value)
     {
     	fs.seekg(pos, std::ios_base::beg);
      	fs << std::setw(size) << value;
     }
     void Write(int64_t pos, int size, double value)
     {
     	fs.seekg(pos, std::ios_base::beg);
      	fs << std::setw(size) << value;
     }

     void Write(int size, const std::string& value)
     {
      	fs << std::setw(size) << value.substr(0, size);
     }
     void Write(int size, int value)
     {
      	fs << std::setw(size) << value;
     }
     void Write(int size, char value)
     {
      	fs << std::setw(size) << value;
     }
     void Write(int size, double value)
     {
      	fs << std::setw(size) << value;
     }
  };
}

#endif /* FILE_MANAGER_H */
