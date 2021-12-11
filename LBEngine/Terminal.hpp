#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

#include<poll.h>

namespace My{
    class Terminal{
    public:
        using Command_t=std::list<std::string>;
        using Commands_t=std::list<Command_t>;
        using Args_t=std::vector<std::string>;

    private:
        int pipefd[2][2]={{-1, -1}, {-1, -1}};
        int in_idx=0;

        int* myin  (void){ return pipefd[  in_idx]; }
        int* myout (void){ return pipefd[1-in_idx]; }
        void swapio(void){        in_idx=1-in_idx ; }

        void flushpipe(void){
        #define BUFFER_SIZE 255
            char buffer[BUFFER_SIZE+1];

            struct pollfd _pollfd={.fd=myout()[0], .events=POLLIN};
            while(true){
                if( poll(&_pollfd, 1, 0)==1){
                    int nread=read(myout()[0], buffer, BUFFER_SIZE);
                    buffer[nread]='\0';
                    outbuffer<<buffer;
                }else break;
            }
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
        int execute(
            const Command_t &command, const bool isWait,
            const bool _in=false, const bool _out=false
        );
    };
}

#endif // __INC_TERMINAL_HPP