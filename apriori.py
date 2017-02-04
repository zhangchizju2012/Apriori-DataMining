#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Wed Feb  1 15:44:01 2017

@author: zhangchi

Basic implementation test of core parts of Apriori.
"""

def candidateGen(test):
    lenList = len(test)
    if lenList > 1:
        length = len(test[0])
        if length == 1:
            return [x+y for x in test for y in test if x<y]
        else:
            index = 0
            gap = 1
            result = []
            while index + gap < lenList:
                a = test[index]
                b = test[index+gap]
                while a[:length-1] == b[:length-1]:
                    gap = gap + 1
                    label = True
                    for j in range(length-1):
                        if a[:j]+a[j+1:length-1]+[a[-1]]+[b[-1]] not in test[index+gap:]:
                        #if [a[j],a[-1],b[-1]] not in test:
                            label = False
                            break
                    if label == True:
                        result.append(a[:length-1]+[a[-1]]+[b[-1]])
                    if index+gap >= lenList:
                        break
                    else: 
                        b = test[index+gap]
                index = index + 1
                gap = 1
            return result
    else:
        return []

test = [[1,2,3],[1,2,4],[1,2,5],[1,3,4],[1,3,5],[1,4,5],[2,3,4],[2,3,5],[2,4,5],[3,4,5]]
#test = [[1],[2],[3]]
print candidateGen(test)
'''
def checkIn(candidate,trans):
    lenCandidate = len(candidate)
    lenTrans = len(trans)
    indexCan = 0
    indexTrans = 0
    label = True
    while lenCandidate - indexCan <= lenTrans - indexTrans and label:
        if indexCan == lenCandidate:
            label = False
        else:
            if candidate[indexCan] == trans[indexTrans]:
                indexCan = indexCan + 1
                indexTrans = indexTrans + 1
            else:
                indexTrans = indexTrans + 1
    return not label

a = [2,7,12,34]
b = [0,1,2,7,12]
#b = [1,2,3,4,5,6,7,8,12,14,15,33,34,56,67]
print checkIn(a,b)
'''
