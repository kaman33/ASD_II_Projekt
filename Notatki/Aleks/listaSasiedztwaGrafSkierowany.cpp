#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

int main(){
    int n;
    ifstream file("./dane.txt");
    if(file.is_open()){
        file>>n;
    }

    vector<int> t;
    vector<vector<int>> nmatrix;
    for(int i=0; i<n; i++){
        nmatrix.push_back(t);
    }

    while(file.good()){
        int a, b;
        file>>a>>b;
        nmatrix[a-1].push_back(b);
    }
    
    for(int i=0; i<n; i++){
        cout<<i+1<<": ";
        for(unsigned int j=0; j<nmatrix[i].size(); j++){
            cout<<nmatrix[i][j]<<" ";
        }
        cout<<endl;
    }

    file.close();
    return 0;
}