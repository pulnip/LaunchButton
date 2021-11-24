#ifndef __INC_TERMINAL_TPP
#define __INC_TERMINAL_TPP

int My::Terminal::execute(const std::string &raw_command){
    #warning "Not impl";
    auto programs=toList(raw_command);
    
    //...
    for(auto& raw_args: programs){
        Args_t args=toVector(raw_args);
        executeOne(args);
    }
    //...
}

int My::Terminal::executeOne(const Args_t &args){
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

int My::Terminal::pipeline(bool out, bool in){
    if(!(out&&in)) return 1;


    // exec recursively, with fifo file?

    // in fifo file
    // out fifo file

    // out->in
    // in->out

    // if done "from", pop "from" (at exec)
}

int My::Terminal::redirect(const My::Args_t &arg){

}

My::Args_t My::toArgs(const Command_t &command){

}

#endif