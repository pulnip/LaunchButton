#ifndef __INC_MSTRING_TPP
#define __INC_MSTRING_TPP

std::vector<std::string> My::split(const std::string &str, char delim){
    std::vector<std::string> result;

    std::stringstream ss(str+delim);
    std::string temp;
    while(std::getline(ss, temp, delim)){
        result.push_back(temp);
    }

    return result;
}

#endif