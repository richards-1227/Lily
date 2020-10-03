//
//  main.cpp
//  Assembly
//
//  Created by Andrew Richards on 7/3/19.
//  Copyright © 2019 Andrew Richards. All rights reserved.
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
#include <cstdlib> // for exit()

std::vector<int> getIndex(int y, int z);

std::vector<int> getIndex(int y, int z){
    std::vector<int> q;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> die(0, (y-1));
    
    for (int i=0; i<z; ++i){
        q.push_back(die(gen));
    }
        
    return q;
}


int main(int argc, char *argv[]) {
 
    
    std::string str1a{};
    
    std::stringstream str1{argv[1]};
    str1 >> str1a;
    
    int noGenes{};
    std::stringstream option2{argv[2]};
    option2 >> noGenes;
    
    std::string geneInf{};
    std::stringstream option3{argv[3]};
    option3 >> geneInf;
    
    int firstInd{};
    std::stringstream option4{argv[4]};
    option4 >> firstInd;
    
    std::ifstream inf(str1a);
    std::string fileLine;
    std::vector<std::string> seqArray{};
    std::vector<std::string> nameVector{};
    // int recordInd{};
    std::string str3{";"};
    std::string str4{"matrix"};
    
    
    std::ofstream out4{};
    

      out4.open("infile_data3", std::ios::app);

    
    if (!inf)
    {
        std::cerr << "Uh oh, file1 could not be opened for reading!" << std::endl;
        exit(1);
    }
    
      int counterInf{};
    std::string str2a{};
    std::string str2b{};
    
    while(inf){
        getline(inf, fileLine);

        std::stringstream str2{fileLine};
        char testChar{fileLine[1]};
        
            if(testChar=='.'){
        str2 >> str2a;
        nameVector.push_back(str2a);
                if(counterInf>0){
                    seqArray.push_back(str2b);
                    str2b.clear();
                }
                ++counterInf;
            }
        if(testChar=='A' || testChar=='C' || testChar=='G' || testChar=='T' || testChar=='-'){
            std::string str2c{};
            str2 >> str2c;
            str2b.append(str2c);
        }
    }
    seqArray.push_back(str2b);

    for(int i=0; i< nameVector.size(); ++i){
        std::cout << nameVector[i] << " ";
    }
    std::cout << '\n';
    for(int i=0; i< nameVector.size(); ++i){
        std::cout << seqArray[i].length() << " ";
    }
    std::cout << '\n';
    
  /*  std::cout << seqArray.size() << " ";
    for(int i=0; i<seqArray.size(); ++i){
        std::cout << nameVector[i] << " " << seqArray[i].size() << '\n';
    } */
    
    std::ifstream inf2(geneInf);
     std::string fileLine2;
     std::vector<int> geneLenghts{};
    std::vector<int> geneStart{};
     std::vector<int> geneFinish{};
    geneStart.push_back(0);
    
     if (!inf2)
     {
         std::cerr << "Uh oh, file1 could not be opened for reading!" << std::endl;
         exit(1);
     }
     
     int counter{};
     
     while(inf2){
         getline(inf2, fileLine2);
         std::stringstream str3{fileLine2};
         if(counter<noGenes){
             int geneLength{};
             str3>> geneLength;
             geneLenghts.push_back(geneLength);
         }
         ++counter;
     }
    
    for (int i=0; i<geneLenghts.size(); ++i){
        geneFinish.push_back(geneStart[i]+geneLenghts[i]);
        if(geneStart.size()<geneLenghts.size()){
            geneStart.push_back(geneFinish[i]);
        }
    }

                      
std::vector<std::string> seqArray2{};
    
    if(firstInd==1)
    {
       for(int i=0; i<seqArray.size(); ++i){
          std::string r{seqArray[i]};
           seqArray2.push_back(r);
       }
    }
    else{
std::vector<int> geneIndex{getIndex(noGenes, noGenes)};
std::vector<int> siteIndex2{};
for(int gene=0; gene<noGenes; ++gene){
std::vector<int> siteIndex{getIndex(geneLenghts[geneIndex[gene]], geneLenghts[geneIndex[gene]])};
    for(int site=0; site<siteIndex.size(); ++site){
        siteIndex[site] += geneStart[geneIndex[gene]];
        siteIndex2.push_back(siteIndex[site]);
                                                         }
                                                     }
std::cout << siteIndex2.size() << '\n';
for(int i=0; i<seqArray.size(); ++i){
    std::string r{};
    for(int k=0; k<siteIndex2.size(); ++k){
       r.push_back(seqArray[i][siteIndex2[k]]);
                                                         }
    seqArray2.push_back(r);
                                                     }
    
    
    }
    
    out4 << "matrix"  << '\n';
for(int taxa=0; taxa<seqArray2.size(); ++taxa){
    out4 << nameVector[taxa] << " ";
    for(int site=0; site<seqArray2[taxa].size(); ++site){
        out4 << seqArray2[taxa][site];
    }
    out4 << '\n';
}
    out4 << ";" << '\n';
    
    out4.close();
    
    return 0;
}
