#ifndef __INC_LOGGING_TPP
#define __INC_LOGGING_TPP

int My::log(int i){
    static bool once=true;
    if(i==0){
        if(once)
            return 1-write(logfd, "0", 1);
        once=true;
        return 0;
    }
    else{
        once=false;
        if(i<0)
            return log(-i)+1-write(logfd, "-", 1);
        int temp=log(i/10);
        char buffer[2]={'\0', '\0'}; buffer[0]='0'+(i%10);
        return temp+1-write(logfd, buffer, 1);
    }
}

#endif // __INC_LOGGING_TPP