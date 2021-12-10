#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

#define PIPE1_FILENAME "myfifo1.pipe"
#define PIPE2_FILENAME "myfifo2.pipe"

namespace My{
    class Terminal{
    public:
        using Command_t=std::list<std::string>;
        using Commands_t=std::list<Command_t>;
        using Args_t=std::vector<std::string>;

    private:
        int in_idx=1;
        int pipein(void){
            if(in_idx) return open(PIPE1_FILENAME, O_RDONLY|O_NDELAY);
            else return open(PIPE2_FILENAME, O_RDONLY|O_NDELAY);
        }
        int pipeout(void){
            if(in_idx) return open(PIPE2_FILENAME, O_WRONLY|O_NDELAY);
            else return open(PIPE1_FILENAME, O_WRONLY|O_NDELAY);
        }
        inline void swapio(void){        in_idx=1-in_idx ; }

        void flushpipe(void){
        #define BUFFER_SIZE 255
            int outfd=pipeout();
            char buffer[BUFFER_SIZE+1];

            int nread=0;
            while((nread=read(outfd, buffer, BUFFER_SIZE))>0){
                buffer[nread]='\0';
                outbuffer<<buffer<<'\n';
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