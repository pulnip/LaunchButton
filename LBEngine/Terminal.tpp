#ifndef __INC_TERMINAL_TPP
#define __INC_TERMINAL_TPP

My::Terminal::Terminal(){
#define PIPE1_FILENAME "myfifo1.pipe"
#define PIPE2_FILENAME "myfifo1.pipe"
#define PERMS 0666
    mkfifo(PIPE1_FILENAME, PERMS);
    mkfifo(PIPE2_FILENAME, PERMS);

    pipeio[0]=open(PIPE1_FILENAME, O_RDWR);
    pipeio[1]=open(PIPE2_FILENAME, O_RDWR);
#undef PERMS
}

My::Terminal::~Terminal(){
    close(pipein ());
    close(pipeout());

    remove(PIPE1_FILENAME);
    remove(PIPE2_FILENAME);
#undef PIPE1_FILENAME
#undef PIPE2_FILENAME
}

int My::Terminal::run(const std::string& raw_command){
    Commands_t commands;

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

        int retVal=execute(command, !setDaemon, from_myin, to_myout);
        if(from_myin) swapio();
        from_myin=to_myout=false;

        if(checkRetval && (bool(retVal)!=doubleAmpersand) ){
            return 1;
        }

        // pipeline
        if(isDelim==4) from_myin=true;
    }

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
    const bool useMyIn, const bool useMyOut
){
    pid_t pid=fork();

    if(pid==0){
        // pipelining
        if(useMyIn ) dup2( pipein(),  STDIN_FILENO);
        else close(pipein());
        if(useMyOut) dup2(pipeout(), STDOUT_FILENO);
        else close(pipeout());

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