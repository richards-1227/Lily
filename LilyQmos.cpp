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

struct outputSum{
    int ID1;
    int ID2;
    int ID3;
    int ID4;
    double prob1;
    double prob2;
    double prob3;
};

std::vector<long double> probAsymmPost(double lambda, double theta, double alpha);
std::vector<double> getPostLik(double lambda, double theta, double alpha, std::vector<int> countVec);
std::vector<long double> postSymProb(double lambda, double theta,double alpha);
std::vector<double> probVec(std::vector<double> x);
std::vector<int> generateCounts (std::vector<std::string> s);
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


std::vector<int> generateCounts (std::vector<std::string> s){
std::vector<int> counts{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    char t1{'A'};
    char t2{'C'};
    char t3{'G'};
    char t4{'T'};
    for (int i=0; i<s[0].length(); ++i){
        char c1{s[0][i]};
        char c2{s[1][i]};
        char c3{s[2][i]};
        char c4{s[3][i]};
if((c1==t1 || c1==t2 || c1==t3 || c1==t4) && (c2==t1 || c2==t2 || c2==t3 || c2==t4)
    && (c3==t1 || c3==t2 || c3==t3 || c3==t4) && (c4==t1 || c4==t2 || c4==t3 || c4==t4)){
        if (c1 == c2 && c1 == c3 && c1 == c4 )
            counts[0] += 1;
        if (c1 == c2 && c1 == c3 && c1 != c4 )
            counts[1] += 1;
        if (c1 == c2 && c1 != c3 && c1 == c4 )
            counts[2] += 1;
        if (c1 == c2 && c1 != c3 && c3 == c4 )
            counts[3] += 1;
        if (c1 == c2 && c1 != c3 && c3 != c4 && c1 != c4)
            counts[4] += 1;
        if (c1 != c2 && c1 == c3 && c3 == c4 )
            counts[5] += 1;
        if (c1 == c3 && c1 != c2 && c2 == c4 )
            counts[6] += 1;
        if (c1 == c3 && c1 != c2 && c2 != c4  && c1 != c4)
            counts[7] += 1;
        if (c1 ==c4 && c1 != c2 && c2 == c3 )
            counts[8] += 1;
        if (c2 == c3 && c2 == c4 && c1 != c2 )
            counts[9] += 1;
        if (c2 == c3 && c1 != c2 && c2 != c4 && c1 != c4)
            counts[10] += 1;
        if (c1 == c4 && c1 != c2 && c1 != c3 && c2 != c3)
            counts[11] += 1;
        if (c2 == c4 && c1 != c2 && c2 != c3 && c1 != c3)
            counts[12] += 1;
        if (c4 == c3 && c1 != c2 && c2 != c4 && c1 != c4)
            counts[13] += 1;
        if (c1 != c2 && c1 != c3 && c1 != c4 && c2 != c3 && c2 != c4 && c3 != c4)
            counts[14] += 1;
    }
    }
    return counts;
}

std::vector<double> probVec(std::vector<double> x){
    
    std::vector<double> y{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    double maxLogLik{x[0]};
    for (int i=1; i<15; ++i){
        if (x[i] < maxLogLik)
            maxLogLik=x[i];
    }
    
    for (int i=0; i<5; i+=2){
        y[i]=2.0*exp(maxLogLik- x[i])/(2.0*exp(maxLogLik-x[0])+
                                         exp(maxLogLik - x[1])+2.0*exp(maxLogLik - x[2])+
                                         exp(maxLogLik - x[3])+2.0*exp(maxLogLik - x[4])+
                                         exp(maxLogLik - x[5])+exp(maxLogLik - x[6])
                                         +exp(maxLogLik - x[7])+exp(maxLogLik - x[8])+
                                         exp(maxLogLik - x[9])+exp(maxLogLik - x[10])
                                         +exp(maxLogLik - x[11])+
                                         exp(maxLogLik - x[12])+exp(maxLogLik - x[13])+
                                         exp(maxLogLik - x[14]));
    }
    
    for (int i=1; i<6; i+=2){
        y[i]=exp(maxLogLik- x[i])/(2.0*exp(maxLogLik-x[0])+
                                       exp(maxLogLik - x[1])+2.0*exp(maxLogLik - x[2])+
                                       exp(maxLogLik - x[3])+2.0*exp(maxLogLik - x[4])+
                                       exp(maxLogLik - x[5])+exp(maxLogLik - x[6])
                                       +exp(maxLogLik - x[7])+exp(maxLogLik - x[8])+
                                       exp(maxLogLik - x[9])+exp(maxLogLik - x[10])
                                       +exp(maxLogLik - x[11])+
                                       exp(maxLogLik - x[12])+exp(maxLogLik - x[13])+
                                       exp(maxLogLik - x[14]));
    }
    
    for (int i=6; i<15; ++i){
        y[i]=exp(maxLogLik- x[i])/(2.0*exp(maxLogLik-x[0])+
                                   exp(maxLogLik - x[1])+2.0*exp(maxLogLik - x[2])+
                                   exp(maxLogLik - x[3])+2.0*exp(maxLogLik - x[4])+
                                   exp(maxLogLik - x[5])+exp(maxLogLik - x[6])
                                   +exp(maxLogLik - x[7])+exp(maxLogLik - x[8])+
                                   exp(maxLogLik - x[9])+exp(maxLogLik - x[10])
                                   +exp(maxLogLik - x[11])+
                                   exp(maxLogLik - x[12])+exp(maxLogLik - x[13])+
                                   exp(maxLogLik - x[14]));
    }
    return y;
}

std::vector<long double> postSymProb(double lambda, double theta,double alpha){
    
    long double t  {2*theta};
    long double m {alpha};
    
    long double f[9][9] {};

    
    //row 1
    for (int i = 0; i < 9; i++){
        f[0][i]=1.0/256;
    }
    
    //row 2
    f[1][0]=f[1][2]=f[1][4]=f[1][6] = 3.0/(256.0*(1.0+m*t));
    f[1][1]=f[1][3]=f[1][5]=f[1][7]=f[1][8]=-1.0/(256*(1.0+m*t));
    
    // row 3
    f[2][0]=f[2][1]=f[2][4]=f[2][5] = 3/(256*(1+m*t));
    f[2][2]=f[2][3]=f[2][6]=f[2][7]=f[2][8]=-1/(256*(1+m*t));
    
    //row 4
    f[3][0]=f[3][4]=9/(256*pow((1+m*t),2));
    f[3][1]=f[3][2]=f[3][5]=f[3][6]=-3/(256*pow((1+m*t),2));
    f[3][3]=f[3][7]=f[3][8]=1/(256*pow((1+m*t),2));
    
    //row 5
    f[4][0]=12/(256*pow((1+m*t),1));
    f[4][1]=f[4][2]=f[4][3]=4/(256*pow((1+m*t),1));
    f[4][4]=f[4][5]=f[4][6]=f[4][8]=-4/(256*pow((1+m*t),1));
    f[4][7]=0;
    
    //row 6
    f[5][0]=24/(256*(1+m*t)*(2+m*t));
    f[5][1]=f[5][3]=f[5][4]=f[5][6]=-8/(256*(1+m*t)*(2+m*t));
    f[5][5]=f[5][8]=f[5][2]=8/(256*(1+m*t)*(2+m*t));
    f[5][7]=0;
    
    //row 7
    f[6][0]=24/(256*(1+m*t)*(2+m*t));
    f[6][1]=f[6][6]=f[6][8]=8/(256*(1+m*t)*(2+m*t));
    f[6][2]=f[6][3]=f[6][4]=f[6][5]=-8/(256*(1+m*t)*(2+m*t));
    f[6][7]=0;
    
    // row 8
    f[7][0]=48/(256*pow((1+m*t),1)*pow((2+m*t),2));
    f[7][1]=f[7][2]=f[7][4]=f[7][8]=-16/(256*pow((1+m*t),1)*pow((2+m*t),2));
    f[7][3]=f[7][5]=f[7][6]=16/(256*pow((1+m*t),1)*pow((2+m*t),2));
    f[7][7]=0;
    
    
    // row 9
    f[8][0]=6*m*t*(4+m*t)*(4+3*m*t)/(256*pow((1+m*t),2)*pow((2+m*t),2)*(3+m*t));
    f[8][1]=f[8][2]=-2*m*t*(4+m*t)*(4+3*m*t)
    /(256*pow((1+m*t),2)*pow((2+m*t),2)*(3+m*t));
    f[8][3]=m*t*(32+40*m*t+10*pow(m,2)*pow(t,2))
    /(256*pow((1+m*t),2)*pow((2+m*t),2)*(3+m*t));
    f[8][4]=2*m*t*pow(4+m*t,2)/(256*pow((1+m*t),2)*pow((2+m*t),2)*(3+m*t));
    f[8][5]=f[8][6]=2*(4+m*t)*pow(m,2)*pow(t,2)
    /(256*pow((1+m*t),2)*pow((2+m*t),2)*(3+m*t));
    f[8][7]=-(4+2*m*t)*pow(m,2)*pow(t,2)/(256*pow((1+m*t),2)*pow((2+m*t),2)*(3+m*t));
    f[8][8]=2*pow(m,3)*pow(t,3)/(256*pow((1+m*t),2)*pow((2+m*t),2)*(3+m*t));
    
    long double g[9]  {
        1,
        (log(2.0*m*theta+lambda)-log(lambda))/(2.0*m*theta),
        (log(2.0*m*theta+lambda)-log(lambda))/(2.0*m*theta),
        (lambda*log(lambda)-2.0*(2.0*m*theta+lambda)*log(2.0*m*theta+lambda)+
           (4.0*m*theta+lambda)*log(4.0*m*theta+lambda))/(2.0*m*theta*2.0*m*theta),
        1.0/(2.0*m*theta+lambda),
        log((3.0*m*theta+lambda)/(2.0*m*theta+lambda))/(m*theta),
        (log(3.0*m*theta+lambda)-log(2.0*m*theta+lambda))/(m*theta),
((2.0*m*theta+lambda)*log(2.0*m*theta+lambda)-2.0*(3.0*m*theta+lambda)*log(3.0*m*theta+lambda)+
           (4.0*m*theta+lambda)*log(4.0*m*theta+lambda))/(m*theta*m*theta),
        (lambda+2.0+4.0*m*theta)*log(lambda+2.0+4.0*m*theta)
        -2.0*(lambda+1.0+4.0*m*theta)*log(lambda+1.0+4.0*m*theta)
        +(lambda+4.0*m*theta)*log(lambda+4.0*m*theta)
    };
    std::vector<long double> h {0,0,0,0,0,0,0,0,0};
    
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++){
                      if(j !=0){
                h[i] += f[j][i] *lambda*g[j];
            }
            else{
                h[i] += 1.0/256;
            }
        }
    }
    
  /*  for(int i=0; i<9; ++i){
        std::cout << h[i] << " ";
    }
    std::cout << '\n';*/
    
    return h;
}

