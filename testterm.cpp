#include"LBEngine/Terminal.h"
#include<iostream>

int main(int argc, const char* argv[]){
    if(argc == 1) return 0;
    My::initLog();
    My::Terminal t;

    std::string s=argv[1];
    t.run(s);

    std::string result;
    while(!t.outbuffer.eof()){
        t.outbuffer>>result;
        std::cout<<result<<std::endl;
    }
    My::closeLog();

    return 0;
}