//
//  main.cpp
//  test3
//
//  Created by Andrew Richards on 10/17/19.
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


struct node{
    int fromNode;
    int d1;
    int d2;
    double distNode;
};

struct treeProb{
    std::string tree2;
    int prob2;
};

struct treeProb2{
    std::string tree2;
    double prob2;
    int counter;
};

struct branchRF{
    int leftNode;
    int rightNode;
    std::vector<int> leftDec;
    std::vector<int> rightDec;
};

void getNewick3(std::vector<node> tree1, int nodeID, std::string &str);
void inNewickMod(std::string newick1, int nodeCounter, int noTaxa, std::vector<node>& tree3);
std::vector<node> organizeTree(std::vector<node> tree1, int noTaxa);
void getDecendents2(std::vector<node> input2, int x, std::vector <int>& vec1 );
int getRF2(std::vector<node> tree1, std::vector<node> tree2, int noTaxa);
std::vector<branchRF> getInternalBranches(std::vector<node> tree3);

std::vector<branchRF> getInternalBranches(std::vector<node> tree3){
    std::vector<int> taxonList{};
    for(int i=1; i<=(tree3.size()+1); ++i){
        taxonList.push_back(i);
    }
     std::sort(taxonList.begin(), taxonList.end());
    std::vector<branchRF> branchList{};
    for(int i=0; i<tree3.size(); ++i){
       
        if(tree3[i].fromNode==0 && tree3[i].d1<0 && tree3[i].d2<0){
             branchRF b1{};
            b1.leftNode=tree3[i].d1;
            b1.rightNode=tree3[i].d2;
            getDecendents2(tree3, tree3[i].d1, b1.leftDec);
            std::sort(b1.leftDec.begin(), b1.leftDec.end());
            std::set_difference(taxonList.begin(), taxonList.end(), b1.leftDec.begin(), b1.leftDec.end(), std::inserter(b1.rightDec, b1.rightDec.begin()));
            branchList.push_back(b1);
        }
        if(tree3[i].fromNode<0 && tree3[i].d1<0){
             branchRF b1{};
            b1.leftNode=tree3[i].d1;
            b1.rightNode=tree3[i].fromNode;
            getDecendents2(tree3, tree3[i].d1, b1.leftDec);
            std::sort(b1.leftDec.begin(), b1.leftDec.end());
            std::set_difference(taxonList.begin(), taxonList.end(), b1.leftDec.begin(), b1.leftDec.end(), std::inserter(b1.rightDec, b1.rightDec.begin()));
            branchList.push_back(b1);
        }
        if(tree3[i].fromNode<0 && tree3[i].d2<0){
             branchRF b1{};
            b1.leftNode=tree3[i].d2;
            b1.rightNode=tree3[i].fromNode;
            getDecendents2(tree3, tree3[i].d2, b1.leftDec);
            std::sort(b1.leftDec.begin(), b1.leftDec.end());
            std::set_difference(taxonList.begin(), taxonList.end(), b1.leftDec.begin(), b1.leftDec.end(), std::inserter(b1.rightDec, b1.rightDec.begin()));
            branchList.push_back(b1);
        }
    }
    return branchList;
}

int getRFunroot(std::vector<node> tree1, std::vector<node> tree2, int noTaxa){
     int RF{2*(noTaxa-3)};
    std::vector<branchRF> branchListOne{getInternalBranches(tree1)};
    std::vector<branchRF> branchListTwo{getInternalBranches(tree2)};
    for(int i=0; i<branchListOne.size(); ++i){
      for(int j=0; j<branchListTwo.size(); ++j){
          std::sort(branchListOne[i].leftDec.begin(), branchListOne[i].leftDec.end());
          std::sort(branchListTwo[j].leftDec.begin(), branchListTwo[j].leftDec.end());
          std::sort(branchListTwo[j].rightDec.begin(), branchListTwo[j].rightDec.end());
          std::vector<int> a{branchListOne[i].leftDec};
          std::vector<int> b{branchListTwo[j].leftDec};
           std::vector<int> d{branchListTwo[j].rightDec};
          std::vector<int> c{};
          std::vector<int> e{};
          std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(c));
          std::set_symmetric_difference(a.begin(), a.end(), d.begin(), d.end(), std::back_inserter(e));
          if(c.size() == 0 || e.size() == 0){
              RF -= 2;
          }
      }
    }
    
    return RF;
}

