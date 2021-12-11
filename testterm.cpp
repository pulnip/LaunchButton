#define __RELEASE

#include"LBEngine/Terminal.h"
#include<iostream>

int main(int argc, const char* argv[2]){
    if(argc == 1) argv[1]="cat test.txt | sort ; echo END";
    My::initLog();
    My::Terminal t;

    std::string s=argv[1];
    t.run(s);

    std::string result;
    while(std::getline(t.outbuffer, result, '\n')){
        std::cout<<result<<std::endl;
    }
    My::closeLog();

    return 0;
}