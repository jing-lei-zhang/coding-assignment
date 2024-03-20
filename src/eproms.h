#ifndef EPROMS_H
#define EPROMS_H
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "eprom.h"
#include <map>

using namespace std;



class Eproms {
    private:
    // the selected active file; Empty means no file is selected 
    string _loadedFile;
    map<string, Eprom> _eprom_files;

    Eproms():_eprom_files(){};

    static Eproms eproms;
    public:

    // return a single instance of Eproms
    static Eproms* getInstance(){
        return &eproms;
    }

    string getLoadedFile(){
        return _loadedFile;
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
        vector<Eprom> vec;
        for(auto it = _eprom_files.begin(); it!=_eprom_files.end(); it++){
            vec.push_back(it->second);
        }
        return vec;
    }
};



#endif
