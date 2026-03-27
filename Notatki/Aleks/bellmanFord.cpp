#include<iostream>
#include<vector>
#include<sstream>
using namespace std;

vector<vector<int>> G;
vector<int> pi;
vector<int> d;
vector<bool> dB;


bool BW(vector<vector<int>>&G, int s){
	unsigned int n = G.size();
	for(unsigned int i{}; i<n; i++){
		pi.push_back(-1);
		d.push_back(-1);
		dB.push_back(false);
	}
	d[s] = 0;
	dB[s] = true;

	for(unsigned fre{}; fre<n-2; fre++){
		for(unsigned int i{}; i<n; i++){
			if(!dB[i]) continue;
			for(unsigned int j{}; j<n; j++){
				if(G[i][j] != 0){
					if(!dB[j]){
						d[j] = d[i] + G[i][j];
						pi[j] = i;
						dB[j] = true;
					} else{
						if(d[j] > d[i] + G[i][j]){
							d[j] = d[i] + G[i][j];
							pi[j] = i;
						}
					}
				}
			}
		}
	}

	cout<<endl<<" d[s]: ";
	for(unsigned int i{}; i<n; i++){
		cout<<d[i]<<", ";
	}
	cout<<endl<<"pi[s]: ";
	for(unsigned int i{}; i<n; i++){
		cout<<pi[i]+1<<", ";
	}
	cout<<endl<<endl;

	for(unsigned int i{}; i<n; i++){
		for(unsigned int j{}; j<n; j++){
			if(G[i][j] != 0){
				if(d[j] > d[i] + G[i][j]){
					return false;
				}
			}
		}
	}
	return true;
}

void pG(vector<vector<int>>&G){
	cout<<endl<<"Macierz sasiedztwa:"<<endl;
	for(unsigned int i{}; i<G.size(); i++){
		for(unsigned int j{}; j<G[i].size(); j++){
			cout<<G[i][j]<<" ";
		}
		cout<<endl;
	}

	return;
}

int main(){
	int n, c;
    cout<<"Podaj liczbe wierzcholkow:\n";
    cin>>n;
    string st, sw;
    getline(cin, st);
    vector<int> line;
    for(int i{}; i<n; i++){
        line.push_back(0);
    }
    for(int i{}; i<n; i++){
        G.push_back(line);
    }

    for(int i{}; i<n; i++){
        cout<<endl<<"Podaj wierzcholki krawedzie skierowane z "<<i+1<<":\n";
        getline(cin, st);
        stringstream ss(st);

		cout<<"Podaj odpowiednio wagi krawedzi:\n";
		getline(cin, sw);
		stringstream ssw(sw);

        while((ss>>c) && (c > 0)){
            ssw>>G[i][c-1];
        }
    }

    cout<<endl<<"Podaj wierzcholek startowy:\n";
    int s;
    cin>>s;
    s = s - 1;

	pG(G);
	cout<<BW(G, s)<<endl;

	return 0;
}
