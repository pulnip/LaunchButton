#define __RELEASE
#include"LBEngine/LBEngine.h"

int main(void){
    if(My::initLog()<0){
        perror("Failed to start logging");
        return 1;
    }

    My::LBEngine be;

    if(be.start()){
        perror("Failed to start engine");
        return 2;
    }

    if(My::closeLog()){
        perror("Failed to close logging");
        return 3;
    }
    
    return 0;
}