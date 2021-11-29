#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

namespace My{
    class Terminal{
    public:
        using Command_t=std::list<std::string>;
        using Commands_t=std::list<Command_t>;
        using Args_t=std::vector<std::string>;

    private:
        Terminal()=delete;
    public:
        static int run(const std::string& raw_command);
    private:
        static Args_t preprocessing(const Command_t &command);
        static int execute(
            const Command_t &command, const bool isWait,
            const bool myin, const bool myout,
            const int myio_fd[2], const short myin_idx
        );
    };
}

#endif // __INC_TERMINAL_HPP