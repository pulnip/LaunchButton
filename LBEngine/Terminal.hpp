#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

namespace My{
    class Terminal{
    public:
        using Command_t=std::list<std::string>;
        using Commands_t=std::list<Command_t>;
        using Args_t=std::vector<std::string>;

    private:
        int pipeio[2]={-1, -1};
        int in_idx=1;
        inline int  pipein(void){ return pipeio[  in_idx]; }
        inline int pipeout(void){ return pipeio[1-in_idx]; }
        inline void swapio(void){        in_idx=1-in_idx ; }

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
            const bool useMyIn, const bool useMyOut
        );
    };
}

#endif // __INC_TERMINAL_HPP