#ifndef __INC_TERMINAL_TPP
#define __INC_TERMINAL_TPP

int My::Terminal::run(const std::string& raw_command){
    Commands_t commands;

    short fifo_out=0;
    int fifo_fd[2][2];
    if(pipe(fifo_fd[0])&&pipe(fifo_fd[1])) return 1;

    std::vector<std::string> delims={";", "&&", "&", "||", "|"};
    for(auto &command: toList(raw_command, delims, true)){
        commands.push_back(toList(command));
    }
    
    int retVal=0;
    for(auto it=commands.begin(); it!=commands.end(); ++it){
        int isDelim=in((*it).front(), delims);
        if(isDelim==-1){
            #warning "Not Impl"
            retVal=execute(preprocessing((*it)));
        }
        else{
            switch(isDelim){
            case 0: // ";"
            break;
            case 1: // "&&"
                if(!retVal) return 1;
            break;
            case 2: // "&"
                // 이전 명령어->bg
                #warning "Not Impl"
            break;
            case 3: // "||"
                if(retVal) return 1;
            break;
            case 4: // "|"
                // pipelining
                #warning "Not Impl"
            }
        }
    }

    return 0;
}

My::Terminal::Args_t My::Terminal::preprocessing(const Command_t &command){

}
/**
 * @brief execute program
 * 
 * @param args program argument
 * @param isWait 
 * @return if wait, return success(0) or not. if not, return program pid.
 */
int My::Terminal::execute(const Terminal::Args_t& args, bool isWait=true){
    pid_t pid=fork();
    if(pid==0){
        CStyleArray wrapped(args);

        if(execvp(wrapped[0], const_cast<char* const*>(wrapped.data()))<0){
            log("Terminal::execute failed\n");
        }
    }

    if(isWait){
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
    else return pid;
}

int My::Terminal::pipeline(const bool in, const bool out){
    if(in){

    }

    // exec recursively, with fifo file?

    // in fifo file
    // out fifo file

    // out->in
    // in->out

    // if done "from", pop "from" (at exec)
}

int My::Terminal::redirect(const My::Terminal::Terminal::Args_t &arg){

}

#endif