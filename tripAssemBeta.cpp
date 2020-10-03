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
#include "/usr/local/Cellar/tbb/2019_U9/include/tbb/parallel_for.h"
#include "/usr/local/Cellar/tbb/2019_U9/include/tbb/task_scheduler_init.h"

std::vector<double> finalProb(std::vector<double> x);
std::vector<double> getPostLik(std::vector<double> probs, std::vector<int> countVec);
std::vector<int> getCounts(std::vector<std::string>);
double mean(std::vector<double> x);
std::vector<double> probVec(std::vector<double> x);
std::vector<double> getTrueProb(double myt1,double myt2, double theta,double alpha);
std::vector<double> postProb(double lambda, double theta, double alpha);
double getUnif(double y, double z);
std::vector<double> averageProbs(std::vector<std::vector<double>> x);
double mean(std::vector<double> x);
void Assemble(std::vector<int> a, std::vector<std::vector<double>> b, std::vector<std::vector<double>> c, std::ostream &out3);
std::vector< std::vector<int> > getAllSubsets(std::vector<int> set);
std::vector<int> pairCounts (std::string c1, std::string c2);

std::vector<int> pairCounts (std::string c1, std::string c2){
    
    std::vector<int> z{0,0};
    
    char t1{'A'};
    char t2{'C'};
    char t3{'G'};
    char t4{'T'};
    
    for (int i=0; i<c1.length(); ++i){
        char s1{c1[i]};
        char s2{c2[i]};
        
        if((s1==t1 || s1==t2 || s1==t3 || s1==t4) && (s2==t1 || s2==t2 || s2==t3 || s2==t4) ) {
            if(s1==s2){
                ++z[0];
            }
            else{
                ++z[1];
            }
        }
    }
    return z;
}

std::vector< std::vector<int> > getAllSubsets(std::vector<int> set)
{
    std::vector< std::vector<int> > subset;
    std::vector<int> empty;
    subset.push_back( empty );
    
    for (int i = 0; i < set.size(); i++)
    {
        std::vector< std::vector<int> > subsetTemp = subset;
        
        for (int j = 0; j < subsetTemp.size(); j++)
            subsetTemp[j].push_back( set[i] );
        
        for (int j = 0; j < subsetTemp.size(); j++)
            subset.push_back( subsetTemp[j] );
    }
    return subset;
}

void Assemble(std::vector<int> a, std::vector<std::vector<double>> b, std::vector<std::vector<double>> c, std::ostream &out3){
  
    if (a.size()==1){
            out3 <<  a[0];
    }
    else {
        if (a.size()==2){
                out3 << "(" << a[0] << "," << a[1] << ")";
        }
        else{
                    int max{};
            int evenTaxa{};
                max = static_cast<int>(a.size())/2;
            if(static_cast<int>(a.size()) % 2 ==0){
                evenTaxa=1;
            }
            
            std::vector< std::vector<int> > d{getAllSubsets(a)};
            std::vector<int> e{0};
            std::vector<int> g{a};
            double splitScore{};
            
            for (int i=0; i<d.size(); ++i){
                std::sort(a.begin(), a.end());
                std::sort(d[i].begin(), d[i].end());
            if ((d[i].size()>0) && ((d[i].size() < max) || (d[i].size() == max && evenTaxa==0)   || (d[i].size() == max && d[i][0]==a[0])) ) {
                    std::vector<int> f{};
                    std::set_difference(a.begin(), a.end(), d[i].begin(),  d[i].end(), std::inserter(f, f.end()));
                    double num{};
                    double den{};
                    for (int j=0; j<d[i].size(); ++j){
                        for (int k=0; k<f.size(); ++k){
                            num += b[(d[i][j])][(f[k])];
                            den += c[(d[i][j])][(f[k])];
                        }
                    }
                    if((num/den)>splitScore){
                        e=d[i];
                        splitScore=(num/den);
                        g=f;
                    }
                }
            }
            out3 << "(";
            Assemble(e, b, c, out3);
            out3 << ",";
            Assemble(g, b, c, out3);
            out3 << ")";
    }
    
}
}

std::vector<double> averageProbs(std::vector<std::vector<double>> x){
    std::vector<double> y{};
    for(int i=0; i<x.size(); ++i){
        y.push_back(0);
    }
    for(int i=0; i<x.size(); ++i){
        double maxValue {x[i][0]};
        for(int j=0; j<x[i].size(); ++j){
            if(x[i][j]>maxValue){
                maxValue=x[i][j];
            }
        }
        double sumValue{};
        for(int j=0; j<x[i].size(); ++j){
            sumValue += exp(x[i][j]-maxValue);
        }
        y[i] = exp(maxValue)*sumValue;
    }
    return y;
}

