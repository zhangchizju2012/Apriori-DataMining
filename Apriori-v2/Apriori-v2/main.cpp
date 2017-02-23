//
//  main.cpp
//  version 2, 10 times faster than version 1.
//
//  Created by ZHANG CHI on 2017/2/3.
//  Copyright © 2017年 ZHANG CHI. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <map>
#include <iomanip>
using namespace std;

class apriori
{
public:
    std::vector<std::unordered_set<int>> transList;
    std::vector<std::vector<int>> myvectorEachNumber;
    void setTransList(std::vector<std::unordered_set<int>> tran){
        transList = tran;
    }
    
     bool checkFormerSame(std::vector<int> a,std::vector<int> b, int k){
         //maybe can pop one item of each vector and compare
         bool label = true;
         for(int i=0;i<k-1;i++){
             if(a.at(i) != b.at(i)){
                 label = false;
                 break;
             }
         }
         return label;
     }
    
    void setMyVector(std::vector<std::vector<int>> myVectorTemp){
        myvectorEachNumber = myVectorTemp;
    }
    
    bool checkInList(std::vector<int> a, int k, int length){
        bool label = false;
        std::vector<int> temp;
        for(int i=k;i<length;i++){
            if(a==myvectorEachNumber.at(i)){
                label = true;
                break;
            }
        }
        return label;
    }
    
    std::vector<int> checkNumber(std::vector<int> a, std::vector<int> b){
        std::vector<int> convergeResult;
        unsigned iA = 0;
        unsigned iB = 0;
        unsigned long lengthA = a.size();
        unsigned long lengthB = b.size();
        while(iA<lengthA && iB<lengthB){
            if(a.at(iA)==b.at(iB)){
                convergeResult.push_back(a.at(iA));
                iA = iA + 1;
                iB = iB + 1;
            }
            else if(a.at(iA)>b.at(iB)){
                iB = iB + 1;
            }
            else{
                iA = iA + 1;
            }
        }
        return convergeResult;
    }
    
    
    std::map<std::vector<int>,std::vector<int>> candidateGen(std::map<std::vector<int>,std::vector<int>> myvector,
                                                             int threshold,
                                                             std::vector<std::vector<int>> tempResult){
        // data structure of myvector
        // key: itemset with min_support; value: transactions which have itemset
        std::map<std::vector<int>,std::vector<int>> result;
        std::vector<int> convergeResult;
        std::vector<int> key;
        int lenList = int(myvector.size());
        //std::cout << lenList << std::endl;
        if(lenList > 1){
            int length;
            length= int(myvector.begin()->first.size());
            //std::cout << length << std::endl;
            if(length==1){
                for ( auto it = myvector.begin(); it != myvector.end(); ++it ){
                    auto temp = it;
                    ++temp;
                    for ( auto j = temp; j != myvector.end(); ++j ){
                        convergeResult = checkNumber(it->second, j->second);
                        if(convergeResult.size()>=threshold){
                            key.push_back(it->first.at(0));
                            key.push_back(j->first.at(0));
                            result.insert({key,convergeResult});
                            key.clear();
                        }
                        
                    }
                }
            }
            else{
                setMyVector(tempResult);
                int index = 0;
                int gap = 1;
                std::vector<int> a;
                std::vector<int> b;
                auto itA = myvector.begin();
                auto itB = myvector.begin();
                ++itB;
                while(index + gap < lenList){
                    a = itA->first;
                    b = itB->first;
                    
                    while(checkFormerSame(a, b, length)){
                        ++itB;
                        gap = gap + 1;
                        bool label = true;
                        for(int k=0;k<length-1;k++){
                            std::vector<int> temp;
                            for(int m=0;m<k;m++){
                                temp.push_back(a.at(m));
                            }
                            for(int n=k+1;n<length;n++){
                                temp.push_back(a.at(n));
                            }
                            temp.push_back(b.at(length-1));
                            if(checkInList(temp, index+gap, lenList)==false){
                                label = false;
                                break;
                            }
                        }
                        if(label==true){
                            --itB;
                            convergeResult = checkNumber(itA->second, itB->second);
                            ++itB;
                            if(convergeResult.size()>=threshold){
                                a.push_back(b.at(length-1));
                                key = a;
                                result.insert({key,convergeResult});
                                a.pop_back();
                                key.clear();
                            }
                        }
                        if(index+gap>=lenList){break;}
                        else{
                            b = itB->first;
                        }
                    }
                    index = index + 1;
                    gap = 1;
                    ++itA;
                    itB = itA;
                    ++itB;
                }
                
            }
        }
        //std::cout << "hello" << std::endl;
        return result;
        
    }
    
    
    std::map<std::vector<int>,std::vector<int>> firstPass(std::unordered_map<int,std::vector<int>> freqList,
                                                          int threshold){
        //C2 = candidates generated from L1
        std::vector<int> tempVector;
        for ( auto it = freqList.begin(); it != freqList.end(); ++it ){
            if(it->second.size()>=threshold){
                tempVector.push_back(it->first);
            }
        }
        std::sort (tempVector.begin(), tempVector.end());
        
        std::map<std::vector<int>,std::vector<int>> myvector;
        std::vector<int> key;
        std::vector<int> value;
        //std::vector<std::vector<int>> myvector;
        for (std::vector<int>::iterator it = tempVector.begin() ; it != tempVector.end(); ++it){
            key.push_back(*it);
            auto j = freqList.find(*it);
            myvector.insert({key, j->second});
            key.clear();
        }
        return myvector;
    }
};

