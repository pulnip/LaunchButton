#ifndef __INC_MSTRING_TPP
#define __INC_MSTRING_TPP

template<class Container>
void My::CStyleArray<Container>::copy(const Container &that){
    int i=0;
    for(auto &element: that){
        if constexpr(std::is_class_v<typename Container::value_type>) pT[i]=element.data();
        else pT[i]=reinterpret_cast<value_type*>(&element);
    }
    pT[n]=nullptr;
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
StringContainer My::toSome(const std::string &origin, const std::vector<std::string> &delims){
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
            result.push_back(strip(std::string(&origin[pos], min-pos)));
            result.push_back(delims[i]);
            
            pos=min+delims[idx].size();
        }
        // if not found, exit.
        else{
            result.push_back(strip(std::string(&origin[pos], origin.size()-pos)));
            pos=origin.npos;
        }
    }

    return result;
}

std::string My::strip(const std::string &origin){
    const std::size_t origin_len=origin.length();
    std::size_t first=0, last=0;
    bool once=false;

    for(int i=0; i<origin_len; ++i){
        if(origin[i]-' '){
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

#endif