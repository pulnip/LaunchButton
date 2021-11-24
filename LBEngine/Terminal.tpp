#ifndef __INC_TERMINAL_TPP
#define __INC_TERMINAL_TPP

int My::Terminal::run(const std::string& raw_command){
    Commands_t commands;
    auto programs=toList(raw_command);
    for(auto &program: programs){
        commands.push_back(toList(program, {" "}, false));
    }
    
    for(auto &command: commands){
    #warning "Not impl";
        // pipe, redirect
        Args_t args=toArgs(command);
        execute(args);
    }
}

int My::Terminal::execute(const Args_t& args){
    pid_t pid=fork();
    if(pid==0){
        CStyleArray wrapped(args);

        if(execvp(wrapped[0], const_cast<char* const*>(wrapped.data()))<0){
            log("Terminal::execute failed\n");
        }
    }

    int status;
    waitpid(pid, &status, 0);
    return status;
}

int My::Terminal::pipeline(const Args_t &args){

}

int My::Terminal::redirect(const Args_t &args){

}

My::Args_t My::Terminal::toArgs(const Command_t &cmd){

}

#endif