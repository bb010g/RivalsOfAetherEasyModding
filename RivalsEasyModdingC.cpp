#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <math.h>
#include <sstream>

using namespace std;

void getOffsets(vector <int> &starts,vector <int> &ends,vector <int> &starta,vector <int> &enda){
    vector <string> lines;
    string temp;
    ifstream f("offsets.txt");
    for(int i=0;f;i++){
        getline(f,temp,'-');
        if(temp=="")
            break;
        starts.push_back(atoi(temp.c_str()));
        getline(f,temp);
        ends.push_back(atoi(temp.c_str()));
    }
    for(int i=0;f;i++){
        getline(f,temp,'-');
        if(temp=="")
            break;
        starta.push_back(atoi(temp.c_str()));
        getline(f,temp);
        enda.push_back(atoi(temp.c_str()));
    }
    f.close();
}

int bitsetInt(bitset <8> a,bitset <8> b,bitset <8> c,bitset <8> d){
    bitset <32> full(a.to_string()+b.to_string()+c.to_string()+d.to_string());
    int temp = 0;
    for(int i=0;i<32;i++)
        if(full.test(31-i))
            temp += pow(2,i);
    return temp;
}

bool compareLastX(vector <char> a, int x, int c[]){
    bool temp = true;
    for(int i=x;i>0;i--){
        if(int(a[a.size()-i])!=c[x-i]){
            temp = false;
        }
        //printf("\n%i==%i",int(a[a.size()-i]),c[x-i]);
    }

    return temp;
}

bool saveOffsetsFromList(vector <int> spriteOffs,vector <int> wavOffs,string offsets){
    ofstream offsetsTxt(offsets.c_str());
    for(int i=0;i<spriteOffs.size();i+=2)
        offsetsTxt<<spriteOffs[i]<<"-"<<spriteOffs[i+1]<<endl;
    offsetsTxt<<"-\n";
    for(int i=0;i<wavOffs.size();i+=2)
        offsetsTxt<<wavOffs[i]<<"-"<<wavOffs[i+1]<<endl;

    return true;
}

bool update(string exe, string offsets){
    vector <int> offsetList;
    vector <int> wavOffsetList;
    vector <char> lastEight;
    int currentTuple[2] = {0,0};
    int currentWav[2] = {0,0};
    int currentOffset = -1;
    char currentValue;

    streampos begin,end;
    ifstream myfile ("RivalsofAether.exe", ios::binary);
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    myfile.close();
    printf("\nFile Size - %i\n",end-begin);

    char *file = (char*)malloc (sizeof(char)*(end-begin));

    FILE* fp = fopen(exe.c_str(), "rb");
    while (currentOffset<=(end-begin)){
        currentOffset++;
        currentValue = fgetc(fp);
        file[currentOffset] = currentValue;
        if (currentOffset<=(end-begin))
            lastEight.push_back(int(currentValue));
        if (lastEight.size() > 8){
            reverse(lastEight.begin(),lastEight.end());
            lastEight.pop_back();
            reverse(lastEight.begin(),lastEight.end());
        }
        int otherOtherInts[4] = {82,73,70,70};
        if (lastEight.size()>=4&&equal (lastEight.end() - 4, lastEight.end(), otherOtherInts)){
            currentWav[0] = currentOffset - 3;
            bitset<8> temp1(fgetc(fp));
            bitset<8> temp2(fgetc(fp));
            bitset<8> temp3(fgetc(fp));
            bitset<8> temp4(fgetc(fp));
            int length = bitsetInt(temp1,temp2,temp3,temp4);
            currentOffset += 4;
            currentWav[1] = currentWav[0] + length;
            wavOffsetList.push_back(currentWav[0]);
            wavOffsetList.push_back(currentWav[1]);
            printf("\nWAV start - %i\nWAV length - %i\nWAV end - %i",currentWav[0],length,currentWav[1]);
        }

    }
    char * p = file;
    while(p!=file+(end-begin)){
        printf("HEELLOOOO");
        p = find(p, file+(end-begin), 137);//Search for start of PNG header
        if(file[(p-file)+1]==80&&file[(p-file)+2]==78&&file[(p-file)+3]==71){
            currentTuple[0] = p - file;
            bool found = false;
            while(found==false&&p!=file+(end-begin)){
                p = find(p,file+(end-begin),73);
                if(file[(p-file)+1]==69&&file[(p-file)+2]==78&&file[(p-file)+3]==68&&file[(p-file)+4]==174&&file[(p-file)+5]==66&&file[(p-file)+6]==96&&file[(p-file)+7]==130){
                    found = true;
                    currentTuple[1] = (p-file) + 7;
                }
            }
            offsetList.push_back(currentTuple[0]);
            offsetList.push_back(currentTuple[1]);
        }
    }
    free(file);
    fclose(fp);
    printf("\n%i - %X",currentOffset,currentValue);
    if(!saveOffsetsFromList(offsetList,wavOffsetList,offsets))
        return false;

    return true;
}

bool ripSprites(string exe,string folder){
    vector <int> s1,e1,s2,e2;
    char buffer[3];
    getOffsets(s1,e1,s2,e2);
    FILE* rivalsEXE = fopen(exe.c_str(), "rb");
    for(int i=0;i<s1.size();i++){

        int start = s1[i], end = e1[i];
        itoa(i+1,buffer,10);
        string temp(buffer);
        temp = "sprites\\RIP_"+temp+".png";
        FILE* f = fopen(temp.c_str(),"wb");
        fseek(rivalsEXE,start,SEEK_SET);
        for(int j = 0;j<(end-start);j++){
            putc(getc(rivalsEXE),f);
        }
        fclose(f);
    }
    fclose(rivalsEXE);
    printf("\n\n---------------------\nSprite rip complete");
    return true;
}

bool replaceSprites(string exe, string folder){
    vector <int> s1,e1,s2,e2;
    char buffer[3];
    getOffsets(s1,e1,s2,e2);
    FILE* rivalsEXE = fopen(exe.c_str(), "r+b");
    for(int i=0;i<s1.size();i++){

        int start = s1[i], end = e1[i];
        itoa(i+1,buffer,10);
        string temp(buffer);
        temp = "sprites\\RIP_"+temp+".png";
        FILE* f = fopen(temp.c_str(),"rb");
        fseek(rivalsEXE,start,SEEK_SET);
        for(int j = 0;j<(end-start);j++){
            putc(getc(f),rivalsEXE);
        }
        fclose(f);
    }
    fclose(rivalsEXE);
    printf("\n\n---------------------\nSprite replacement complete");
    return true;
}

int main(int argc, char* argv[]){
    string path = argv[0];//Get path of executable
    while(path.substr(path.length()-1,1)!="\\"){//Cut down to path of folder executable is in
        path = path.substr(0,path.length()-1);
    }

    string command;
    if(argc>1)
        command = argv[1];//Set the first argument as the command

    if(argc==1){//If no arguments are provided print the commands
        //printf("\n   Commands   ");
        //printf("\n--------------");
        //printf("\n-rip [exe path] [folder path] (note: input neither will assume everything is in this folder, standard names.)");
        //printf("\n-replace [folder path] [exe path]");
        //printf("\n-ripWav [exe path] [folder path]");
        //printf("\n-replaceWav [folder path] [exe path]");
        //printf("\n-update");
        //printf("\n-help [command]");
    }
    if(argc==3&&command=="-help"){//If help is needed give it

    }

    replaceSprites("RivalsofAether.exe","");

    return 0;
}