int getRF2(std::vector<node> tree1, std::vector<node> tree2, int noTaxa){
    int RF{2*(noTaxa-2)};
    for(int i=-1; i >= -(noTaxa-2); --i){
        for(int j=-1; j >= -(noTaxa-2); --j){
            std::vector<int> a{};
            getDecendents2(tree1, i, a);
            std::vector<int> b{};
            getDecendents2(tree2, j, b);
            std::vector<int> c{};
            std::sort(a.begin(), a.end());
            std::sort(b.begin(), b.end());
            std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(c));
            if(c.size() == 0){
                RF -= 2;
            }
        }
    }
    return RF;
}

void getDecendents2(std::vector<node> input2, int x, std::vector <int>& vec1 ){
    for(int i=0; i<input2.size(); ++i){
        if(input2[i].fromNode==x){

                if(input2[i].d1<0){
                 //   vec1.push_back(input2[i].d1);
                    getDecendents2(input2, input2[i].d1, vec1 );
                }
                else{
                    vec1.push_back(input2[i].d1);
                }
                if(input2[i].d2<0){
                   // vec1.push_back(input2[i].d2);
                    getDecendents2(input2, input2[i].d2, vec1 );
                }
                else{
                    vec1.push_back(input2[i].d2);
                }
        }
    }
    return;
}

std::vector<node> organizeTree(std::vector<node> tree1, int noTaxa){
    std::vector<node> tree2{tree1};
    std::vector< std::vector<int> > decMat{};
    for(int i=0; i >= -(noTaxa-2); --i){
        std::vector<int> r{};
        getDecendents2(tree1, i, r);
        std::sort(r.begin(), r.end());
        decMat.push_back(r);
    }
    
    for(int i=0; i<tree1.size(); ++i){
        if(tree1[i].d1 >0 && tree1[i].d2 >0){
            if(tree1[i].d1>tree1[i].d2){
                tree2[i].d1=tree1[i].d2;
                tree2[i].d2=tree1[i].d1;
            }
        }
        
        if(tree1[i].d1 >0 && tree1[i].d2 <0){
            if(tree1[i].d1>decMat[-tree1[i].d2][0]){
                tree2[i].d1=tree1[i].d2;
                tree2[i].d2=tree1[i].d1;
            }
        }

        if(tree1[i].d1 <0 && tree1[i].d2 >0){
            if(tree1[i].d2<decMat[-tree1[i].d1][0]){
                tree2[i].d1=tree1[i].d2;
                tree2[i].d2=tree1[i].d1;
            }
        }
        
        if(tree1[i].d1 <0 && tree1[i].d2 <0){
            if(decMat[-tree1[i].d1][0]>decMat[-tree1[i].d2][0]){
                tree2[i].d1=tree1[i].d2;
                tree2[i].d2=tree1[i].d1;
            }
        }
    }
    
    return tree2;
}

void inNewickMod(std::string newick1, int nodeCounter, int noTaxa, std::vector<node>& tree3){
    node node1{};
    if(nodeCounter<(noTaxa-2)){
        int rightPos{};
        int leftPos{};
        int commaPos{};
        int leftTaxon{};
        int rightTaxon{};
        for(int i=0; i<newick1.length(); ++i){
            std::string r{newick1[i]};
            if(r.compare(")")==0){
                rightPos=i;
                break;
            }
        }
        for (int j=rightPos; j>=0; --j){
            std::string s{newick1[j]};
            if(s.compare("(")==0){
                leftPos=j;
                break;
        }
    }
        for(int k=leftPos; k<rightPos; ++k){
            std::string t{newick1[k]};
            if(t.compare(",")==0){
                commaPos=k;
                break;
        }
    }
        std::string u{};
        std::string t{};
        
        for(int l=(leftPos+1); l<commaPos; ++l){
            u.push_back(newick1[l]);
        }
            std::stringstream u2(u);
            u2 >> leftTaxon;
        if(leftTaxon >=0){
            ++leftTaxon;
        }
            for(int l=(commaPos+1); l<rightPos; ++l){
             t.push_back(newick1[l]);
            }
                std::stringstream t2(t);
                t2 >> rightTaxon;
        if(rightTaxon >=0){
            ++rightTaxon;
        }
        node1.fromNode=-(noTaxa-nodeCounter-2);
        node1.d1=leftTaxon;
        node1.d2=rightTaxon;
        node1.distNode=(nodeCounter+1.0)*1.1;
        tree3.push_back(node1);
        std::string v{};
        for(int i=0; i<leftPos; ++i){
            v.push_back(newick1[i]);
        }
        v.push_back(45);
        if(node1.fromNode>(-10)){
        v.push_back(-node1.fromNode+48);
        }
        else{
            int digit1 {-node1.fromNode/10};
            int digit2 {-node1.fromNode % 10};
            v.push_back(digit1+48);
            v.push_back(digit2+48);
        }
        for(int i=(rightPos+1); i<newick1.length(); ++i){
            v.push_back(newick1[i]);
        }
        ++nodeCounter;
        inNewickMod(v, nodeCounter, noTaxa, tree3);
    }
    else{
        int commaPos{};
        int leftTaxon{};
        int rightTaxon{};
        node1.fromNode=0;
        std::string u{};
        std::string t{};
            for(int k=0; k<newick1.length(); ++k){
                std::string t{newick1[k]};
                if(t.compare(",")==0){
                    commaPos=k;
                    break;
            }
        }
        for(int l=1; l<commaPos; ++l){
            u.push_back(newick1[l]);
        }
            std::stringstream u2(u);
            u2 >> leftTaxon;
        if(leftTaxon >=0){
            ++leftTaxon;
        }
            for(int l=(commaPos+1); l<newick1.length(); ++l){
             t.push_back(newick1[l]);
            }
                std::stringstream t2(t);
                t2 >> rightTaxon;
        if(rightTaxon >=0){
            ++rightTaxon;
        }
        node1.d1=leftTaxon;
        node1.d2=rightTaxon;
        node1.distNode=(nodeCounter+1.0)*1.1;
         tree3.push_back(node1);
    }
}

