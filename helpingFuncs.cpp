#ifndef HELPINGFUNC_H
#define HELPINGFUNC_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <ctime>
#include <map>

using namespace std;

const int NUMBER_OF_CHARACKTERISTICS = 16;
const int NUMBER_OF_SETS=10;
int peopleNumber = 0;



struct Person
{
    string charachteristics[NUMBER_OF_CHARACKTERISTICS];
    char className[11];
};
Person peoples[700];

/*split words in a text by delimiter, in our case it's ',' and save the result in the
result prameter
*/
void split(char delim,string text,vector<string> &result)
{
    text+=',';// to catch the last symbol in the string text
    string word="";
    for(int i=0; i<text.size()+1; i++)
    {
        if(text[i] != delim )
            word+=text[i];
        else
        {
            result.push_back(word);
            word="";
        }
    }
    return ;
}

/*
we save the sizes of every set of the K-fold-cross-validation in the parameter @setSizes,
it's important to notice when there is residue in the peopleNumber/NUMBER_OF_SET operation.
Then we include 1 in every set starting from the first set while the residueSize become 0
*/
void setSizesOfK_fold_Sets(vector<int> &setSizes)
{
    int setSize = peopleNumber/NUMBER_OF_SETS;//we calculate the set size
    int residueSize = peopleNumber%NUMBER_OF_SETS;//we check if there is a residue from the peopleNumber/NUMBER_OF_SETS operation
    for(int i=0; i<NUMBER_OF_SETS ; i++) //we save the sizes of every set
    {
        if(residueSize > 0){
            setSizes.push_back(setSize+1);
            residueSize--;
        }
        else
            setSizes.push_back(setSize);
    }
}

/* we count how many times we will see the class given as parameter @name out of the interval [leftBorder,rightBorder]*/
int  countPeoples(const char *name,int leftBorder,int rightBorder)
{
    int currentCount=0;
    for(int i=0; i<peopleNumber; i++)
    {
        if(i<leftBorder || i>rightBorder){
            if(!strcmp(peoples[i].className,name))
                currentCount++;
        }
    }
    return currentCount;

}

/*
    this function calculate the probability for characteristic of one class.
*/
void calculateChanceForCharacteristic(map<string,double> &elems, int denominator)
{
    elems["y"]=(double)elems["y"]/denominator;
    elems["n"]=(double)elems["n"]/denominator;
    elems["?"]=(double)elems["?"]/denominator;
}

#endif // H_H_
