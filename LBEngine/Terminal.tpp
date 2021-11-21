#ifndef __INC_TERMINAL_TPP
#define __INC_TERMINAL_TPP

int My::Terminal::execute(const std::string& raw_command){
    #warning "Not impl";
    auto programs=toList(raw_command);
    
    //...
    for(auto& raw_args: programs){
        Args_t args=toVector(raw_args);
        executeOne(args);
    }
    //...
}

int My::Terminal::executeOne(const Args_t& args){
    pid_t pid=fork();
    if(pid==0){
        strvecWrapper wrapped=args;

        if(execvp(wrapped[0], const_cast<char* const*>(wrapped.data()))<0){
            log("Terminal::execute failed\n");
        }
    }

    int status;
    waitpid(pid, &status, 0);
    return status;
}

int My::Terminal::pipeline(const My::Command_t& cmd){

}

int My::Terminal::redirect(const My::Args_t& arg){

}

#endif