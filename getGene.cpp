//
//  main.cpp
//  getGenes
//
//  Created by Andrew Richards on 7/3/20.
//  Copyright © 2020 Andrew Richards. All rights reserved.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <random>
#include <algorithm>
#include <cstdlib>

int main(int argc, const char * argv[]) {

    int noTaxa{};
    std::stringstream option1{argv[1]};
    option1 >> noTaxa;
    
    int noGenes{};
    std::stringstream option2{argv[2]};
    option2 >> noGenes;
    
    int sitesPer{};
    std::stringstream option3{argv[3]};
    option3 >> sitesPer;
    
    std::ifstream inf("infile_data2");
    std::string fileLine;
    std::vector<std::string> seqArray{};
    std::vector<std::string> nameVector{};
    int recordInd{};
    std::string str3{";"};
    std::string str4{"MATRIX"};
    
    if (!inf)
    {
        std::cerr << "Uh oh, file1 could not be opened for reading!" << std::endl;
        exit(1);
    }
    
    while(inf){
        getline(inf, fileLine);
        std::string str2a{};
        std::string str2b{};
        std::stringstream str2{fileLine};
        str2 >> str2a;
        str2 >> str2b;
        if(str2a.compare(str3)==0){
            recordInd=0;
        }
        if(recordInd==1){
            nameVector.push_back(str2a);
            seqArray.push_back(str2b);
        }
        if(str2a.compare(str4)==0){
            recordInd=1;
        }
    }
    
    std::cout << seqArray.size() << " ";
    for(int i=0; i<seqArray.size(); ++i){
        std::cout << nameVector[i] << " " << seqArray[i].size() << '\n';
    }
      std::ofstream out3{};
    for(int i=0; i<noGenes; ++i){
        std::string str0{"Gene"};
        int symbol1 {i/100};
        int symbol2 {(i % 100)/10};
        int symbol3 {i % 10};
                str0.push_back(symbol1+48);
        str0.push_back(symbol2+48);
        str0.push_back(symbol3+48);
    //    std::cout << str0 << '\n';
         out3.open(str0);
        out3 << noTaxa << " " << sitesPer << '\n';
        for(int j=0; j<noTaxa; ++j){
        out3 << j << " ";
        for(int k=0; k<sitesPer; ++k){
            out3 << seqArray[j][i*sitesPer+k];
        }
                   out3 << '\n';
        }
        
        out3.close();
    }
    
    return 0;
}