double getUnif(double y, double z){
    std::random_device rd;
    std::mt19937 gen(rd());
    double h{};
    std::uniform_real_distribution<double> q(y, z);
    h=(q(gen));
    return h;
}

std::vector<double> finalProb(std::vector<double> x){
    std::vector<double> y{0,0,0};
    
        y[0] = 1/(1+exp(x[1]-x[0])+exp(x[2]-x[0]));
        y[1] = 1/(1+exp(x[0]-x[1])+exp(x[2]-x[1]));
        y[2] = 1/(1+exp(x[0]-x[2])+exp(x[1]-x[2]));
    
    return y;
}

double mean(std::vector<double> x){
    double z{};
    for(int i=0; i<x.size(); ++i){
        z+=x[i];
    }
    z /= x.size();
    
    return z;
}

std::vector<int> getCounts(std::vector<std::string> x){
    std::vector<int> q{0,0,0,0,0};
    char t1{'A'};
    char t2{'C'};
    char t3{'G'};
    char t4{'T'};
    for (int i=0; i<x[0].length(); ++i){
        char c1{x[0][i]};
        char c2{x[1][i]};
        char c3{x[2][i]};
        if((c1==t1 || c1==t2 || c1==t3 || c1==t4) && (c2==t1 || c2==t2 || c2==t3 || c2==t4)
           && (c3==t1 || c3==t2 || c3==t3 || c3==t4) ){
            if(c1==c2 && c1==c3){
                q[0] += 1;
            }
            if(c1 != c2 && c2==c3){
                q[1] += 1;
            }
            if(c1 != c2 && c1==c3){
                q[2] += 1;
            }
            if(c1==c2 && c1 != c3){
                q[3] += 1;
            }
            if(c1 != c2 && c1 != c3  && c2 != c3){
                q[4] += 1;
            }
        }
    }
    
    return q;
}

std::vector<double> getPostLik(std::vector<double> probs, std::vector<int> countVec){

    std::vector<double> outLik{0,0,0};
      
    double permMat[3][5] {
        {0,1,2,3,4},
        {0,2,3,1,4},
        {0,3,1,2,4}
    };
    
    for(int i=0; i<3; ++i){
    for(int j=0; j<5; ++j){
        outLik[i] += countVec[permMat[i][j]]*log(probs[j]);
    }
    }
    
    return outLik;
}


std::vector<double> getTrueProb(double myt1,double myt2, double theta,double alpha){
    
    double t1  {myt1*theta};  // t1*theta
    double t2 {myt2*theta};  // t2*theta
    double t  {2*theta};
    double m {alpha};
    
    double g[4] {1,exp(-2*m*t1),exp(-2*m*t2), exp(-m*t1)*exp(-2*m*t2)};
    double f[5][4];
    f[0][0] = f[1][0] = f[2][0] = f[3][0] = f[4][0] = 1.0/64;
    
    f[0][1] = f[1][1] = 3.0/(64.0*(1.0+m*t));
    f[2][1] = f[3][1] = f[4][1] = -1.0/(64.0*(1.0+m*t));
    
    f[0][2] = 6.0/(64.0*(1.0+m*t));
    f[1][2] =  f[4][2] =  -2.0/(64.0*(1.0+m*t));
    f[2][2] =  f[3][2] =  2.0/(64.0*(1.0+m*t));
    
    f[0][3] = 12.0/(64.0*(1.0+m*t)*(2.0+m*t));
    f[1][3] = f[2][3] = f[3][3] = -4.0/(64.0*(1.0+m*t)*(2.0+m*t));
    f[4][3] = 4.0/(64.0*(1.0+m*t)*(2.0+m*t));
   
    std::vector<double> h {0,0,0,0,0};
    
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            h[i] += f[i][j]  * g[j];
        }
    }
    
    return h;
}

std::vector<double> postProb(double lambda, double theta, double alpha){
    
    double m {alpha};
    double g[4] {
        1.0/64.0,
        (lambda/64.0)*log((lambda+2.0*m*theta)/lambda)/((1.0+2.0*m*theta)*(2.0*m*theta)),
        (lambda/64.0)/((1.0+2.0*m*theta)*(lambda+2.0*m*theta)),
        (lambda/64.0)*log((lambda+3.0*m*theta)/(lambda+2.0*m*theta))/
          ((1.0+2.0*m*theta)*(2.0+2.0*m*theta)*m*theta)
    };
    double f[5][4];
    
    f[0][0] = f[1][0] = f[2][0] = f[3][0] = f[4][0] = 1.0;
    
    f[0][1] = f[1][1] = 3.0;
    f[2][1] = f[3][1] = f[4][1] = -1.0;
    
    f[0][2] = 6.0;
    f[1][2] = f[4][2] = -2.0;
    f[2][2] = f[3][2] = 2.0;
    
    f[0][3] = 12.0;
    f[1][3] = f[2][3] = f[3][3] = -4.0;
    f[4][3] = 4.0;
    
    std::vector<double> h {0,0,0,0,0};
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            h[i] += f[i][j] * g[j];
        }
    }

    return h;
}

