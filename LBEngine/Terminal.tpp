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