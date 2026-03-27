#include<iostream>
#include<vector>
#include<queue>
#include<sstream>
using namespace std;

void BFS_Visit(vector<vector<int>>& g, vector<int>& c, int s, queue<int>& q, vector<int>& delta, vector<int>& pi){
    q.push(s);
    c[s] = 1; // szary
    delta[s] = 0;
    pi[s] = -1;

    while(!q.empty()){
        int u = q.front();
        q.pop();

        for(int i{}; i<g.size(); i++){
            if((g[u][i] == 1) && (c[i] == 0)){
                c[i] = 1; // szary
                delta[i] = delta[u] + 1;
                pi[i] = u;
                q.push(i);
            }
        }
        c[u] = 2; // czarny
    }
}

void BFS(vector<vector<int>>& g, int s){
    cout<<"Macierz sasiedztwa:\n";
    for(int i{}; i<g.size(); i++){
        for(int j{}; j<g.size(); j++){
            cout<<g[i][j]<<" ";
        }
        cout<<endl;
    }
    
    // kolorowanie
    vector<int> c;
    for(int i{}; i<g.size(); i++){
        c.push_back(0);
    }

    queue<int> q; // kolejka piorytetowa
    vector<int> delta; // w ile przyszedl
    vector<int> pi; // skad przyszedl
    for(int i{}; i<g.size(); i++){
        delta.push_back(-1);
        pi.push_back(-1);
    }

    // pierw na startowym i potem reszta
    BFS_Visit(g, c, s, q, delta, pi);
    for(int i{}; i<g.size(); i++){
        if(c[i] == 0){ // jezeli bialy
            BFS_Visit(g, c, i, q, delta, pi);
        }
    }

    cout<<"Tablica delta i pi:\n";
    for(int i{}; i<g.size(); i++){
        cout<<"Wierzcholek "<<i+1<<": delta = "<<delta[i]<<", pi = "<<pi[i]+1<<endl;
    }
}

int main(){
    int n, c;
    cout<<"Podaj liczbe wierzcholkow:\n";
    cin>>n;
    string st;
    getline(cin, st);
    vector<int> line;
    vector<vector<int>> g;
    for(int i{}; i<n; i++){
        line.push_back(0);
    }
    for(int i{}; i<n; i++){
        g.push_back(line);
    }

    for(int i{}; i<n; i++){
        cout<<"Podaj wierzcholki sasiadujace z "<<i+1<<":\n";
        getline(cin, st);
        stringstream ss(st);

        while((ss>>c) && (c > 0)){
            g[i][c-1] = 1;
            g[c-1][i] = 1;
        }
    }

    cout<<"Podaj wierzcholek startowy:\n";
    int s;
    cin>>s;
    s = s - 1;

    BFS(g, s);

    return 0;
}