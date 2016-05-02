#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <math.h>
#include <sstream>
#include <windows.h>
#include <time.h>

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
    printf("\n-");
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

bool compareLastX(vector <char> a, int x, int c[]){
    bool temp = true;
    for(int i=x;i>0;i--){
        if(int(a[a.size()-i])!=c[x-i]){
            temp = false;
        }
    }

    return temp;
}

bool saveOffsetsFromList(vector <int> spriteOffs,vector <int> spriteOffe,vector <int> wavOffs,vector <int> wavOffe,string offsets){
    ofstream offsetsTxt(offsets.c_str());
    for(int i=0;i<spriteOffs.size();i++)
        offsetsTxt<<spriteOffs[i]<<"-"<<spriteOffe[i]<<endl;
    offsetsTxt<<"-\n";
    for(int i=0;i<wavOffs.size();i++)
        offsetsTxt<<wavOffs[i]<<"-"<<wavOffe[i]<<endl;

    return true;
}

bool update(string exe, string offsets){
    printf("Updating offsets, this should take less than 15 seconds...");
    vector <int> s1,e1,s2,e2;
    streampos begin,end;
    ifstream myfile (exe.c_str(), ios::binary);
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    myfile.close();
    int currentOffset = -1;

    char *file = (char*)malloc (sizeof(char)*(end-begin));

    FILE* fp = fopen(exe.c_str(), "rb");

    while (currentOffset<=(end-begin)){
        currentOffset++;
        file[currentOffset] = fgetc(fp);
    }
    char pngStart[] = {137,80,78,71};
    char pngEnd[] = {73,69,78,68,174,66,96,130};
    char wavStart[] = {82,73,70,70};
    int nPngs = 0, nWavs = 0;
    for(int currentOff=0;currentOff<(end-begin);currentOff++){
        bool st = true;
        bool en = true;
        bool au = true;
        for(int j=0;j<8;j++){
            if(!(st||en||au))
                break;
            if(st&&j<4&&file[currentOff+j]!=pngStart[j])
                st = false;
            if(en&&file[currentOff+j]!=pngEnd[j])
                en = false;
            if(au&&j<4&&file[currentOff+j]!=wavStart[j])
                au = false;

            if(j==4){
                if(st)
                    s1.push_back(currentOff);
                if(au){
                    s2.push_back(currentOff);
                    bitset <8> a(file[currentOff+4]);
                    bitset <8> b(file[currentOff+5]);
                    bitset <8> c(file[currentOff+6]);
                    bitset <8> d(file[currentOff+7]);
                    bitset <32> length(a.to_string()+b.to_string()+c.to_string()+d.to_string());
                    currentOff += 8;
                    e2.push_back(currentOff+length.to_ulong());
                }
                if(st||au)
                    break;
            }

        }
        nPngs += st;
        nWavs += au;
        if(st||au)
            currentOff += 4;
        if(en){
            e1.push_back(currentOff);
            currentOff += 8;
        }
    }
    free(file);
    fclose(fp);

    if(!saveOffsetsFromList(s1,e1,s2,e2,offsets))
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
        temp = folder+"RIP_"+temp+".png";
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
        temp = folder+"RIP_"+temp+".png";
        FILE* f = fopen(temp.c_str(),"rb");
        if(f){
            fseek(rivalsEXE,start,SEEK_SET);
            for(int j = 0;j<(end-start);j++){
                putc(getc(f),rivalsEXE);
            }
            fclose(f);
        }
    }
    fclose(rivalsEXE);
    printf("\n\n---------------------\nSprite replacement complete");
    return true;
}