int main(int argc, char *argv[]) {
 
    std::string str1a{};
    
    std::stringstream str1{argv[1]};
    str1 >> str1a;
    
    double thetaHat{};
    std::stringstream input2{argv[2]};
    input2 >> thetaHat;
    
    std::ifstream inf(str1a);
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
    

    
    std::vector<std::vector<double>> goodMat{};
    std::vector<std::vector<double>> badMat{};
    
    std::vector<double> zeroVec{};
    for (int i=0; i<seqArray.size(); ++i){
        zeroVec.push_back(0);
    }
    
    for (int i=0; i<seqArray.size(); ++i){
        goodMat.push_back(zeroVec);
        badMat.push_back(zeroVec);
    }
    
   
    tbb::parallel_for( tbb::blocked_range<int>(0,seqArray.size()-2),
                      [&](tbb::blocked_range<int> u)
                      {
                          for (int species1=u.begin(); species1<u.end(); ++species1){
        tbb::parallel_for( tbb::blocked_range<int>(species1+1,seqArray.size()-1),
                          [&](tbb::blocked_range<int> t)
                          {
                              for (int species2=t.begin(); species2<t.end(); ++species2){
               tbb::parallel_for( tbb::blocked_range<int>(species2+1,seqArray.size()),
            [&](tbb::blocked_range<int> s)
            {
                for (int species3=s.begin(); species3<s.end(); ++species3){


        std::vector<std::string> seq2{seqArray[species1], seqArray[species2], seqArray[species3]};
        std::vector<int> countArray{getCounts(seq2)};

                    double rootAge{};
                    
                    std::vector<int> pair1{pairCounts(seqArray[species1], seqArray[species2])};
                    std::vector<int> pair2{pairCounts(seqArray[species1], seqArray[species3])};
                    std::vector<int> pair3{pairCounts(seqArray[species2], seqArray[species3])};
                    
                    double phat1 {(pair1[1]+0.0)/(pair1[1]+pair1[0])};
                    double tau1 {-6.0*log(1.0-4.0*phat1/3)/0.012};
                    
                    double phat2 {(pair2[1]+0.0)/(pair2[1]+pair2[0])};
                    double tau2 {-6.0*log(1.0-4.0*phat2/3)/0.012};
                    
                    double phat3 {(pair3[1]+0.0)/(pair3[1]+pair3[0])};
                    double tau3 {-6.0*log(1.0-4.0*phat3/3)/0.012};
                    
                    rootAge = (tau1+tau2+tau3)/3.0;
                    
           std::vector<double> posteriorProbVec{postProb(1.0/rootAge, thetaHat, 4.0/3)};
                   
                   std::vector<double> posteriors{getPostLik(posteriorProbVec, countArray)};
                   
                   std::vector<double> finalProbVec{finalProb(posteriors)};
                   
                               goodMat[species1][species2] += finalProbVec[0];
                               goodMat[species1][species3] += finalProbVec[0];
                               badMat[species2][species3] += finalProbVec[0];
                               goodMat[species1][species2] += finalProbVec[1];
                               goodMat[species2][species3] += finalProbVec[1];
                               badMat[species1][species3] += finalProbVec[1];
                               goodMat[species2][species3] += finalProbVec[2];
                               goodMat[species1][species3] += finalProbVec[2];
                               badMat[species1][species2] += finalProbVec[2];
                   
        std::cout << species1 << " " << species2 << " " << species3 << " " << rootAge << '\n';
                   
                    }
                                   });
                                       }
                                       });
                                                     }
                                                     });


    
    for (int i=1; i<seqArray.size(); ++i){
        for(int j=0; j<i; ++j){
            goodMat[i][j]=goodMat[j][i];
            badMat[i][j]=badMat[j][i];
        }
    }
  /*  std::cout << '\n';
    for(int i=0; i<7; ++i){
        for(int j=0; j<7; ++j){
            std::cout << goodMat[i][j] << " ";
        }
        std::cout << '\n';
    }
    for(int i=0; i<7; ++i){
        for(int j=0; j<7; ++j){
            std::cout << badMat[i][j] << " ";
        }
        std::cout << '\n';
    }
     */
   
    
    std::ofstream out3{};
    

      out3.open("output1", std::ios::app);
    
    std::vector<int> speciesIdentifier{};
    for(int i=0; i<seqArray.size(); ++i){
        speciesIdentifier.push_back(i);
    }
    
    Assemble(speciesIdentifier, goodMat,  badMat, out3);
    out3 << '\n';
  
    
    out3.close();
    
    return 0;
}
