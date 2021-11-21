#ifndef __INC_MSTRING_HPP
#define __INC_MSTRING_HPP

namespace My{
    class strvecWrapper{
    private:
        size_t n=0;
        const char** pT=nullptr;

    public:
        strvecWrapper()=default;
        strvecWrapper(const std::vector<std::string_view>& that);
        strvecWrapper(const strvecWrapper &that);
        strvecWrapper& operator=(const std::vector<std::string_view>& that);
        strvecWrapper& operator=(const strvecWrapper &that);
        const char** data(void) const{ return pT; }
        const char*& operator[](int idx) const{ return pT[idx]; }
        ~strvecWrapper(){ delete[] pT; }
    };

    // for pipelining, redirecting, etc...
    std::list<std::string_view> toList(const std::string_view &origin, const std::vector<std::string> &delims={";", "&&", "&", "||", "|"});
    // for exec function(program args)
    std::vector<std::string_view> toVector(const std::string_view &origin, const std::vector<std::string> &delims={" "});
    std::string_view strip(const std::string_view &origin);
    std::string strip(const std::string &origin);
}

#endif