#ifndef __INC_TERMINAL_TPP
#define __INC_TERMINAL_TPP

int My::Terminal::run(const std::string& raw_command){
    Commands_t commands;

    mkfifo("myfifo1.pipe", 0666);
    mkfifo("myfifo2.pipe", 0666);

    int myio[2]={
        open("myfifo1.pipe", O_RDWR),
        open("myfifo2.pipe", O_RDWR)
    };
    short myin=0;

    std::vector<std::string> delims={";", "&&", "&", "||", "|"};

    auto raw_commands=toList(raw_command, delims, true);
    raw_commands.push_back(";"); // semi-colon ended for last command.
    
    for(auto &command: raw_commands){
        commands.push_back(toList(command));
    }
    
    bool from_myin=false;
    for(auto &command: commands){
        bool to_myout=false;

        int isDelim=in(command.front(), delims);
        bool doubleAmpersand=false;
        bool checkRetval=false;
        bool setDaemon=false;
        switch(isDelim){
        case 1: // "&&"
            doubleAmpersand=true;
        case 3: // "||"
            checkRetval=true;
            break;
        case 2: // "&"
            setDaemon=true;
            break;
        case 4: // "|"
            to_myout=true;
            break;
        default:
            // 0(";"), -1
        break;
        }

        int retVal=execute(command, !setDaemon, from_myin, to_myout, myio, myin);
        from_myin=to_myout=false;

        if(checkRetval && (bool(retVal)!=doubleAmpersand) ){
            return 1;
        }

        if(isDelim==4) {
            // pipeline
            myin=1-myin;
            from_myin=true;
        }
    }

    remove("myfifo1.pipe");
    remove("myfifo2.pipe");

    return 0;
}

My::Terminal::Args_t My::Terminal::preprocessing(const Command_t &command){
    Args_t args;

    for(auto it=command.cbegin(); it!=command.cend(); ++it){
        if(!(*it).compare("<")){
            // redirect
            int fd=open((++it)->c_str(), O_RDONLY);
            dup2(fd, STDIN_FILENO);
        }
        else if(!(*it).compare(">")){
            // redirect to 
            int fd=open((++it)->c_str(), O_CREAT|O_WRONLY);
            dup2(fd, STDOUT_FILENO);
        }
        else args.push_back(*it);
    }

    return args;
}

/**
 * @brief execute program
 * 
 * @param args program argument
 * @param isWait 
 * @return if wait, return success(0) or not. if not, return program pid.
 */
int My::Terminal::execute(
    const Terminal::Command_t &command, const bool isWait,
    const bool myin, const bool myout,
    const int myio_fd[2], const short myin_idx
){
    pid_t pid=fork();

    if(pid==0){
        // pipelining
        if(myin) dup2(myio_fd[myin_idx], STDIN_FILENO);
        else close(myio_fd[myin_idx]);
        if(myout) dup2(myio_fd[1-myin_idx], STDOUT_FILENO);
        else close(myio_fd[1-myin_idx]);

        Args_t args=preprocessing(command);
        CStyleArray wrapped(args);

        if(!isWait) daemon(0, 0);

        if(execvp(wrapped[0], const_cast<char* const*>(wrapped.data()))<0){
            log("Terminal::execute failed\n");
        }
    }

    if(!isWait) return pid;

    int status=0;
    waitpid(pid, &status, 0);
    return status;
}

#endif // __INC_TERMINAL_TPP