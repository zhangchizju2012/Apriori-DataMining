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
    
    /*
    bool checkFormerSame(std::vector<int> a,std::vector<int> b, int k){
        //maybe can pop one item of each vector and compare
        bool label = true;
        for(int i=k-2;i>-1;i--){
            if(a.at(i) != b.at(i)){
                label = false;
                break;
            }
        }
        return label;
    }
    */
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
    
    
    std::map<std::vector<int>,std::vector<int>> candidateGen(std::map<std::vector<int>,std::vector<int>> myvector, int threshold, std::vector<std::vector<int>> tempResult){
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
    
    
    std::map<std::vector<int>,std::vector<int>> firstPass(std::unordered_map<int,std::vector<int>> freqList, int threshold){
        //for the first time
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
    /*
     bool checkIn(std::vector<int> oneCandidate, std::unordered_set<int> oneTrans){
     bool label = true;
     std::unordered_set<int>::const_iterator got;
     //for (unsigned i=0; i<oneCandidate.size(); i++){
     for (std::vector<int>::iterator it = oneCandidate.begin() ; it != oneCandidate.end(); ++it){
     //got = oneTrans.find (oneCandidate.at(i));
     got = oneTrans.find (*it);
     if (got == oneTrans.end()){
     label = false;
     break;
     }
     }
     return label;
     }
     
     int countNumber(std::vector<int> oneCandidate){
     int number = 0;
     for (int i=0; i<transList.size(); i++){
     if(checkIn(oneCandidate, transList.at(i))){
     number = number + 1;
     }
     }
     return number;
     }
     
     std::vector<std::vector<int>> checkCandidate(std::vector<std::vector<int>> candidate,int threshold){
     std::vector<std::vector<int>> candidateSurvive;
     std::vector<int> temp;
     int count;
     for (int i=0; i<candidate.size(); i++){
     temp = candidate.at(i);
     count = countNumber(temp);
     if(count>=threshold){
     candidateSurvive.push_back(temp);
     }
     }
     return candidateSurvive;
     }
     */
    /*
    std::vector<std::vector<int>> checkCandidate(std::vector<std::vector<int>> candidate,int threshold){
        std::vector<std::vector<int>> candidateSurvive;
        int temp;
        
        for(int i=0; i<candidate.size(); i++){
            temp = 0;
            for(auto& transaction: transList){
                bool label = true;
                for (std::vector<int>::iterator it = candidate.at(i).begin() ; it != candidate.at(i).end(); ++it){
                    if(transaction.find(*it)==transaction.end()){
                        label = false;
                        break;
                    }
                }
                if(label){
                    temp = temp + 1;
                }
            }
            if(temp>=threshold){
                candidateSurvive.push_back(candidate.at(i));
            }
        }
        return candidateSurvive;
    }
    */
};

vector< vector<int> > getAllSubsets(vector<int> set)
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

int getAssociationRuleNumber(std::unordered_map<int,std::map<std::vector<int>,int>> freqItemSetsAndNumber, std::vector<std::vector<std::vector<int>>> result, float confidence, bool show=false){
    vector< vector<int> > subsets;
    int numerator;
    int numeratorLength;
    int denominator;
    int denominatorLength;
    int associationRuleNumber = 0;
    for(int i=1; i<result.size()-1; i++){
        for(int j=0;j<result.at(i).size();j++){
            denominatorLength = result.at(i).at(j).size();
            auto it = freqItemSetsAndNumber.find(denominatorLength);
            auto ij = it->second.find(result.at(i).at(j));
            denominator = ij->second;
            subsets = getAllSubsets(result.at(i).at(j));
            for(int k=1;k<subsets.size()-1;k++){
                numeratorLength = subsets.at(k).size();
                auto im = freqItemSetsAndNumber.find(numeratorLength);
                auto in = im->second.find(subsets.at(k));
                numerator = in->second;
                if(float(denominator)/float(numerator)>=confidence||numerator==0){
                    if(show){
                        for(int m=0;m<subsets.at(k).size();m++){
                            if(m==0){
                                cout << "[ ";
                            }
                            cout << subsets.at(k).at(m) << " ";
                            if(m==subsets.at(k).size()-1){
                                cout << "] " <<endl;
                            }
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
    std::map<std::vector<int>,std::vector<int>> survive;
    //std::vector<std::vector<int>> candidate;
    std::vector<std::vector<std::vector<int>>> result;
    std::map<std::vector<int>,int> tempFreqItemSetsAndNumber;
    
    survive = a.firstPass(freqList, threshold);
    
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    std::vector<std::vector<int>> tempResult;
    int itemSetsLength = 1;
    for ( auto it = survive.begin(); it != survive.end(); ++it ){
        tempResult.push_back(it->first);
        tempFreqItemSetsAndNumber.insert({it->first,it->second.size()});
    }
    freqItemSetsAndNumber.insert({itemSetsLength, tempFreqItemSetsAndNumber});
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
        tempFreqItemSetsAndNumber.clear();
        //cout << "survive size: " << survive.size() << endl;
        result.push_back(tempResult);
    }
    
    int associationRuleNumber = getAssociationRuleNumber(freqItemSetsAndNumber, result, confidence);
    int resultCount = getResultCountAndPrintResult(result,false);
    
    cout << endl;
    cout << "result count: " << resultCount << endl;
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    cout << "associate rule number: " << associationRuleNumber << endl;
    return 0;
}
