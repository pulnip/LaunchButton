#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
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

template<typename T>
class ArrayWrapper{
private:
    T* data;
public:
    ArrayWrapper(T* data):data(data){}
    operator T*(){ return data; }
    ~ArrayWrapper(){ delete[] data; }
};

ArrayWrapper<const char*> c_style(const std::vector<std::string>& str_vec){
    const int vec_size=str_vec.size();

    const char** result=new const char*[vec_size+1];

    for(int i=0; i<vec_size; ++i){
        result[i]=str_vec[i].data();
    }
    result[vec_size]=NULL;

    return result;
}

int main(void){
    std::string raw;
    std::getline(std::cin, raw);
    auto tokenized=split(raw, ' ');
    auto strWrapper=c_style(tokenized);
    const char** c_str_arr=strWrapper;

    pid_t pid;
    if((pid=fork())==0){
        execvp(c_str_arr[0], const_cast<char* const*>(c_str_arr));
    }

    int status;
    if(waitpid(pid, &status, 0)==pid){
        std::cout<<"wait success"<<std::endl;
    }
}