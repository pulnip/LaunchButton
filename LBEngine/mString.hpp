#ifndef __INC_MSTRING_HPP
#define __INC_MSTRING_HPP

namespace My{
    template<class Container>
    class CStyleArray{
    public:
        using value_type=std::conditional_t<
            std::is_class_v<typename Container::value_type>,
            typename Container::value_type::value_type, typename Container::value_type
        >;
        using self_type=CStyleArray<Container>;
    private:
        std::size_t n=0;
    public:
        std::size_t size(){ return n; }
    private:
        const value_type** pT=nullptr;
        void copy(const Container &that);
    public:
        CStyleArray()=default;
        CStyleArray(const Container &that);
        self_type& operator=(const Container &that);
        const value_type** data(void) const noexcept{ return pT; }
        const value_type*& operator[](int idx){ return pT[idx]; }
    };

    template<class StringContainer> StringContainer toSome(
        const std::string &origin, const std::vector<std::string> &delim,
        const bool includeDelim
    );
    /**
     * @brief from string to list
     * 
     * @param origin raw string
     * @param delims (The long one must come front. ex) &&, &)
     * @param includeDelim 
     * @return std::list&lt;std::string&gt;
     */
    inline std::list<std::string> toList(
        const std::string &origin,
        const std::vector<std::string> &delims={" "},
        const bool includeDelim=false
    ){
        return toSome<std::list<std::string>>(origin, delims, includeDelim);
    }
    /**
     * @brief from string to vector
     * 
     * @param origin raw string
     * @param delims (The long one must come front. ex) &&, &)
     * @param includeDelim 
     * @return std::vector&lt;std::string&gt;
     */
    inline std::vector<std::string> toVector(
        const std::string &origin,
        const std::vector<std::string> &delims={" "},
        const bool includeDelim=false
    ){
        return toSome<std::vector<std::string>>(origin, delims, includeDelim);
    }

    std::string strip(const std::string &origin);

    template<class StringContainer>
    int in(const std::string &target, const StringContainer &seq);
}

#endif