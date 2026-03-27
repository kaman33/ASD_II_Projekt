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
    for(int i=0; i<n; i++){
        t.push_back(0);
    }
    vector<vector<int>> nmatrix;
    for(int i=0; i<n; i++){
        nmatrix.push_back(t);
    }
    
    while(file.good()){
        int a, b;
        file>>a>>b;
        nmatrix[a-1][b-1] = 1;
    }
    
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout<<nmatrix[i][j]<<" ";
        }
        cout<<endl;
    }
    

    file.close();
    return 0;
}