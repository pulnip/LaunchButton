#ifndef __INC_MSTRING_HPP
#define __INC_MSTRING_HPP

namespace My{
    class strvecWrapper{
    private:
        size_t n=0;
        const char** pT=nullptr;

    public:
        strvecWrapper()=default;
        strvecWrapper(const std::vector<std::string>& that)
        :n(that.size()), pT(new const char*[n+1]){
            for(int i=0; i<n; ++i){
                pT[i]=that[i].data();
            }
            pT[n]=NULL;
        }
        strvecWrapper& operator=(const std::vector<std::string>& that){
            delete[] pT;
            n=that.size();
            pT=new const char*[n+1];
            for(int i=0; i<n; ++i){
                pT[i]=that[i].data();
            }
            pT=NULL;

            return *this;
        }
        strvecWrapper(const strvecWrapper &that)
        :n(that.n), pT(new const char*[n+1]){
            for(int i=0; i<n; ++i){
                pT[i]=that[i];
            }
            pT[n]=NULL;
        }
        strvecWrapper operator=(const strvecWrapper &that){
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
        const char** data(void) const{ return pT; }
        const char*& operator[](int idx) const{ return pT[idx]; }
        ~strvecWrapper(){ delete[] pT; }
    };
    std::vector<std::string> split(const std::string &str, char delim=' ');
}

#endif