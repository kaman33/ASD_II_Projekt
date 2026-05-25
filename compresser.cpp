#include "Struktury/Huffman.h"
#include "Struktury/Funkcje/KMP.h"
#include<iostream>
#include<filesystem>
#include<string>
#include<fstream>
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
        if (++i == id) {
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
        
        // dalej trzeba przepisac caly plik uzywajac kodow do innego pliku oraz zapisac tabele kodow do odkodowania pliku       
    } else if(mode == 2){
        // dekompresja
    }
}