#include "eprom.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>


using namespace std;

int Eprom::seq_num = 0;


Eprom::Eprom(int sz, string fileName):_size(sz),_load(false), _fileName(fileName){

}


Eprom::~Eprom(){
}

bool Eprom::isLoaded(){
    return _load;
}

void Eprom::setLoaded(bool val){
    _load = val;
}

int Eprom::getSize(){
    return _size;
}

string Eprom::getFileName(){
    return _fileName;
}

void Eprom::createFile(){
  int N=KILO;
  char buf[N];
  for (int i = 0; i< N; i++){
    buf[i] = 0xff;
  }
  ofstream fs(_fileName, std::ios::out | std::ios::binary);
  int k = _size / KILO;
  for (int i = 0; i<k; i++){
    fs.write(buf,N);
  }
  fs.close();
}

void Eprom::writeRaw(int startAddress, string bytes){
    if (!_load || _fileName.size()==0){
        throw invalid_argument( "missing eprom file" );
    }

    if (startAddress<0 || startAddress + bytes.size() > _size){
        throw invalid_argument( "incorrect start address or data size" );
    }

    if (bytes.size()==0){
        return;
    }

    basic_fstream<char> fileStream(_fileName , ios::out | ios::in | ios::binary );
    fileStream.seekp(startAddress);
    fileStream.write(bytes.c_str(), bytes.size());
    fileStream.close(); 
}

string Eprom::readRaw(int startAddress, int length, string inputFileName){
    if (!_load || _fileName.size()==0){
        throw invalid_argument( "missing eprom file" );
    }

    if (startAddress<0 || length < 0 ||startAddress + length > _size){
        throw invalid_argument( "incorrect start address or length" );
    }

    if (length ==0){
        return "";
    }

    if (inputFileName.size() ==0){
        inputFileName = _fileName;
    }

    char bytes[length+1] = {};
    basic_fstream<char> fileStream(inputFileName , ios::out | ios::in | ios::binary );
    fileStream.seekp(startAddress);
    fileStream.read(bytes, length);
    fileStream.close();
    return string(bytes, length);
}

void Eprom::inputFile(string inputFileName){
    ifstream inf(inputFileName, ifstream::ate | ifstream::binary);
    if (!inf.good()){
        throw invalid_argument( string("file '") + inputFileName + "' is invalid");
    }
    
    if (inf.tellg() != _size){
        throw invalid_argument( "source file size differ from that of the destination" );
    }
    writeRaw(0,readRaw(0,inf.tellg(),inputFileName));
}

void Eprom::eraseContent(){
    if (!_load || _fileName.size()==0 || _size<=0){
        throw invalid_argument( "missing eprom file" );
    }
    string st(_size, 0xff);
    writeRaw(0,st);  
}

string Eprom::generateFileName(){
  ostringstream stm;
  stm << PATH<<"/feprom_"<<seq_num;
  seq_num++;
  string fileName = stm.str();
  //cout<<"file name is: " << fileName <<endl;
  return fileName;
}

vector<Eprom> Eprom::loadFiles(){
    vector<Eprom> vec;
    std::string path = PATH;
    for (const auto & entry : filesystem::directory_iterator(PATH)){
        string file = entry.path();
        try {
            auto pos = file.find_last_of('_');
            if (pos != string::npos && (pos+1)<file.size()){
                string sub = file.substr(pos+1);
                int idx = stoi(sub);
                if (idx >= seq_num){
                    seq_num = idx + 1;
                }
                std::ifstream inf(file, std::ifstream::ate | std::ifstream::binary);
                int sz=inf.tellg();
                Eprom e(sz,file);
                e.setLoaded(true);
                vec.push_back(e);

                //cout << " file size is:"<<sz<<endl;
                //cout <<" sub number is: "<< sub<<endl;
                //cout <<" seq_num is: "<< seq_num<<endl;
            }
            //cout << entry.path() << std::endl;
        }
        catch(exception& e) {
            throw invalid_argument( string("can't load file '")+file+"'" );
        }
    }
    return vec;
}