vector< vector<int> > getAllSubsets(vector<int> set)
// Generate all subsets for a itemset (except empty and itself),
// this function is used in generating association rules
// eg. itemset = {2,3,5}, this function will return
// {2},{3},{5},{2,3},{2,5},{3,5} ({} and {2,3,5} is not included)
{
    vector< vector<int> > subset;
    vector<int> empty;
    subset.push_back( empty );
    
    for (int i = 0; i < set.size(); i++)
    {
        vector< vector<int> > subsetTemp = subset;
        
        for (int j = 0; j < subsetTemp.size(); j++)
        subsetTemp[j].push_back( set[i] );
        
        for (int j = 0; j < subsetTemp.size(); j++)
        subset.push_back( subsetTemp[j] );
    }
    return subset;
}

vector<int> getRemain(vector<int> all, vector<int> subset){
// Generate a vector which is in vector 'all' but not in vector 'subset'
// both 'all' and 'subset' are sorted
// this function is used in generating association rules
// eg. all = {2,3,5}, subset = {2,3}
// this function will return {5}
    vector<int> remain;
    int i = 0;
    int j = 0;
    while(i<all.size() and j<subset.size()){
        if(all.at(i)<subset.at(j)){
            remain.push_back(all.at(i));
            i = i + 1;
        }
        else{
            i = i + 1;
            j = j + 1;
        }
    }
    while(i<all.size()){
        remain.push_back(all.at(i));
        i = i + 1;
    }
    return remain;
}

