#ifndef __INC_LOGGING_HPP
#define __INC_LOGGING_HPP

namespace My{
    int logfd=-1;
    int initLog(const std::string &path=""){
        return logfd=open(
            (path+"LBEngine.log").c_str(),
            O_CREAT|O_WRONLY|O_APPEND, 0664
        );
    }
    int closeLog(void){ return close(logfd); }
    int log(const char* str){
        int len=strlen(str);
        return len-write(logfd, str, len);
    }
    int log(int i);
}

#endif // __INC_LOGGING_HPP