std::vector<double> getPostLik(double lambda, double theta, double alpha, std::vector<int> countVec){
    std::vector<double> notNormalized{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    int permMat[15][15] {
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
        {1,2,6,7,8,3,4,5,9,10,11,12,14,13,15},
        {1,2,6,7,8,3,4,5,9,10,11,12,14,13,15},
        {1,3,6,9,12,2,4,5,7,10,13,8,14,11,15},
        {1,3,6,9,12,2,4,5,7,10,13,8,14,11,15},
        {1,2,3,4,5,10,9,11,7,6,8,13,12,14,15},
        {1,2,10,9,11,3,4,5,7,6,8,13,14,12,15},
        {1,3,10,7,13,2,4,5,9,6,12,11,14,8,15},
        {1,2,6,7,8,10,9,11,4,3,5,14,12,13,15},
        {1,2,10,9,11,6,7,8,4,3,5,14,13,12,15},
        {1,6,10,4,14,2,7,8,9,3,12,11,13,5,15},
        {1,3,6,9,12,10,7,13,4,2,5,14,8,11,15},
        {1,3,10,7,13,6,9,12,4,2,5,14,11,8,15},
        {1,6,10,4,14,3,9,12,7,2,8,13,11,5,15}
    };
    
    std::vector<long double> probs{postSymProb(lambda, theta, alpha)};
    long double probs2[15]{
        probs[0]*4,
        probs[1]*12,
        probs[1]*12,
        probs[4]*12,
        probs[5]*24,
        probs[2]*12,
        probs[3]*12,
        probs[7]*24,
        probs[3]*12,
        probs[2]*12,
        probs[7]*24,
        probs[7]*24,
        probs[7]*24,
        probs[6]*24,
        probs[8]*24
    };
     std::vector<long double> probs3{probAsymmPost(lambda, theta, alpha)};
    long double probs4[15]{
        probs3[0]*4,
        probs3[1]*12,
        probs3[1]*12,
        probs3[4]*12,
        probs3[6]*24,
        probs3[2]*12,
        probs3[5]*12,
        probs3[8]*24,
        probs3[5]*12,
        probs3[3]*12,
        probs3[9]*24,
        probs3[8]*24,
        probs3[9]*24,
        probs3[7]*24,
        probs3[10]*24
    };
    
 /*   for(int i=0; i<15; ++i){
        std::cout << probs2[i] << " ";
    }
    std::cout << '\n';
    for(int i=0; i<15; ++i){
        std::cout << probs4[i] << " ";
    }
    std::cout << '\n';
  */
    
    
    for(int m=0; m<5; m +=2) {
        double logLik{};
        for(int i=0; i<15; ++i){
            logLik -= countVec[permMat[m][i]-1]*log(probs2[i]);
        }
        notNormalized[m]=logLik;
    }
    for(int m=1; m<6; m +=2) {
        double logLik{};
        for(int i=0; i<15; ++i){
            logLik -= countVec[permMat[m][i]-1]*log(probs4[i]);
        }
        notNormalized[m]=logLik;
    }
    for(int m=6; m<15; ++m) {
        double logLik{};
        for(int i=0; i<15; ++i){
            logLik -= countVec[permMat[m][i]-1]*log(probs4[i]);
        }
        notNormalized[m]=logLik;
    }
    
    return notNormalized;
}

std::vector<long double> probAsymmPost(double lambda, double theta, double alpha){
    
   
    long double t  {2*theta};
    long double m {alpha};
    
    long double f[11][10] {};

    
    //row 1
    f[0][0]=1.0/256;
    f[0][1]=3/(256*(1.0+m*t));
    f[0][2]=6/(256*(1.0+m*t));
    f[0][3]=f[0][5]=12/(256*(1.0+m*t)*(2.0+m*t));
    f[0][4]=9/(256*(1.0+m*t));
    f[0][6]=9/(256*pow((1.0+m*t),2));
    f[0][7]=24/(256*(1.0+m*t)*(2.0+m*t));
    f[0][8]=48/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[0][9]=(6.0*m*t*(4.0+m*t)*(4+3.0*m*t))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 2
    f[1][0]=1.0/256;
    f[1][1]=-1/(256*(1.0+m*t));
    f[1][2]=2/(256*(1.0+m*t));
    f[1][3]=f[1][5]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[1][4]=5/(256*(1.0+m*t));
    f[1][6]=-3/(256*pow((1.0+m*t),2));
    f[1][7]=8/(256*(1.0+m*t)*(2.0+m*t));
    f[1][8]=-16/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[1][9]=(-2.0*m*t*(4.0+m*t)*(4+3.0*m*t))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 3
    f[2][0]=1.0/256;
    f[2][1]=3/(256*(1.0+m*t));
    f[2][2]=-2/(256*(1.0+m*t));
    f[2][3]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[2][4]=5/(256*(1.0+m*t));
    f[2][5]=12/(256*(1.0+m*t)*(2.0+m*t));
    f[2][6]=-3/(256*pow((1.0+m*t),2));
    f[2][7]=-8/(256*(1.0+m*t)*(2.0+m*t));
    f[2][8]=-16/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[2][9]=(-2.0*m*t*(4.0+m*t)*(4+3.0*m*t))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 4
    f[3][0]=1.0/256;
    f[3][1]=3/(256*(1.0+m*t));
    f[3][2]=6/(256*(1.0+m*t));
    f[3][3]=12/(256*(1.0+m*t)*(2.0+m*t));
    f[3][4]=-3/(256*(1.0+m*t));
    f[3][5]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[3][6]=-3/(256*pow((1.0+m*t),2));
    f[3][7]=-8/(256*(1.0+m*t)*(2.0+m*t));
    f[3][8]=-16/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[3][9]=(-2.0*m*t*(4.0+m*t)*(4+3.0*m*t))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 5
    f[4][0]=1.0/256;
    f[4][1]=3/(256*(1.0+m*t));
    f[4][2]=-2/(256*(1.0+m*t));
    f[4][3]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[4][4]=1/(256*(1.0+m*t));
    f[4][5]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[4][6]=9/(256*pow((1.0+m*t),2));
    f[4][7]=-8/(256*(1.0+m*t)*(2.0+m*t));
    f[4][8]=-16/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[4][9]=(2.0*m*t*pow(4.0+m*t,2))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 6
    f[5][0]=1.0/256;
    f[5][1]=-1/(256*(1.0+m*t));
    f[5][2]=2/(256*(1.0+m*t));
    f[5][3]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[5][4]=1/(256*(1.0+m*t));
    f[5][5]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[5][6]=1/(256*pow((1.0+m*t),2));
    f[5][7]=-8/(256*(1.0+m*t)*(2.0+m*t));
    f[5][8]=16/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[5][9]=(m*t*(2.0*16+40.0*m*t+10.0*pow(m,2)*pow(t,2)))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 7
    f[6][0]=1.0/256;
    f[6][1]=-1/(256*(1.0+m*t));
    f[6][2]=-2/(256*(1.0+m*t));
    f[6][3]=4/(256*(1.0+m*t)*(2.0+m*t));
    f[6][4]=1/(256*(1.0+m*t));
    f[6][5]=4/(256*(1.0+m*t)*(2.0+m*t));
    f[6][6]=-3/(256*pow((1.0+m*t),2));
    f[6][7]=-8/(256*(1.0+m*t)*(2.0+m*t));
    f[6][8]=16/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[6][9]=((4.0+m*t)*2.0*pow(m,2)*pow(t,2))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 8
    f[7][0]=1.0/256;
    f[7][1]=3/(256*(1.0+m*t));
    f[7][2]=-2/(256*(1.0+m*t));
    f[7][3]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[7][4]=-3/(256*(1.0+m*t));
    f[7][5]=-4/(256*(1.0+m*t)*(2.0+m*t));
    f[7][6]=-3/(256*pow((1.0+m*t),2));
    f[7][7]=8/(256*(1.0+m*t)*(2.0+m*t));
    f[7][8]=16/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[7][9]=((4.0+m*t)*2.0*pow(m,2)*pow(t,2))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 9
    f[8][0]= 1.0/256;
    f[8][1]= -1/(256*(1.0+m*t));
    f[8][2]= -2/(256*(1.0+m*t));
    f[8][3]= 4/(256*(1.0+m*t)*(2.0+m*t));
    f[8][4]= 1/(256*(1.0+m*t));
    f[8][5]= -4/(256*(1.0+m*t)*(2.0+m*t));
    f[8][6]= 1/(256*pow((1.0+m*t),2));
    f[8][7]= 0;
    f[8][8]= 0;
    f[8][9]=(-(4.0+2.0*m*t)*pow(m,2)*pow(t,2))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 10
    f[9][0]= 1.0/256;
    f[9][1]= -1/(256*(1.0+m*t));
    f[9][2]= 2/(256*(1.0+m*t));
    f[9][3]= -4/(256*(1.0+m*t)*(2.0+m*t));
    f[9][4]= -3/(256*(1.0+m*t));
    f[9][5]= 4/(256*(1.0+m*t)*(2.0+m*t));
    f[9][6]= 1/(256*pow((1.0+m*t),2));
    f[9][7]= 0;
    f[9][8]= 0;
    f[9][9]=(-(4.0+2.0*m*t)*pow(m,2)*pow(t,2))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    //row 11
    f[10][0]= 1.0/256;
    f[10][1]= -1/(256*(1.0+m*t));
    f[10][2]= -2/(256*(1.0+m*t));
    f[10][3]= 4/(256*(1.0+m*t)*(2.0+m*t));
    f[10][4]= -3/(256*(1.0+m*t));
    f[10][5]= 4/(256*(1.0+m*t)*(2.0+m*t));
    f[10][6]= 1/(256*pow((1.0+m*t),2));
    f[10][7]= 8/(256*(1.0+m*t)*(2.0+m*t));
    f[10][8]= -16/(256*(1.0+m*t)*pow((2.0+m*t),2));
    f[10][9]=(2*pow(m,3)*pow(t,3))/
    (256*pow((1.0+m*t),2)*pow((2.0+m*t),2)*(3.0+m*t));
    
    long double g[10]  {
    1.0,
    (2.0*m*theta+lambda)*pow(2.0*m*theta,-2.0)*(log(2.0*m*theta+lambda)-log(lambda))-1.0/(2.0*m*theta),
    (lambda)*pow(2.0*m*theta,-2.0)*(log(lambda)-log(2.0*m*theta+lambda))+1.0/(2.0*m*theta),
    pow(m*theta*2.0*m*theta*(3.0*m*theta),-1.0)*(m*theta*lambda*log(lambda)-(3.0*m*theta)*
        (2.0*m*theta+lambda)*log(2.0*m*theta+lambda)
        +(2.0*m*theta)*(3.0*m*theta+lambda)*log(3.0*m*theta+lambda)),
        0.5/(2.0*m*theta+lambda),
    (3.0*m*theta+lambda)*pow(m*theta,-2.0)*(log(3.0*m*theta+lambda)-log(2.0*m*theta+lambda))
        -1.0/(m*theta),
    (4.0*m*theta+lambda)*pow(2.0*m*theta,-2.0)*(log(4.0*m*theta+lambda)-log(2.0*m*theta+lambda))
        -1.0/(2.0*m*theta),
    (2.0*m*theta+lambda)*pow(m*theta,-2.0)*(log(2.0*m*theta+lambda)-log(3.0*m*theta+lambda))
        +1.0/(m*theta),
    pow(m*theta*m*theta*(2.0*m*theta),-1.0)*(m*theta*(2.0*m*theta+lambda)*log(2.0*m*theta+lambda)-
        (2.0*m*theta)*(3.0*m*theta+lambda)*log(3.0*m*theta+lambda)
        +(m*theta)*(4.0*m*theta+lambda)*log(4.0*m*theta+lambda)),
    pow(-(1.0+2.0*m*theta)*(2.0*m*theta),-1.0)
        *(-(2.0*m*theta+lambda)*log(2.0*m*theta+lambda)-
        (2.0*m*theta)*(1.0+4.0*m*theta+lambda)*log(1.0+4.0*m*theta+lambda)
        +(1.0+2.0*m*theta)*(4.0*m*theta+lambda)*log(4.0*m*theta+lambda))
    };
    std::vector<long double> h {0,0,0,0,0,0,0,0,0,0,0};
    
    for (int i = 0; i < 11; i++)
    {
        
        for (int j = 0; j < 10; j++)
        {
         //   double r{f[i][j] * 2.0*lambda*g[j]};
            if(j !=0){
                h[i] += f[i][j] * 2.0*lambda*g[j];
            }
            else{
                h[i] += 1.0/256;
            }
        }
    }
    
  /*  for(int i=0; i<11; ++i){
        std::cout << h[i] << " ";
    }
    std::cout << '\n'; */
    
    return h;
}

int main(int argc, char *argv[]) {
 
     std::string str1a{};
     
     std::stringstream str1{argv[1]};
     str1 >> str1a;
     
     double theta{};
     std::stringstream input2{argv[2]};
     input2 >> theta;
      
   std::ifstream inf(str1a);
    std::string fileLine;
    std::vector<std::string> seqArray{};
    std::vector<std::string> nameVector{};
    int recordInd{};
    std::string str3{";"};
    std::string str4{"matrix"};
    
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
    
    int noSeq{};
    noSeq=static_cast<int>(seqArray.size());
    
    for(int i=0; i<seqArray.size(); ++i){
        std::cout << seqArray[i].length() << " ";
    }
    std::cout << '\n';
    
       std::ofstream out3("output8", std::ios::app);
    
    std::vector<outputSum> output2{};
    
    for(int i=0; i<(noSeq-3); ++i){
        for(int j=(i+1); j<(noSeq-2); ++j){
            for(int k=(j+1); k<(noSeq-1); ++k){
                for(int l=(k+1); l<noSeq; ++l){
                    outputSum output1{i,j,k,l,0,0,0};
                    output2.push_back(output1);
                }
            }
        }
    }
    
    std::cout << noSeq << " " << output2.size() << " " ;
    int counter{};
    
       tbb::parallel_for( tbb::blocked_range<int>(0,(noSeq-3)),
    [&](tbb::blocked_range<int> s)
    {
    for(int taxa1=s.begin(); taxa1<s.end(); ++taxa1){
        tbb::parallel_for( tbb::blocked_range<int>(taxa1+1,(noSeq-2)),
     [&](tbb::blocked_range<int> t)
     {
        for(int taxa2=t.begin(); taxa2<t.end(); ++taxa2){
         tbb::parallel_for( tbb::blocked_range<int>(taxa2+1,(noSeq-1)),
      [&](tbb::blocked_range<int> u)
      {
            for(int taxa3=u.begin(); taxa3<u.end(); ++taxa3){
       tbb::parallel_for( tbb::blocked_range<int>(taxa3+1,noSeq),
    [&](tbb::blocked_range<int> v)
    {
          for(int taxa4=v.begin(); taxa4<v.end(); ++taxa4){
                
    std::vector<std::string> seqArray2{
        seqArray[taxa1], seqArray[taxa2], seqArray[taxa3], seqArray[taxa4]
                };
         
              double beta{};
              
              std::vector<int> pair1{pairCounts(seqArray[taxa1], seqArray[taxa2])};
              std::vector<int> pair2{pairCounts(seqArray[taxa1], seqArray[taxa3])};
              std::vector<int> pair3{pairCounts(seqArray[taxa1], seqArray[taxa4])};
              std::vector<int> pair4{pairCounts(seqArray[taxa2], seqArray[taxa3])};
              std::vector<int> pair5{pairCounts(seqArray[taxa2], seqArray[taxa4])};
              std::vector<int> pair6{pairCounts(seqArray[taxa3], seqArray[taxa4])};
              
              double phat1 {(pair1[1]+0.0)/(pair1[1]+pair1[0])};
              double tau1 {-6.0*log(1.0-4.0*phat1/3)/0.012};
              
              double phat2 {(pair2[1]+0.0)/(pair2[1]+pair2[0])};
              double tau2 {-6.0*log(1.0-4.0*phat2/3)/0.012};
              
              double phat3 {(pair3[1]+0.0)/(pair3[1]+pair3[0])};
              double tau3 {-6.0*log(1.0-4.0*phat3/3)/0.012};
              
              double phat4 {(pair4[1]+0.0)/(pair4[1]+pair4[0])};
              double tau4 {-6.0*log(1.0-4.0*phat4/3)/0.012};
              
              double phat5 {(pair5[1]+0.0)/(pair5[1]+pair5[0])};
              double tau5 {-6.0*log(1.0-4.0*phat5/3)/0.012};
              
              double phat6 {(pair6[1]+0.0)/(pair6[1]+pair6[0])};
              double tau6 {-6.0*log(1.0-4.0*phat6/3)/0.012};
              
              beta = (tau1+tau2+tau3+tau4+tau5+tau6)/6.0;
              
              std::vector<int> countVec{generateCounts(seqArray2)};
    std::vector<double> posteriorLiks{getPostLik(1.0/beta, theta, 4.0/3, countVec)};
              std::vector<double> finalProb{probVec(posteriorLiks)};
   std::vector<double> finalProbVec{0,0,0};
       finalProbVec[0]=finalProb[0]+finalProb[1]+finalProb[6]+finalProb[11]+finalProb[14];
       finalProbVec[1]=finalProb[2]+finalProb[3]+finalProb[8]+finalProb[9]+finalProb[13];
       finalProbVec[2]=finalProb[4]+finalProb[5]+finalProb[7]+finalProb[10]+finalProb[12];

              for(int i=0; i<output2.size(); ++i){
                  if(output2[i].ID1==taxa1
                     && output2[i].ID2==taxa2
                     && output2[i].ID3==taxa3
                     && output2[i].ID4==taxa4){
                      output2[i].prob1=finalProbVec[0];
                      output2[i].prob2=finalProbVec[1];
                      output2[i].prob3=finalProbVec[2];
                  }
              }
              
              ++counter;
              if(counter % 100 == 0){
                  std::cout << counter << " ";
              }
              
                  }
                            });
              }
                                          });
              }
                                          });
    }
       });
              
    for(int i=0; i<output2.size(); ++i){
        out3 << output2[i].ID1 << "," << output2[i].ID2  << "|" << output2[i].ID3  << "," << output2[i].ID4  << ":" << output2[i].prob1 << '\n';
       out3 << output2[i].ID1 << "," << output2[i].ID3  << "|" << output2[i].ID2  << "," << output2[i].ID4  << ":" << output2[i].prob2 << '\n';
       out3 << output2[i].ID1 << "," << output2[i].ID4  << "|" << output2[i].ID2  << "," << output2[i].ID3  << ":" << output2[i].prob3 << '\n';
    }
    
    
    return 0;
}
