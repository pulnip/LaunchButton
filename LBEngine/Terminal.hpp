#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

namespace My{
    using Args_t=std::vector<std::string>;
    using Command_t=std::list<std::string>;
    using Commands_t=std::list<Command_t>;

    class Terminal{
    private:
        Terminal()=delete;
    public:
        static int run(const std::string& raw_command);
    private:
        static int execute(const Args_t &args);
        static int pipeline(const Args_t &args);
        static int redirect(const Args_t &args);
        static My::Args_t My::Terminal::toArgs(const Command_t &cmd);
    };
}

#endif