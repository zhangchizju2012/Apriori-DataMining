//
//  main.cpp
//  assignment1
//
//  Created by ZHANG CHI on 2017/2/1.
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
    
    std::vector<std::vector<int>> candidateGen(std::vector<std::vector<int>> myvector){
        std::vector<std::vector<int>> result;
        int lenList = int(myvector.size());
        //std::cout << lenList << std::endl;
        if(lenList > 1){
            int length;
            length= int(myvector.at(0).size());
            //std::cout << length << std::endl;
            if(length==1){
                for(int i=0;i<lenList;i++){
                    for(int j=i+1;j<lenList;j++){
                        result.push_back({myvector.at(i).at(0),myvector.at(j).at(0)});
                    }
                }
            }
            else{
                setMyVector(myvector);
                int index = 0;
                int gap = 1;
                std::vector<int> a;
                std::vector<int> b;
                while(index + gap < lenList){
                    a = myvector.at(index);
                    b = myvector.at(index+gap);
                    while(checkFormerSame(a, b, length)){
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
                            //std::vector<int> tempPush = a;
                            //tempPush.push_back(b.at(length-1));
                            //result.push_back(tempPush);
                            a.push_back(b.at(length-1));
                            result.push_back(a);
                            a.pop_back();
                        }
                        if(index+gap>=lenList){break;}
                        else{
                            b = myvector.at(index+gap);
                        }
                    }
                    index = index + 1;
                    gap = 1;
                }
                
            }
        }
        //std::cout << "hello" << std::endl;
        return result;
        
    }
    
    
    std::vector<std::vector<int>> firstPass(std::unordered_map<int,int> freqList, int threshold){
        //for the first time
        std::vector<int> tempVector;
        for ( auto it = freqList.begin(); it != freqList.end(); ++it ){
            if(it->second>=threshold){
                tempVector.push_back(it->first);
            }
        }
        std::sort (tempVector.begin(), tempVector.end());
        
        std::vector<std::vector<int>> myvector;
        for (std::vector<int>::iterator it = tempVector.begin() ; it != tempVector.end(); ++it){
            myvector.push_back({*it});
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
    
};



int main (int argc, char *argv[])
{
    std::clock_t start = std::clock();
    //string name = argv[2];
    string name = "/Users/zhangchi/Desktop/Apriori-DataMining/Apriori-v1/Apriori-v1/test_easy.txt";
    int threshold = 2;
    //string mystring = argv[4];
    //int threshold = atoi(mystring.c_str());
    string line_;
    ifstream file_(name);
    int buf;
    std::vector<std::unordered_set<int>> transList;
    std::unordered_set<int> eachTran;
    std::unordered_map<int,int> freqList;
    if(file_.is_open()){
        while(getline(file_, line_)){
            stringstream ss(line_);
            while (ss >> buf){
                eachTran.insert(buf);
                auto it = freqList.find(buf);
                if(it != freqList.end()){it->second = it->second + 1;}
                else{freqList.insert({buf, 1});}
            }
            transList.push_back(eachTran);
            eachTran.clear();
        }
        file_.close();
    }
    else{cout<<"file is not open!"<<endl;}
    
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    apriori a;
    a.setTransList(transList);
    
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    std::vector<std::vector<int>> survive;
    std::vector<std::vector<int>> candidate;
    std::vector<std::vector<std::vector<int>>> result;
    survive = a.firstPass(freqList, threshold);
    
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    result.push_back(survive);
    
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    while(survive.size()>0){
        candidate =  a.candidateGen(survive);
        cout << "candidate size: "<< candidate.size() << endl;
        cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
        survive = a.checkCandidate(candidate, threshold);
        cout << "survive size: " << survive.size() << endl;
        cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
        result.push_back(survive);
        cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    }
    int resultCount = 0;
    int resultTemp = 0;
    for (int i=0; i<result.size()-1; i++){
        for(int j=0;j<result.at(i).size();j++){
            for(int k=0;k<result.at(i).at(j).size();k++){
                if(k==0){
                    cout << "[ ";
                }
                cout << result.at(i).at(j).at(k) << " ";
                if(k==result.at(i).at(j).size()-1){
                    cout << "] ";
                    resultCount = resultCount + 1;
                }
            }
        }
        cout << "result count: " << resultCount - resultTemp << endl;
        resultTemp = resultCount;
    }
    cout << endl;
    cout << "result count: " << resultCount << endl;
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
    return 0;
}
