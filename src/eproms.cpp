#include "eproms.h"
#include "eprom.h"

Eproms Eproms::eproms;

void Eproms::loadExistingFiles(){
    _eprom_files.clear();
    auto files = Eprom::loadFiles();
    for(auto it=files.begin(); it!=files.end(); it++){
        _eprom_files.insert({it->getFileName(),*it});
    }
}

string Eproms::createFile(int size_k){
    int sz = size_k * KILO;
    string fileName = Eprom::generateFileName();
    Eprom e(sz, fileName);
    e.createFile();
    e.setLoaded(true);
    _eprom_files.insert({fileName, e});
    loadFile(fileName);
    return fileName;
}

bool Eproms::loadFile(string fileName){
    _loadedFile = "";
    if (_eprom_files.find(fileName)!=_eprom_files.end()){
        _loadedFile = fileName;
        return true;
    }
    return false;
}

void Eproms::writeRaw(int startAddress, string bytes){
    if (_loadedFile == ""){
        throw invalid_argument( "no loaded eprom file" );
    }
    _eprom_files.at(_loadedFile).writeRaw(startAddress, bytes);
}

string Eproms::readRaw(int startAddress, int length){
    if (_loadedFile == ""){
        throw invalid_argument( "no loaded eprom file" );
    }
    return _eprom_files.at(_loadedFile).readRaw(startAddress, length);
}


void Eproms::inputFile(string inputFileName){
    if (_loadedFile == ""){
        throw invalid_argument( "no loaded eprom file" );
    }
    _eprom_files.at(_loadedFile).inputFile(inputFileName);
}

void Eproms::eraseContent(){
    if (_loadedFile == ""){
        throw invalid_argument( "no loaded eprom file" );
    }
    _eprom_files.at(_loadedFile).eraseContent();
}