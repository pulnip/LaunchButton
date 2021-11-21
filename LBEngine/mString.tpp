#ifndef __INC_MSTRING_TPP
#define __INC_MSTRING_TPP

My::strvecWrapper::strvecWrapper(const std::vector<std::string_view>& that)
:n(that.size()), pT(new const char*[n+1]){
    for(int i=0; i<n; ++i){
        pT[i]=that[i].data();
    }
    pT[n]=NULL;
}

My::strvecWrapper::strvecWrapper(const strvecWrapper &that)
:n(that.n), pT(new const char*[n+1]){
    for(int i=0; i<n; ++i){
        pT[i]=that[i];
    }
    pT[n]=NULL;
}

My::strvecWrapper&
My::strvecWrapper::operator=(const std::vector<std::string_view>& that){
    delete[] pT;
    n=that.size();
    pT=new const char*[n+1];
    for(int i=0; i<n; ++i){
        pT[i]=that[i].data();
    }
    pT=NULL;

    return *this;
}

My::strvecWrapper& My::strvecWrapper::operator=(const strvecWrapper &that){
    if(this!=&that){
        delete[] pT;
        n=that.n;
        pT=new const char*[n+1];
        for(int i=0; i<n; ++i){
            pT[i]=that[i];
        }
        pT[n]=NULL;
    }

    return *this;
}

std::list<std::string_view>
My::toList(const std::string_view &origin, const std::vector<std::string> &delims){
    std::list<std::string_view> res;
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
            res.push_back(strip(std::string_view(&origin[pos], min-pos)));
            pos=min+delims[idx].size();
        }
        // if not found, exit.
        else{
            res.push_back(strip(std::string_view(&origin[pos], origin.size()-pos)));
            pos=origin.npos;
        }
    }

    return res;
}

std::vector<std::string_view>
My::toVector(const std::string_view &origin, const std::vector<std::string> &delims){
    std::vector<std::string_view> res;
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
            res.push_back(strip(std::string_view(&origin[pos], min-pos)));
            pos=min+delims[idx].size();
        }
        // if not found, exit.
        else{
            res.push_back(strip(std::string_view(&origin[pos], origin.size()-pos)));
            pos=origin.npos;
        }
    }

    return res;
}

std::string_view My::strip(const std::string_view &origin){
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