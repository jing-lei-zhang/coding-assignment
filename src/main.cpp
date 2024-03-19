#include "eprom.h"
#include "eproms.h"
#include <iostream>
#include <exception>
#include <algorithm>
#include <iomanip>

using namespace std;

const char table[] = "0123456789ABCDEF";


inline bool isHex(char ch){
 return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f');
};


inline void listFiles(Eproms* pEproms){
    auto vec = pEproms->getEproms();
    if (vec.size() ==0){
        cout<<"No existing eprom files"<<endl;
    }
    else {
        cout<< "Existing eprom files are:"<<endl;
        for (auto it : vec){
            cout<< it.getFileName()<<"     Size: "<<it.getSize()<<" bytes"<<endl;
        }
    }
}

int main() {
    Eproms* pEproms = Eproms::getInstance();
    bool has_error = false;
    try {
        pEproms->loadExistingFiles();
        listFiles(pEproms);
        cout<<endl;
    }
    catch(exception& e) {
      cout << "Exception loading existing eprom files: "<< e.what() << endl;
      has_error = true;
    } 

   while(!has_error){
        try {
            string command;
            cout <<"Please enter command or 'Quit':"<<endl;
            cin >> command;

            transform(command.begin(), command.end(), command.begin(),[](unsigned char c){ return std::tolower(c); });  
            cout << command << endl;
            if (command == "quit" || command == "q"){
                break;
            }
            
            else if (command == "load"){
                string file;
                cin >> file;
                if(!pEproms->loadFile(file)) {
                    throw invalid_argument( string("no eprom file: '") + file + "'" );
                }
                cout << "File: '"<<file<<"' is loaded"<<endl;
                continue;
            }

            else if (command == "create"){
                string st_size_k;
                cin >> st_size_k;
                try {
                    int size_k = stoi(st_size_k);
                    if (size_k <1 || size_k >1000){
                        throw invalid_argument( "Eprom size must be in (1 ~ 1000) kilo" );
                    } 
                    string file = pEproms->createFile(size_k);
                    cout << "File: '"<<file<<"' is created"<<endl;
               
                }
                catch(exception& e) {
                        throw invalid_argument( "Eprom size must be in (1 ~ 1000) kilo" );
                }
                continue;              
            }

            else if (command == "list") {
                auto vec = pEproms->getEproms();
                if (vec.size() ==0){
                    cout<<"No existing eprom files"<<endl;
                }
                else {
                    cout<< "Existing eprom files are:"<<endl;
                    for (auto it : vec){
                        cout<< it.getFileName()<<endl;
                    }
                }
                continue;
            }

            else if (command == "erase") {
                pEproms->eraseContent();
                string file = (pEproms->getEproms())[pEproms->getCursor()].getFileName();
                cout<<"File '"<<file<<"'s content is erased."<<endl;
                continue;
            }

            else if (command == "write"){
                string inputFileName;
                cin >> inputFileName;
                pEproms->inputFile(inputFileName);
                string file = (pEproms->getEproms())[pEproms->getCursor()].getFileName();
                cout<<"File '"<<file<<"'s content is overwritten by file '"<<inputFileName<<"'"<<endl;
                continue;
            }

            else if (command == "read_raw"){
                string st_address;  // address value in 10 base
                cin >> st_address;
                int address = 0;
                try {
                    address = stoi(st_address);
                    if (address<0){
                        throw invalid_argument("");
                    }            
                }
                catch(exception& e) {
                        throw invalid_argument( "address must be a 10-based positive integer value" );
                }

                string st_length;  // address value in 10 base
                cin >> st_length;
                int length = 0;
                try {
                    length = stoi(st_length);
                    if (length<0){
                        throw invalid_argument("");
                    }            
                }
                catch(exception& e) {
                    throw invalid_argument( "length must be a 10-based positive integer value" );
                }

                string data = pEproms->readRaw(address, length);
                cout<< "Read content is:"<<endl;
                for (int i = 0; i < data.size(); ++i){
                    unsigned char ch = data[i];
                    cout << table[ch >> 4] <<table[ch & 0xf] <<" ";
                }
                cout << endl;               

                continue;
            }

            else if (command == "write_raw"){
                string st_address;  // address value in 10 base
                cin >> st_address;
                int address = 0;
                try {
                    address = stoi(st_address);
                    if (address<0){
                        throw invalid_argument("");
                    }            
                }
                catch(exception& e) {
                    throw invalid_argument( "address must be a 10-based positive integer value" );
                }

                string st_hex_data; // the hex data string
                cin >> st_hex_data;
                if (st_hex_data.size()==0){
                    cout<< "no need to write for empty data string"<<endl;
                    continue;
                }
                if (st_hex_data.size()%2==1){
                    throw invalid_argument( "data string must be in double-hex bytes" );
                }               
                transform(st_hex_data.begin(), st_hex_data.end(), st_hex_data.begin(),[](unsigned char c){ return std::tolower(c); });  
                for (int i = 0; i<st_hex_data.size(); i++){
                   if (!isHex(st_hex_data[i])){
                        throw invalid_argument( "data string must be in double-hex bytes" );
                    }
                }
                string str_bytes(st_hex_data.size()/2, '\0');
                for (int i = 0; i<st_hex_data.size(); i+=2){
                    string bs = st_hex_data.substr(i, 2);
                    unsigned char ch = (unsigned char) strtol(bs.c_str(), NULL, 16);
                    str_bytes[i/2] =ch;
                }
                pEproms->writeRaw(address, str_bytes);
                string file = (pEproms->getEproms())[pEproms->getCursor()].getFileName();
                cout<<"Write to File '"<<file<<"'"<<endl;
                continue;
            }

            else cout << "Invalid command: '"<<command<<"'"<<endl;

        }
        catch(exception& e) {
            cout << "Error: "<< e.what() << endl;
        } 

    }

    return 0;
}
