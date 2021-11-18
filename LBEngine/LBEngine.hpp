#ifndef __INC_LBENGINE_HPP
#define __INC_LBENGINE_HPP

namespace My{
    int logfd=-1;
    int initLog(void){ return logfd=open("LBEngine.log", O_CREAT|O_WRONLY|O_APPEND); }
    int closeLog(void){ return close(logfd); }
    int log(const char* str){ int len=strlen(str); return len-write(logfd, str, len); }
    int log(int i){
        if(i==0) return 0;
        else if(i<0) return log(-i)+1-write(logfd, "-", 1);
        else{
            int temp=log(i/10);
            char buffer[2]; buffer[0]='0'+i%10; buffer[1]='\0';
            return temp+1-write(logfd, buffer, 1);
        }
    }

    class LBEngine{
    public:
        using ElapsedTime=double;

    private:
        bool isActive=false;

        WINDOW *base=nullptr;
        Size baseSize={0, 0};

        std::list<Button> buttons;

    public:
        LBEngine(){ log("Engine Created\n"); }
        ~LBEngine(){ log("Engine Closed\n"); }

    private:
    /** @brief Main Engine Thread */
        void EngineThread(void);
        int OnCreate(void);
        int OnUpdate(ElapsedTime dt);
        int OnDestroy(void);

        int prompt(void);
        int click(MEVENT *mevent);

    public:
    /** @brief start the Engine */
        int start(void);
    };
}

#endif