int getAssociationRuleNumber(std::unordered_map<int,std::map<std::vector<int>,int>> freqItemSetsAndNumber,
                             std::vector<std::vector<std::vector<int>>> result,
                             float confidence, int lineNumber, bool show=false){
    // Data structure of freqItemSetsAndNumber:
    // key: size of each itemset (eg. freqItemSet:(2,3,4), size is 3), value: a map, called MAP, see below
    // MAP's key: itemset with min_support, all itemsets here have same size;
    // MAP's value: number of transactions which have itemset
    // eg. itemset size -> {2:({item6,item7}:{trans1},{item4,item8}:{trans3,trans8,trans11}),
    //     itemset size -> 3:({item1,item2,item3}:{trans1,trans2,trans5,trans8},{item1,item4,item8}:{trans2,trans8})}
    
    // each member in result is itemset with min_support
    
    // Generating Association Rules from Frequent Itemsets
    // Algorithm Description:
    // For each frequent itemset, f, generate all non-empty subsets of f.
    // For every non-empty subset s of f do
    //      output rule s->(f-s) if support(f)/support(s) >= min_confidence
    // end
    vector< vector<int> > subsets;
    int denominator; //#X
    unsigned long denominatorLength;
    int numerator; //#(X&Y)
    unsigned long numeratorLength;
    int associationRuleNumber = 0;//count association Rule number
    vector<int> remain;
    ofstream file_("/Users/zhangchi/Desktop/Apriori-DataMining/Apriori-v2/Apriori-v2/rule.txt"); //for rule.txt file
    for(int i=1; i<result.size()-1; i++){
        for(int j=0;j<result.at(i).size();j++){
            numeratorLength = result.at(i).at(j).size();
            auto it = freqItemSetsAndNumber.find(int(numeratorLength));
            auto ij = it->second.find(result.at(i).at(j));
            numerator = ij->second; //#(X&Y)
            subsets = getAllSubsets(result.at(i).at(j));
            for(int k=1;k<subsets.size()-1;k++){
                denominatorLength = subsets.at(k).size();
                auto im = freqItemSetsAndNumber.find(int(denominatorLength));
                auto in = im->second.find(subsets.at(k));
                denominator = in->second; //#X
                // Confidence(A->B) = Prob(B/A) = Support(A->B)/Support(A) = #(X&Y)/#X
                if(float(numerator)/float(denominator)>=confidence||denominator==0){
                    if(show){
                        if(file_.is_open()){
                            for(int m=0;m<subsets.at(k).size()-1;m++){
                                //cout << subsets.at(k).at(m) << ",";
                                file_ << subsets.at(k).at(m) << ",";
                            }
                            //cout << subsets.at(k).at(subsets.at(k).size()-1) << " -> " ;
                            file_ << subsets.at(k).at(subsets.at(k).size()-1) << " -> " ;
                            remain = getRemain(result.at(i).at(j), subsets.at(k));
                            for(int n=0;n<remain.size()-1;n++){
                                //cout << remain.at(n) << ",";
                                file_ << remain.at(n) << ",";
                            }
                            //cout << remain.at(remain.size()-1) << " ";
                            file_ << remain.at(remain.size()-1) << " ";
                            //cout << "(" << std::fixed << std::setprecision(2) << float(numerator)/float(lineNumber)<< "," ;
                            file_ << "(" << std::fixed << std::setprecision(2) << float(numerator)/float(lineNumber)<< "," ;
                            //cout << std::fixed << std::setprecision(2) << float(numerator)/float(denominator) << ")" << endl;
                            file_ << std::fixed << std::setprecision(2) << float(numerator)/float(denominator) << ")" << endl;
                        }
                        
                    }
                    associationRuleNumber = associationRuleNumber + 1;
                }
            }
        }
    }
    return associationRuleNumber;
}

int getResultCountAndPrintResult(vector<std::vector<std::vector<int>>> result, bool show=true){
    int resultCount = 0;
    int resultTemp = 0;
    for (int i=0; i<result.size()-1; i++){
        for(int j=0;j<result.at(i).size();j++){
            for(int k=0;k<result.at(i).at(j).size();k++){
                if(k==0){
                    if(show){cout << "[ ";}
                }
                if(show)
                if(show){cout << result.at(i).at(j).at(k) << " ";}
                if(k==result.at(i).at(j).size()-1){
                    if(show){cout << "] ";}
                    resultCount = resultCount + 1;
                }
            }
        }
        cout << "result count: " << resultCount - resultTemp << endl;
        resultTemp = resultCount;
    }
    return resultCount;
}

void printFrequentItemsets(std::vector<std::map<std::vector<int>,int>> prepareForPrintFrequentItemsets,
                           int lineNumber){
    ofstream file_("/Users/zhangchi/Desktop/Apriori-DataMining/Apriori-v2/Apriori-v2/freq.txt");
    if(file_.is_open()){
        for (int i=0; i<prepareForPrintFrequentItemsets.size(); i++){
            for (std::map<std::vector<int>,int>::iterator it=prepareForPrintFrequentItemsets.at(i).begin(); it!=prepareForPrintFrequentItemsets.at(i).end(); ++it){
                for (int j=0; j<it->first.size()-1; j++){
                    //std::cout << it->first.at(j) << ",";
                    file_ << std::fixed << std::setprecision(2) << it->first.at(j) << ",";
                }
                //std::cout << it->first.at(it->first.size()-1) << " ";
                file_ << std::fixed << std::setprecision(2) << it->first.at(it->first.size()-1) << " ";
                //std::cout << "(" << float(it->second)/float(lineNumber) << ")" << std::endl;
                file_ << "(" << float(it->second)/float(lineNumber) << ")" << std::endl;
            }
        }
    }
}

