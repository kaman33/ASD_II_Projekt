#include "KMP.h"

std::vector<int> buildPi(std::string P){
    std::vector<int> pi;
    //unsigned int m = P.size();
    for(unsigned int i{}; i<P.size(); i++){
        pi.push_back(0);
    }

    unsigned int k = 0;
    for(unsigned int q = 1; q<P.size(); q++){
        while((k > 0) && P[k] != P[q]){
            k = pi[k-1];
        }
        if(P[k] == P[q]){
            k++;
        }
        pi[q] = k;
    }

    return pi;
}

std::vector<int> KMP(std::string T, std::string P){
    std::vector<int> pi = buildPi(P);
    unsigned int q = 0;
    unsigned int n = T.size();
    unsigned int m = P.size();
    std::vector<int> shifts = {};

    for(unsigned int i{}; i<n; i++){
        while((q > 0) && (P[q] != T[i])){
            q = pi[q-1];
        }
        if(P[q] == T[i]){
            q++;
        }
        if(q == m){
            shifts.push_back(i - m + 1);
            q = pi[q-1];
        }
    }
    return shifts;
}