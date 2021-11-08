#include<string>
#include<vector>
#include<iostream>
#include<sstream>
#include<unistd.h>
#include<sys/wait.h>

std::vector<std::string> split(const std::string &str, char delim){
    std::vector<std::string> result;

    std::stringstream ss(str+delim);
    std::string temp;
    while(std::getline(ss, temp, delim)){
        result.push_back(temp);
    }

    return result;
}

class strvecWrapper{
private:
    size_t n;
    const char** pT;

public:
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

int main(void){
    while(true){
        std::string raw;
        std::getline(std::cin, raw);
        auto tokenized=split(raw, ' ');
        strvecWrapper wrapped(tokenized);

        pid_t pid=fork();
        if(pid==0){
            execvp(wrapped[0], const_cast<char* const*>(wrapped.data()));
        }

        int status;
        if(waitpid(pid, &status, 0)==pid){
            std::cout<<"wait success"<<std::endl;
        }
    }
}