int main (int argc, char *argv[])
{
    std::clock_t start = std::clock();
    string name = argv[2];
    //string name = "/Users/zhangchi/Desktop/Apriori-DataMining/Apriori-v2/Apriori-v2/test_easy.txt";
    //int threshold = 2;
    float confidence = 0.8;
    string mystring = argv[4];
    int threshold = atoi(mystring.c_str());
    string line_;
    ifstream file_(name);
    int buf;
    std::vector<std::unordered_set<int>> transList;
    std::unordered_set<int> eachTran;
    std::unordered_map<int,std::vector<int>> freqList;
    std::vector<int> value;
    int lineNumber = 0;
    if(file_.is_open()){
        while(getline(file_, line_)){
            stringstream ss(line_);
            while (ss >> buf){
                eachTran.insert(buf);
                auto it = freqList.find(buf);
                if(it != freqList.end()){
                    it->second.push_back(lineNumber);
                }
                else{
                    value.push_back(lineNumber);
                    freqList.insert({buf, value});
                    value.clear();
                }
            }
            transList.push_back(eachTran);
            eachTran.clear();
            lineNumber = lineNumber + 1;
        }
        file_.close();
    }
    else{cout<<"file is not open!"<<endl;}
    
    apriori a;
    a.setTransList(transList);
    
    std::unordered_map<int,std::map<std::vector<int>,int>> freqItemSetsAndNumber;
    // key: size of each itemset (eg. freqItemSet:(2,3,4), size is 3) value: tempFreqItemSetsAndNumber
    // prepare for Association Rule generation
    std::map<std::vector<int>,std::vector<int>> survive;
    // key: itemset with min_support; value: transactions which have itemset
    std::vector<std::vector<std::vector<int>>> result;
    // constituted by tempResult
    std::map<std::vector<int>,int> tempFreqItemSetsAndNumber;
    // key: itemset with min_support; value: number of transactions which have itemset
    std::vector<std::vector<int>> tempResult;
    // each member in tempResult is itemset with min_support
    std::vector<std::map<std::vector<int>,int>> prepareForPrintFrequentItemsets;
    // constituted by tempFreqItemSetsAndNumber
    
    survive = a.firstPass(freqList, threshold);
    
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    int itemSetsLength = 1;
    for ( auto it = survive.begin(); it != survive.end(); ++it ){
        tempResult.push_back(it->first);
        tempFreqItemSetsAndNumber.insert({it->first,it->second.size()});
    }
    freqItemSetsAndNumber.insert({itemSetsLength, tempFreqItemSetsAndNumber});
    prepareForPrintFrequentItemsets.push_back(tempFreqItemSetsAndNumber);
    tempFreqItemSetsAndNumber.clear();
    
    result.push_back(tempResult);
    
    
    while(survive.size()>0){
        survive =  a.candidateGen(survive, threshold,tempResult);
        cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
        itemSetsLength = itemSetsLength + 1;
        tempResult.clear();
        for ( auto it = survive.begin(); it != survive.end(); ++it ){
            tempResult.push_back(it->first);
            tempFreqItemSetsAndNumber.insert({it->first,it->second.size()});
        }
        freqItemSetsAndNumber.insert({itemSetsLength, tempFreqItemSetsAndNumber});
        prepareForPrintFrequentItemsets.push_back(tempFreqItemSetsAndNumber);
        tempFreqItemSetsAndNumber.clear();
        //cout << "survive size: " << survive.size() << endl;
        result.push_back(tempResult);
    }
    
    int associationRuleNumber = getAssociationRuleNumber(freqItemSetsAndNumber, result, confidence, lineNumber, true);
    int resultCount = getResultCountAndPrintResult(result,false);
    printFrequentItemsets(prepareForPrintFrequentItemsets,lineNumber);
    
    
    cout << endl;
    cout << "result count: " << resultCount << endl;
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    cout << "associate rule number: " << associationRuleNumber << endl;
    return 0;
}
