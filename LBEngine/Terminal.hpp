#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

namespace My{
    using Command_t=std::list<std::string>;
    using Commands_t=std::list<Command_t>;
    using Args_t=std::vector<std::string>;

    class Terminal{
    private:
        Terminal()=delete;
    public:
        static int run(const std::string& raw_command);
    private:
        static int execute(const Args_t &args);
        static int pipeline(bool out=true, bool in=false);
        static int redirect(const Args_t &args);
    };

    Args_t toArgs(const Command_t &command);
}

#endif