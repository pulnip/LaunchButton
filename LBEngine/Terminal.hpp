#ifndef __INC_TERMINAL_HPP
#define __INC_TERMINAL_HPP

namespace My{
    using args_t=std::vector<std::string>;
    using command=std::list<args_t>;

    class Terminal{
    public:
        Terminal()=default;
    };
}

#endif