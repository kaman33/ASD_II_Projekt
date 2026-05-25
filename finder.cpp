#include "Struktury/Funkcje/KMP.h"
#include<iostream>
#include<filesystem>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;
namespace fs = std::filesystem;

string currentPath = fs::current_path().string();;

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

string getPattern(){
    cout<<"Enter the pattern to search for: ";
    string pattern;
    cin>>pattern;
    return pattern;
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

vector<string> breakString(string text, char separator){
    vector<string> lines;

    stringstream ss(text);
    string line;

    while(getline(ss, line)) {
        lines.push_back(line);
    }

    return lines;
}

void writePositions(vector<string> lines, string pattern){
    int lineNumber = 1;
    int totalFound = 0;
    for(const auto& eachLine : lines){
        vector<int> positions = KMP(eachLine, pattern);
        if(!positions.empty()){
            cout<<"Pattern found at line: "<<lineNumber<<" at positions: ";
            for(int pos : positions){
                cout<<pos<<" ";
                totalFound++;
            }
            cout<<endl;
        }
        lineNumber++;
    }

    if(totalFound == 0){
        cout<<"No pattern found in the file."<<endl;
    } else{
        cout<<"Total occurrences found: "<<totalFound<<endl;
    }
}

int main(){
    string file = getFile();
    if(file.empty()){
        cout<<"Error in getting file"<<endl;
        return -1;
    }

    string pattern = getPattern();
    if(pattern.empty()){
        cout<<"Error in getting pattern"<<endl;
        return -1;
    }

    string fileContent = getFileContent(file);
    vector<string> lines = breakString(fileContent, '\n');

    writePositions(lines, pattern);

    return 0;
}