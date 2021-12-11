#ifndef __INC_LOGGING_HPP
#define __INC_LOGGING_HPP

namespace My{
    int logfd=-1;
    int initLog(void){ return logfd=open("LBEngine.log", O_CREAT|O_WRONLY|O_APPEND, 0666); }
    int closeLog(void){ return close(logfd); }
    int log(const char* str){ int len=strlen(str); int result=len-write(logfd, str, len); fsync(logfd); return result; }
    int log(int i);
}

#endif // __INC_LOGGING_HPP