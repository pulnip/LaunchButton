#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

#define PIPE1_FILENAME "mypipe1.txt"
#define PIPE2_FILENAME "mypipe2.txt"

#include<iostream>

namespace My{
    class Terminal{
    public:
        using Command_t=std::list<std::string>;
        using Commands_t=std::list<Command_t>;
        using Args_t=std::vector<std::string>;

    private:
        int myiofd[2]={-1, -1};
        int in_idx=0;
        inline int  pipein(void){ return myiofd[in_idx]; }
        inline int pipeout(void){ return myiofd[1-in_idx]; }
        inline void swapio(void){        in_idx=1-in_idx ; }

        void flushpipe(void){
        #define BUFFER_SIZE 255
            int outfd=pipeout();
            char buffer[BUFFER_SIZE+1];

            int nread=0;
            while((nread=read(outfd, buffer, BUFFER_SIZE))>0){
                buffer[nread]='\0';
                std::cout<<buffer;
            }
            close(outfd);
        #undef BUFFER_SIZE
        }
    
    public:
        std::stringstream outbuffer;

    public:
        Terminal();
        ~Terminal();
        int run(const std::string& raw_command);
    
    private:
        std::map<std::string, std::string> envargs;
        void loadEnvArgs(void);
        void saveEnvArgs(void);

        Args_t preprocessing(const Command_t &command);
        int execute(const Command_t &command, const bool isWait);
    };
}

#endif // __INC_TERMINAL_HPP