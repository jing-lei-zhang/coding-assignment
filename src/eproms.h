#ifndef EPROMS_H
#define EPROMS_H
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "eprom.h"

using namespace std;



class Eproms {
    private:
    // the index of _eprom_files. 
    // The member of _eprom_files pointed by cursor is active for subsequen operations when it's in the range of _eprom_files.
    // It's invalid when less than 0, or out of range
    int _cursor = -1;   
    vector<Eprom> _eprom_files;
    Eproms* _pEproms = NULL;
    Eproms(){};

    static Eproms eproms;
    public:

    // return a single instance of Eproms
    static Eproms* getInstance(){
        return &eproms;
    }

    int getCursor(){
        return _cursor;
    }

    // fill _eprom_files with members corresponding to the exsting eprom files in the managed location
    void loadExistingFiles();

    // create a new eprom file in the managed location in size_k * 1000 byte size
    // when successful, the file is also loaded
    string createFile(int size_k);

    // set the eprom file named by fileName to active
    // return true when success; false otherwise
    // fileName - a file name with path info
    bool loadFile(string fileName);

   // overwrite the content of a section of the active eprom file with the passed in data (hex byte string)
    void writeRaw(int startAddress, string bytes);

   // return the content of a section of the active eprom file as a string
    string readRaw(int startAddress, int length);

    // copy the content of input file into the active eprom file
    void inputFile(string inputFileName);

    // erase the active file content to all 1
    // exception when the op is not successful
    void eraseContent();

    // return _eprom_files list 
    vector<Eprom> getEproms(){
        return _eprom_files;
    }
};



#endif