bool ripAudio(string exe,string folder){
    vector <int> s1,e1,s2,e2;
    char buffer[3];
    getOffsets(s1,e1,s2,e2);
    FILE* rivalsEXE = fopen(exe.c_str(), "rb");
    for(int i=0;i<s1.size();i++){

        int start = s2[i], end = e2[i];
        itoa(i+1,buffer,10);
        string temp(buffer);
        temp = folder+"RIP_"+temp+".wav";
        FILE* f = fopen(temp.c_str(),"wb");
        fseek(rivalsEXE,start,SEEK_SET);
        for(int j = 0;j<(end-start);j++){
            putc(getc(rivalsEXE),f);
        }
        fclose(f);
    }
    fclose(rivalsEXE);
    printf("\n\n---------------------\nAudio rip complete");
    return true;
}

bool replaceAudio(string exe, string folder){
    vector <int> s1,e1,s2,e2;
    char buffer[3];
    getOffsets(s1,e1,s2,e2);
    FILE* rivalsEXE = fopen(exe.c_str(), "r+b");
    for(int i=0;i<s1.size();i++){
        int start = s2[i], end = e2[i];
        itoa(i+1,buffer,10);
        string temp(buffer);
        temp = folder+"RIP_"+temp+".wav";
        FILE* f = fopen(temp.c_str(),"rb");
        fseek(rivalsEXE,start,SEEK_SET);
        for(int j = 0;j<(end-start);j++){
            putc(getc(f),rivalsEXE);
        }
        fclose(f);
    }
    fclose(rivalsEXE);
    printf("\n\n---------------------\nAudio replacement complete");
    return true;
}

int main(int argc, char* argv[]){
    string command = "", exePath = "RivalsofAether.exe", spritesFolder = "sprites\\", audioFolder = "audio\\", offsetsPath = "offsets.txt";
    if(argc>1)
        command = argv[1];//Set the first argument as the command
    if(argc==4){
        exePath = argv[2];
        spritesFolder = argv[3];
        audioFolder = argv[3];
        offsetsPath = argv[3];
    }
    ifstream installedCorrectly(exePath.c_str());
    if(!installedCorrectly){
        printf("\n\n---------------------------\nError %s was not found!\n---------------------------\n",exePath.c_str());
        system("PAUSE");
        installedCorrectly.close();
        return 0;
    }
    installedCorrectly.close();

    if(command=="rip")
        ripSprites(exePath,spritesFolder);
    if(command=="replace")
        replaceSprites(exePath,spritesFolder);
    if(command=="ripWav")
        ripAudio(exePath,spritesFolder);
    if(command=="replaceWav")
        replaceAudio(exePath,spritesFolder);
    if(command=="update")
        update(exePath,offsetsPath);

    if(argc==1||(argc==2&&command=="-audio")||(argc==2&&command=="-time")||(argc==2&&command=="-aTime")){
        string sptFolder = "sprites\\";
        if(command=="-time"||command=="-aTime"){
            time_t rawtime;
            struct tm * timeinfo;
            time (&rawtime);
            timeinfo = localtime (&rawtime);
            printf("\nHour - %i\n",timeinfo->tm_hour);
            if(timeinfo->tm_hour<=7||timeinfo->tm_hour>=20)
                sptFolder += "night\\";
            else
                sptFolder += "day\\";
        }
        CreateDirectory("original resources\\",NULL);
        printf("\nBacking up current sprites...");
        ripSprites("RivalsofAether.exe","original resources\\");
        printf("\nReplacing new sprites...");
        replaceSprites("RivalsofAether.exe",sptFolder);
        if(command=="-audio"||command=="-aTime"){
            printf("\nBacking up current audio...");
            ripAudio("RivalsofAether.exe","original resources\\");
            printf("\nReplacing new audio...");
            replaceAudio("RivalsofAether.exe","audio\\");
        }
        ShowWindow( GetConsoleWindow(), SW_HIDE );
        system("RivalsofAether.exe");
        ShowWindow( GetConsoleWindow(), SW_RESTORE );
        printf("\nPutting back original sprites...");
        replaceSprites("RivalsofAether.exe","original resources\\");
        if(command=="-audio"||command=="-aTime"){
            printf("\nPutting back original audio...");
            replaceAudio("RivalsofAether.exe","original resources\\");
        }
    }

    return 0;
}
