#include "eproms.h"
#include "eprom.h"

Eproms Eproms::eproms;

void Eproms::loadExistingFiles(){
    _eprom_files = Eprom::loadFiles();
}

string Eproms::createFile(int size_k){
    int sz = size_k * KILO;
    string fileName = Eprom::generateFileName();
    Eprom e(sz, fileName);
    e.createFile();
    e.setLoaded(true);
    _eprom_files.push_back(e);
    loadFile(fileName);
    return fileName;
}

bool Eproms::loadFile(string fileName){
    for (int i = 0; i<_eprom_files.size(); i++){
        if (_eprom_files[i].getFileName()==fileName){
            _cursor = i;
            return true;
        }
    }
    _cursor = -1; //invalidate it
    return false;
}

void Eproms::writeRaw(int startAddress, string bytes){
    if (_cursor<0 || _cursor>=_eprom_files.size()){
        throw invalid_argument( "no loaded eprom file" );
    }
    _eprom_files[_cursor].writeRaw(startAddress, bytes);
}

string Eproms::readRaw(int startAddress, int length){
    if (_cursor<0 || _cursor>=_eprom_files.size()){
        throw invalid_argument( "no loaded eprom file" );
    }
    return _eprom_files[_cursor].readRaw(startAddress, length);
}


void Eproms::inputFile(string inputFileName){
    if (_cursor<0 || _cursor>=_eprom_files.size()){
        throw invalid_argument( "no loaded eprom file" );
    }
    _eprom_files[_cursor].inputFile(inputFileName);
}

void Eproms::eraseContent(){
    if (_cursor<0 || _cursor>=_eprom_files.size()){
        throw invalid_argument( "no loaded eprom file" );
    }
    _eprom_files[_cursor].eraseContent();
}