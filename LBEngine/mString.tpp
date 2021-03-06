#ifndef __INC_MSTRING_TPP
#define __INC_MSTRING_TPP

template<class Container>
void My::CStyleArray<Container>::copy(const Container &that){
    int i=0;
    for(auto &element: that){
        if constexpr(std::is_class_v<typename Container::value_type>) pT[i]=element.data();
        else pT[i]=&element;
        i+=1;
    }
    pT[i]=NULL;
}

template<class Container>
My::CStyleArray<Container>::CStyleArray(const Container &that)
:n(that.size()), pT(new const value_type*[n+1]){
    copy(that);
}

template<class Container>
typename My::CStyleArray<Container>::self_type&
My::CStyleArray<Container>::operator=(const Container &that){
    delete[] pT;
    n=that.size(); pT=new const value_type*[n+1];
    
    copy(that);

    return *this;
}

template<class StringContainer>
StringContainer My::toSome(
    const std::string &origin, const std::vector<std::string> &delims,
    const bool includeDelim
){
    StringContainer result;
    const std::size_t delims_size=delims.size();

    std::size_t pos=0;
    while(pos!=origin.npos){
        int idx=0;
        std::size_t min=origin.npos;
        // check delims
        // "idx" for delims indexing
        // "min" for origin indexing
        for(int i=0; i<delims_size; ++i){
            std::size_t temp=origin.find(delims[i], pos);
            if(min>temp){
                idx=i;
                min=temp;
            }
        }
        
        // if found, push_back
        if(min!=origin.npos){
            result.push_back(strip(std::string(&origin[pos], min-pos), " "));
            if(includeDelim){
                result.push_back(delims[idx]);
            }
            pos=min+delims[idx].size();
        }
        // if not found, exit.
        else{
            std::string rest=strip(std::string(&origin[pos], origin.size()-pos), " ");
            if(rest.compare(""))
                result.push_back(rest);
            pos=origin.npos;
        }
    }

    return result;
}

std::pair<std::string, std::string> My::toStringPair(
    const std::string &raw, const char delim
){
    const std::size_t raw_len=raw.size();
    
    std::size_t delim_idx=-1;
    while(raw[++delim_idx]!=delim);

    return std::pair<std::string, std::string>(
        raw.substr(0, delim_idx), raw.substr(delim_idx+1, raw_len)
    );
}

std::string My::strip(const std::string &origin, const std::string &remove){
    const std::size_t origin_len=origin.size();
    const std::size_t remove_len=remove.size();

    std::size_t first=0, last=0;
    bool once=false;

    for(int i=0; i<origin_len; ++i){
        bool isUseful=true;
        for(int j=0; j<remove_len; ++j){
            if(origin[i]==remove[j]){
                isUseful=false;
                break;
            }
        }

        if(isUseful){
            if(!once){
                first=i;
                once=true;
            }
            last=i;
        }
    }
    last+=1;

    return origin.substr(first, last-first);
}

/**
 * @brief check "is target in seq?"
 * 
 * @tparam StringContainer 
 * @param target comparison target
 * @param seq iterable sequence
 * @return index (if not in, -1)
 */
template<class StringContainer>
int My::in(const std::string &target, const StringContainer &seq){
    int result=0;
    for(auto &str: seq){
        if(!target.compare(str)) return result;
        result+=1;
    }
    return -1;
}

template<> int My::in(const std::string &substr, const std::string &str){
    const int end=str.size()-substr.size();
    const int substr_size=substr.size();

    for(int i=0; i<end; ++i){
        if(substr[0]==str[i]){
            bool isSubstr=true;
            for(int j=1; j<substr_size; ++j){
                
                if(substr[j]!=str[i+j]){
                    isSubstr=false;
                    break;
                }
            }
            if(isSubstr) return i;
        }
    }

    return -1;
}

#endif // __INC_MSTRING_TPP