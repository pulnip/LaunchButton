#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

namespace My{
    using Args_t=std::vector<std::string>;
    using Command_t=std::list<Args_t>;

    class Terminal{
    private:
        Terminal()=delete;
    public:
        static int execute(const std::string& raw_command);
    private:
        static int executeOne(const Args_t& args);
        static int pipeline(const Command_t& cmd);
        static int redirect(const Args_t& args);
    };
}

#endif