void getNewick3(std::vector<node> tree1, int nodeID, std::string &str){
    for (int i=0; i<tree1.size(); ++i){
        if(tree1[i].fromNode==nodeID){
            str.append("(");;
            if(tree1[i].d1<0){
                getNewick3(tree1, tree1[i].d1, str);
            }
            else{
                if(tree1[i].d1<10){
                    str.push_back(tree1[i].d1+48);
                }
                else{
                    int a{tree1[i].d1/10};
                     int b{tree1[i].d1 % 10};
                     str.push_back(a+48);
                     str.push_back(b+48);
                }
            }
            str.append(",");;
            if(tree1[i].d2<0){
                getNewick3(tree1, tree1[i].d2, str);
            }
            else{
                if(tree1[i].d2<10){
                    str.push_back(tree1[i].d2+48);
                }
                else{
                    int a{tree1[i].d2/10};
                     int b{tree1[i].d2 % 10};
                     str.push_back(a+48);
                     str.push_back(b+48);
                }
            }
            str.append(")");;
        }
    }
}

int main(int argc, char *argv[]) {
 
    std::string str1{};
    std::stringstream option1{argv[1]};
    option1 >> str1;
        
    int noRuns{};
    std::stringstream option2{argv[2]};
    option2 >> noRuns;

    int rootedInd{};
     std::stringstream option3{argv[3]};
     option3 >> rootedInd;
    
    int noTaxa{};
     std::stringstream option4{argv[4]};
     option4 >> noTaxa;
    
    
     std::ifstream inf{str1};
   

    
 std::string fileLine;
    
    
   std::vector<treeProb2> inputList{};
  

    // If we couldn't open the output file stream for reading
    if (!inf)
    {
        // Print an error and exit
        std::cerr << "Uh oh, Sample.dat could not be opened for reading!" << std::endl;
        exit(1);
    }
    
    
 
    int counter{};
    while(getline(inf, fileLine)){
        if(counter<noRuns){
            std::string input1{};
            double prob1{};
            std::stringstream str2{fileLine};
            str2 >> input1;
            str2 >> prob1;
            std::vector<node> tree1{};
            inNewickMod(input1, 0, noTaxa, tree1);
        input1="";
             organizeTree(tree1, noTaxa);
        getNewick3(tree1, 0, input1);
            treeProb2 s{};
            s.tree2=input1;
            s.prob2=prob1;
            s.counter=1;
            inputList.push_back(s);
        }
        ++counter;
    }
  
    std::vector<int> includeInd{};
    for(int i=0; i<noRuns; ++i){
        includeInd.push_back(0);
    }
    
    std::vector<treeProb2> outputList{};
    
    if(rootedInd==1){
         for(int i=0; i<noRuns; ++i){
             if(includeInd[i]==0){
                 treeProb2 r{};
                 r.tree2=inputList[i].tree2;
                 r.prob2=inputList[i].prob2;
                 r.counter=1;
                 for(int j=(i+1); j<noRuns; ++j){
                     if(r.tree2.compare(inputList[j].tree2)==0){
                         r.prob2 += inputList[j].prob2;
                         ++r.counter;
                         includeInd[j]=1;
                     }
                 }
                 outputList.push_back(r);
             }
         }
    }
    
    
    std::ofstream out4{};
    

      out4.open("output2b", std::ios::app);
    
    for(int i=0; i<outputList.size(); ++i){
        out4 << outputList[i].tree2 << " " << (outputList[i].prob2+0.0)/outputList[i].counter <<
        " " << outputList[i].counter << '\n';
    }
   
    
    out4.close();
   
    
    return 0;
}
