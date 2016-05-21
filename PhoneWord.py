# PhoneConversion
# A program to convert a phone number to all possible equivalent Phonewords  
# Author:   Shahram Mohrehkesh (smohr003@odu.edu)
# Created:  05/16/2016
#
# Copyright (C) 2016 
# For license information, see LICENSE.txt
#
# ID: PhoneWord.py  $

'''
This program converts a (phone) number to all possible equivalnet Phonewords.
Only Phonweords that contain words available in the dictionary are presented. 
The program also shows the best suggestions. 
'''
import re
import string 

### a dictionary of words in set by reading data from an input file
#  @param  
#  @return a dictionary of words  
#
def createDic():
    s = set()

    with open("words")  as infile: 
        line = infile.readline()
        line.strip()
        while line != "": 
            words = line.split()
            for word in words: 
                s.add(word.lower()) 
            line = infile.readline()
            
    return s

##  creates a hashmap with the keys being the keypad numbers and value as the corresponding chars s
#  @param  
#  @return a hashmap with the keys being the keypad numbers and value as the corresponding chars s

def createKeypad(): 
    s = {}
    s[0] = {'0'}
    s[1] = {'1'}
    s[2] = {'a', 'b', 'c'}
    s[3] = {'d', 'e', 'f'}
    s[4] = {'g', 'h', 'i'}
    s[5] = {'j', 'k', 'l'}
    s[6] = {'m', 'n', 'o'}
    s[7] = {'p', 'q', 'r', 's'}
    s[8] = {'t', 'u', 'v'}
    s[9] = {'w', 'x', 'y', 'z'}
    return s

## creates a list which contains the corresponding keypad number for each English alphabet 
#
#  @param  
#  @return a list 
#
def setNum(): 
    charList = [0] * 26 
    charList[0:3] = [2] * 3
    charList[3:6] = [3] * 3
    charList[6:9] = [4] * 3
    charList[9:12] = [5] * 3
    charList[12:15] = [6] * 3
    charList[15:19] = [7] * 4
    charList[19:22] = [8] * 3
    charList[22:25] = [9] * 4
    return charList     
            
## get the corresping digit for each alphabet char based on keypad            
#  @param charlist:  the list of characters for each 26 english alphabet chars. 
#  @param char
#  @return the corresponding digit for a alphabet char 
#
def getDigit(charList, char): 
    if ord(char) >= ord('a') and ord(char)<= ord('z'):
        return charList[ord(char) -ord('a')] 
    else: 
        return (char)
    
## convert a string to its keypad number equivalnet 
#  @param  charlist:  the list of characters for each 26 English alphabet chars. 
#  @param  string: a sequence of alphabets  
#  @return the number corresponding to a string 
#    
def getNum(charList, string): 
    s = 0 
    for c in string: 
        s = s*10 + int(getDigit(charList,c))
    return s 


def getLetters(letterKeypad, num):
    return letterKeypad[num]

## create all the possible combinations for a number in a recursive fashion 
#  @param number: an integer number 
#  @param numStr: the  (partial) equivalent string for the number 
#  @param allCombin: will contain all combinations 
#  @return none
# 
def getCombinations(number, numStr, letterKeypad, allCombin):
    
    if number == 0: 
        return allCombin.append(numStr[::-1])
    else: 
        d = number % 10
        chars = getLetters(letterKeypad, d)
        for e in chars: 
            getCombinations(number//10, numStr+e, letterKeypad, allCombin)

# This is the function that generates all the possible Phonewords in a recursive fashion 
#  @param dicSet: a set containing all words in our English Dictionary 
#  @param  goodNum: a list of all numbers that have a valid corresponding word in the dictionary 
#  @param  num: the number to create its PhoneWords 
#  @param phoneWord: (partially) created Phoneword
#  @param  allConverted: all possible PhoneWords 
#  @return None
def createWords(dicSet, goodNum, num, pos, phoneWord, allConverted): 
    
    if pos >= len(num):
        allConverted.add((phoneWord.lstrip('-')))        
    else:       
        for j in range(0,len(num)): 
            k = min(pos+j+1,len(num))
            n = ''.join(num[pos:k])
            if len(n) == 0: return 
            n = int(n)
            if (n) in goodNum:  
                
                s = createKeypad()
                draft = ''
                allComb = [] 
                getCombinations(n, draft,s, allComb)        
                for  a in allComb: 
                    if a in dicSet: 
                        
                        createWords(dicSet, goodNum,num, k, phoneWord+'-'+a, allConverted)
                    else: 
                        createWords(dicSet, goodNum,num, k, phoneWord+str(n), allConverted)
            else: 
                createWords(dicSet, goodNum,num, k, phoneWord+str(n), allConverted)
                
##################
# main program   #
##################
                     
def main(): 
    # receive the number and clean it 
    num = input("Please Enter the Phone Number: ")
    num = int(re.sub(r'[^\d]', '' , num).strip())
    print(num)
    
    # create keypad and a dictionary of words 
    keyPad = createKeypad()
    dicSet = createDic()
    
    # This only create the single PhoneWords that corresponds to all digits of number. 
    # is very usefull if tne program is used for not 9-digit numbers   
    draft = ''
    allComb = [] 
    getCombinations(num, draft,keyPad, allComb)  
    allCombSet = set(allComb)
    words =  allCombSet.intersection(dicSet)
    print("All conversion to possible words in a dictionary are: ", words)
    
    
    # produces all possible partial PhoneWord in a number 
    allCombAllLength = []     
    numStr = str(num)
    for i in range(len(numStr)):
        for j in range(1,len(numStr)+1): 
            draft = ''        
            getCombinations(int(numStr[i:min(i+j,len(numStr))]), draft,keyPad, allCombAllLength)      
    allCombAllLength = set(allCombAllLength)
    words =  allCombAllLength.intersection(dicSet)
    print("All conversion for various partial numbers to possible words in a dictionary are: ", words)
     
    
    # produces the number corresponding to the partial PhoneWord 
    charList = setNum()
    goodNum = set()
    for w in words: 
        if len(w) >=2: 
            goodNum.add(getNum(charList, w))
    print("Sub-Numbers with possible valid words are: ", goodNum)
    
    

    # combine all partial PhoneWords/digits for the final set of Phone
    allConverted = set()
    l = list(str(num))
    lc = ''
    createWords(dicSet, goodNum ,l, 0, lc, allConverted)    
    print("There are ", len(allConverted), " PhoneWords")
    print("Possible PhoneWords are: ", allConverted)
    
    # prints the best phonewords, which are the phonewords with less than 2 digits in them. 
    print("The best PhoneWords are: " )
    for e in allConverted: 
        if len(re.sub(r'[^\d]', '' , e)) < 2: 
            print(e)
    

if __name__=="__main__": 
   
    main()