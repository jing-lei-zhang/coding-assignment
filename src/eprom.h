#ifndef EPROM_H
#define EPROM_H
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


#define PATH "./veprom"
#define KILO 1024

class Eprom {
    private:
    static int seq_num; //The number for the next new file name to be created

    int _size; //the file size in bytes
    string _fileName; // the file name with path of the eprom file in managed location
    bool _load; //true, means there is a corresponding file in the managed location; false, not sure


    public:
    Eprom(int sz, string fileName);
    ~Eprom();
    bool isLoaded();
    void setLoaded(bool val);
    int getSize();
    string getFileName();

    // create a new eprom file in the managed location with _fileName and _size. The contents are all 1
    void createFile();

    // overwrite the content of a section of the eprom file with the passed in data
    void writeRaw(int startAddress, string bytes);

    // return the content of a section of the eprom file as a string
    string readRaw(int startAddress, int length, string inputFileName="");

    // copy the content of input file into the eprom file
    void inputFile(string inputFileName);

    // write all 1 to the eprom file
    void eraseContent();


    // coming up with a new eprom file name in the managed location
    static string generateFileName();

    // return a list of Eprom objects corresponsed to the eprom files in the managed location
    static vector<Eprom> loadFiles();

};

#endif