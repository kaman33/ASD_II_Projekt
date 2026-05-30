#include "Struktury/Huffman.h"
#include "Struktury/Funkcje/KMP.h"
#include<iostream>
#include<filesystem>
#include<string>
#include<fstream>
#include<unordered_map>
namespace fs = std::filesystem;
using namespace std;

string currentPath = fs::current_path().string();;

int modeSelector(){
    int mode;
    do{
    cout<<"Select mode:"<<endl;
    cout<<"1. Compress file"<<endl;
    cout<<"2. Decompress file"<<endl;
    cin>>mode;
    } while(mode != 1 && mode != 2);
    return mode;
}

int printFolderContents(string path = currentPath){
    int i = 0;
    for (const auto& entry : fs::directory_iterator(path)){
        if(!fs::is_regular_file(entry.path())) continue;
        std::cout<<++i<<": "<<entry.path().filename()<<'\n';
    }
    return i;
}

string getFolderFilePath(int id, string path = currentPath){
    int i = 0;
    for (const auto& entry : fs::directory_iterator(path)){
        if(!fs::is_regular_file(entry.path())) continue;
        if (++i == id){
            return entry.path().string();
        }
    }
    return "";
}

string getFile(){
    int mode = -1;

    do{
        cout<<"What mode do you want to use?"<<endl;
        cout<<"1: Path to file"<<endl;
        cout<<"2: Search for file in current directory"<<endl;
        cout<<"3: Path to folder and select file"<<endl;
        cout<<"Select: ";
        cin>>mode;
    } while(mode < 1 || mode > 3);

    string path;
    if(mode == 1){
        do{
            cout<<"Enter the path to the file: ";
            cin>>path;
        } while(!fs::exists(path) || !fs::is_regular_file(path));
    } else if(mode == 2){
        int max = printFolderContents();
        int id = -1;

        do{
            cout<<"Enter the file number: ";
            cin>>id;
        } while(id < 1 || id > max);

        path = getFolderFilePath(id);
    } else if(mode == 3){
        do{
            cout<<"Enter the path to the folder: ";
            cin>>path;
        } while(!fs::exists(path) || !fs::is_directory(path));

        int max = printFolderContents(path);
        int id = -1;
        do{
            cout<<"Enter the file number: ";
            cin>>id;
        } while(id < 1 || id > max);

        path = getFolderFilePath(id, path);
    }
    return path;
}

string getFileContent(string path){
    string content;
    ifstream file(path);
    if(file.is_open()){
        string line;
        while(getline(file, line)){
            content += line + "\n";
        }
        file.close();
    }
    return content;
}

int main(){
    int mode = modeSelector();

    string file = getFile();
 
    if(mode == 1){
        string content = getFileContent(file);
        
        vector<char> C;
        vector<int> F;
        for(int i = 0; i<256; i++){
            vector<int> kmp =  KMP(content, string(1, (char)i));
            if(kmp.size() > 0){
                C.push_back((char)i);
                F.push_back(kmp.size());
            }
        }

        vector<HuffmanCode> codes = Huffman(C, F);
        
        unordered_map<char, string> codeMap;
        for(const auto &hc : codes){
            codeMap[hc.c] = hc.code;
        }

        string bits;
        bits.reserve(content.size() * 2);
        for(char ch : content){
            auto it = codeMap.find(ch);
            if(it != codeMap.end()) bits += it->second;
        }

        vector<unsigned char> outBytes;
        unsigned char current = 0;
        int bitCount = 0;
        for(char b : bits){
            current = (current<<1) | (b == '1');
            bitCount++;
            if(bitCount == 8){
                outBytes.push_back(current);
                current = 0;
                bitCount = 0;
            }
        }
        int padding = 0;
        if(bitCount > 0){
            current <<= (8 - bitCount);
            outBytes.push_back(current);
            padding = 8 - bitCount;
        }

        fs::path inPath(file);
        string filename = inPath.filename().string();
        fs::path outPath = inPath.parent_path() / (filename + ".huff");
        fs::path tablePath = inPath.parent_path() / ("." + filename + ".huff");

        ofstream outFile(outPath, ios::binary);
        if(!outFile){
            cout<<"Failed to open output file: "<<outPath<<endl;
            return 1;
        }
        unsigned char padByte = static_cast<unsigned char>(padding);
        outFile.write(reinterpret_cast<char*>(&padByte), 1);
        if(!outBytes.empty()){
            outFile.write(reinterpret_cast<char*>(outBytes.data()), outBytes.size());
        }
        outFile.close();

        ofstream tableFile(tablePath, ios::binary);
        if(!tableFile){
            cout<<"Failed to open table file: "<<tablePath<<endl;
            return 1;
        }
        for(const auto &hc : codes){
            int ascii = static_cast<unsigned char>(hc.c);
            tableFile<<ascii<<' '<<hc.code<<'\n';
        }
        tableFile.close();

        cout<<"Compressed file written to: "<<outPath<<endl;
        cout<<"Code table written to: "<<tablePath<<endl;
    } else if(mode == 2){
            fs::path inPath(file);
            string compFilename = inPath.filename().string();
            string originalFilename = compFilename;
            if(compFilename.size() > 5 && compFilename.substr(compFilename.size() - 5) == ".huff"){
                originalFilename = compFilename.substr(0, compFilename.size() - 5);
            }

            fs::path tablePath = inPath.parent_path() / ("." + originalFilename + ".huff");
            fs::path outPath = inPath.parent_path() / originalFilename;

            if(!fs::exists(tablePath)){
                cout<<"Code table not found: "<<tablePath<<endl;
                return 1;
            }

            unordered_map<string, char> decodeMap;
            ifstream tableFile(tablePath);
            if(!tableFile){
                cout<<"Failed to open table file: "<<tablePath<<endl;
                return 1;
            }
            string line;
            while(getline(tableFile, line)){
                if(line.empty()) continue;
                istringstream iss(line);
                int ascii;
                string code;
                if(!(iss >> ascii >> code)) continue;
                decodeMap[code] = static_cast<char>(ascii);
            }
            tableFile.close();

            ifstream inFile(inPath, ios::binary);
            if(!inFile){
                cout<<"Failed to open compressed file: "<<inPath<<endl;
                return 1;
            }
            unsigned char padByte = 0;
            inFile.read(reinterpret_cast<char*>(&padByte), 1);
            if(!inFile){
                cout<<"Invalid compressed file: "<<inPath<<endl;
                return 1;
            }
            int padding = static_cast<int>(padByte);

            vector<unsigned char> dataBytes((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
            inFile.close();

            ofstream outFile(outPath, ios::binary);
            if(!outFile){
                cout<<"Failed to open output file: "<<outPath<<endl;
                return 1;
            }

            string curCode;
            for(size_t idx = 0; idx < dataBytes.size(); ++idx){
                unsigned char byte = dataBytes[idx];
                int bitsInThisByte = 8;
                if(idx + 1 == dataBytes.size() && padding > 0) bitsInThisByte = 8 - padding;
                for(int b = 7; b >= 8 - bitsInThisByte; --b){
                    char bitChar = ((byte >> b) & 1) ? '1' : '0';
                    curCode.push_back(bitChar);
                    auto it = decodeMap.find(curCode);
                    if(it != decodeMap.end()){
                        outFile.put(it->second);
                        curCode.clear();
                    }
                }
            }
            outFile.close();

            cout<<"Decompressed file written to: "<<outPath<<endl;
    }
}