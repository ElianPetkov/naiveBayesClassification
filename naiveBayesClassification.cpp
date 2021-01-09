#include "helpingFuncs.cpp"


/* we take data from readFile and save it in peoples array*/
void readFileData(ifstream &readFile)
{
    string readLine;
    while(getline(readFile, readLine))
    {

        vector<string> lineResult;
        split(',',readLine,lineResult);
        lineResult[0].copy(peoples[peopleNumber].className,lineResult[0].size());
        for(int i=0; i<NUMBER_OF_CHARACKTERISTICS; i++)
        {
            peoples[peopleNumber].charachteristics[i] = lineResult[i+1];
        }
        peopleNumber++;
    }
}

void readFile(const char* fileName)
{
    ifstream readFile;
    readFile.open(fileName);
    readFileData(readFile);
    readFile.close();
}

void k_fold_cross_validation()
{
    random_shuffle(peoples,peoples+peopleNumber);

    vector<int> setSizes;
    setSizesOfK_fold_Sets(setSizes);
    int checkedExemplars=0;
    double avgAccuracy=0.0;

    for(int setNumber=0; setNumber<NUMBER_OF_SETS; setNumber++)
    {
        int leftBorder = checkedExemplars;
        int rightBorder = checkedExemplars + setSizes[setNumber]-1;

        int republicansCount = countPeoples("republican",leftBorder,rightBorder);
        int democratsCount = countPeoples("democrat",leftBorder,rightBorder);

        double republicantChance = (double)republicansCount/(peopleNumber-setSizes[setNumber]);
        double democratChance = (double)democratsCount/(peopleNumber-setSizes[setNumber]);

        map <string, double> countCharacteristicForRepublicans;
        map <string, double> countCharacteristicForDemocrats;

        vector<map<string,double>> republicansLearning;
        vector<map<string,double>> democratsLearning;

        /*Learning phase*/
        for(int i=0; i<NUMBER_OF_CHARACKTERISTICS; i++)
        {
            countCharacteristicForRepublicans["y"]=0;
            countCharacteristicForRepublicans["n"]=0;
            countCharacteristicForRepublicans["?"]=0;

            countCharacteristicForDemocrats["y"]=0;
            countCharacteristicForDemocrats["n"]=0;
            countCharacteristicForDemocrats["?"]=0;

            for(int j=0; j<peopleNumber; j++)
            {
                if(j<leftBorder || j>rightBorder)
                {
                    if(!strcmp(peoples[j].className,"republican"))
                    {
                        countCharacteristicForRepublicans[peoples[j].charachteristics[i]]++;
                    }
                    else
                    {
                        countCharacteristicForDemocrats[peoples[j].charachteristics[i]]++;
                    }
                }
            }

            calculateChanceForCharacteristic(countCharacteristicForRepublicans,republicansCount);
            calculateChanceForCharacteristic(countCharacteristicForDemocrats,democratsCount);

            republicansLearning.push_back(countCharacteristicForRepublicans);
            democratsLearning.push_back(countCharacteristicForDemocrats);
        }
        /*Classification Phase*/
        int match=0;
        for(int i=leftBorder; i<=rightBorder; i++)
        {
            double percentagesCurrentTestRepublican = 1.0, percentagesCurrentTestDemocrat = 1.0;
            for(int j=0;j<NUMBER_OF_CHARACKTERISTICS;j++)
            {
                percentagesCurrentTestRepublican*=republicansLearning[j][peoples[i].charachteristics[j]];
                percentagesCurrentTestDemocrat*=democratsLearning[j][peoples[i].charachteristics[j]];
            }
            percentagesCurrentTestDemocrat*=democratChance;
            percentagesCurrentTestRepublican*=republicantChance;

            if(percentagesCurrentTestDemocrat >= percentagesCurrentTestRepublican && !strcmp(peoples[i].className,"democrat"))
                match++;
            if(percentagesCurrentTestDemocrat <= percentagesCurrentTestRepublican && !strcmp(peoples[i].className,"republican"))
                match++;
        }
        double accuracy = ((double)match / setSizes[setNumber]) * 100.0;
        avgAccuracy += accuracy;
        cout << "Accuracy is " << accuracy << "% for test set " << setNumber << endl;

        checkedExemplars+=setSizes[setNumber];
    }
    cout << "Average accuracy is " << avgAccuracy / NUMBER_OF_SETS <<"%"<< endl;

}
int main()
{
    srand((unsigned)time(0));
    readFile("house-votes-84.data");
    k_fold_cross_validation();


}
