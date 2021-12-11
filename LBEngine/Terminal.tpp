#ifndef __INC_TERMINAL_TPP
#define __INC_TERMINAL_TPP

My::Terminal::Terminal(){
    pipe(pipefd[0]);
    pipe(pipefd[1]);

    loadEnvArgs();
}

My::Terminal::~Terminal(){
    saveEnvArgs();
}

int My::Terminal::run(const std::string& raw_command){
    Commands_t commands;

    std::vector<std::string> delims={";", "&&", "&", "||", "|"};

    auto raw_commands=toList(raw_command, delims, true);
    raw_commands.push_back(";"); // semi-colon ended for last command.
    
    for(auto &command: raw_commands){
        commands.push_back(toList(command));
    }
    
    auto cmd=commands.begin();
    bool _out=false;
    for(auto it=commands.begin(); it!=commands.end(); ++it){
        int isDelim=in(it->front(), delims);
        bool doubleAmpersand=false;
        bool checkRetval=false;
        bool setDaemon=false;

        bool _in=_out;
        switch(isDelim){
        case -1:
            cmd=it;
            continue;
        case 1: // "&&"
            doubleAmpersand=true;
        case 3: // "||"
            checkRetval=true;
            break;
        case 2: // "&"
            setDaemon=true;
            break;
        case 4: // "|"
            _out=true;
        }

        int retVal=execute(*cmd, !setDaemon, _in, _out);
        if(isDelim!=4){
            flushpipe();
        }
        else swapio();
        
        if( checkRetval && (bool(retVal)!=doubleAmpersand) ){
            return 1;
        }
    }

    return 0;
}

void My::Terminal::loadEnvArgs(void){
    int fd=open("myterm.env", O_CREAT|O_RDONLY);
    
#define BUFFER_SIZE 255
    char buffer[BUFFER_SIZE+1];
    std::string line;
    while(true){
        const std::size_t n_read=read(fd, buffer, BUFFER_SIZE);
        buffer[n_read]='\0';

        int first=0;
        const std::size_t end=n_read+1;
        for(int i=0; i<end; ++i){
            if(buffer[i]=='\n'){
                buffer[i]='\0';
                if(i-first){
                    // line+=buffer[first: i]
                    line+=buffer+first;
                    
                    envargs.emplace(toStringPair(line, '='));
                    line.clear();
                    
                    first=i+1;
                }
            }
            else if(buffer[i]=='\0'){
                if(i-first){
                    line+=buffer+first;
                }
            }
        }

        if(n_read!=BUFFER_SIZE){
            // EOF
            if(!line.empty()) envargs.emplace(toStringPair(line, '='));
            break;
        }
    }

    close(fd);
}

void My::Terminal::saveEnvArgs(void){
    int fd=open("myterm.env", O_WRONLY|O_CREAT|O_TRUNC);

    const std::size_t envarg_num=envargs.size();
    for(auto& envarg: envargs){
        std::string raw=envarg.first+'='+envarg.second+'\n';
        write(fd, raw.c_str(), raw.size());
    }

    close(fd);
}

My::Terminal::Args_t My::Terminal::preprocessing(const Command_t &command){
    Args_t args;

    for(auto it=command.cbegin(); it!=command.cend(); ++it){
        if(!(*it).compare("<")){
            // redirect
            int fd=open((++it)->c_str(), O_RDONLY);
            log(it->data());
            dup2(fd, STDIN_FILENO);
        }
        else if(!(*it).compare(">")){
            // redirect to 
            int fd=open((++it)->c_str(), O_CREAT|O_WRONLY);
            log(it->data());
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
    const bool _in, const bool _out
){
    pid_t pid=fork();

    if(pid==0){
        // pipelining
        if(_in) dup2(myin()[0], STDIN_FILENO);
        else close(myin()[0]);
        if(_out) dup2(myout()[1], STDOUT_FILENO);
        else close(myout()[1]);

        Args_t args=preprocessing(command);
        CStyleArray wrapped(args);

        if(!isWait) daemon(0, 0);

        log(wrapped[0]);
        if(execvp(wrapped[0], const_cast<char* const*>(wrapped.data()))<0){
            log("Terminal::execute failed\n");
            exit(-1);
        }
    }
    else if(pid==-1){
        log("fork() failed\n");
        return -1;
    }

    if(!isWait) return pid;

    int status=0;
    waitpid(pid, &status, 0);
    return status;
}

#endif // __INC_TERMINAL_TPP