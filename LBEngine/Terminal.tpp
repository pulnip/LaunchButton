#ifndef __INC_TERMINAL_TPP
#define __INC_TERMINAL_TPP

My::Terminal::Terminal(){
    loadEnvArgs();
}

My::Terminal::~Terminal(){
    saveEnvArgs();
}

int My::Terminal::run(const std::string& raw_command){
    Commands_t commands;

    std::vector<std::string> delims={";", "&&", "&", "||", "|"};

    auto raw_commands=toList(raw_command, delims, true);
    // semi-colon ended for last command.
    if(in(raw_commands.back(), delims)==-1){
        raw_commands.push_back(";");
    }
    for(auto &command: raw_commands){
        commands.push_back(toList(command));
    }
    
    auto cmd=commands.begin();
    bool pipeline=false;
    for(auto it=commands.begin(); it!=commands.end(); ++it){
        int isDelim=in(it->front(), delims);
        bool doubleAmpersand=false;
        bool checkRetval=false;
        bool setDaemon=false;

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
        }

        // make out stream for new program.
        pipe2(myout(), O_CLOEXEC);
        int retVal=execute(*cmd, !setDaemon, pipeline);
        
        if(isDelim!=4){
            flushpipe();
            closeout();
            pipeline=false;
        }
        else{
            swapio();
            pipeline=true;
        }
        
        if( checkRetval && (bool(retVal)==doubleAmpersand) ){
            return 1;
        }
    }

    return 0;
}

void My::Terminal::loadEnvArgs(void){
    int fd=open("myterm.env", O_CREAT|O_RDONLY, 0664);
    
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
    int fd=open("myterm.env", O_WRONLY|O_CREAT|O_TRUNC, 0664);

    const std::size_t envarg_num=envargs.size();
    for(auto& envarg: envargs){
        std::string raw=envarg.first+'='+envarg.second+'\n';
        write(fd, raw.c_str(), raw.size());
    }

    close(fd);
}

My::Terminal::Args_t My::Terminal::preprocessing(const Command_t &command){
    Args_t args;
    std::list<std::string> delim={"#"};

    for(auto it=command.cbegin(); it!=command.cend(); ++it){
        // redirect
        if(!(*it).compare("<")){
            int fd=open((++it)->c_str(), O_RDONLY);
            dup2(fd, STDIN_FILENO);
        }
        else if(!(*it).compare(">")){
            int fd=open((++it)->c_str(), O_CREAT|O_WRONLY, 0664);
            dup2(fd, myout()[1]);
        }
        else if(in(it->substr(0, 1), delim)==0){
            std::string key=strip(*it, "#{}");
#ifndef __RELEASE
            log((key+'\n').c_str());
#endif
            auto _it=envargs.find(key);

            if(_it != envargs.end()){
                args.push_back(_it->second);
            }
            else args.push_back(*it);
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
    const bool pipeline
){
    pid_t pid=fork();

    if(pid==0){
        Args_t args=preprocessing(command);
        // normal process
        if(isWait){
            // pipelining
            if(pipeline) dup2(myin()[0], STDIN_FILENO);
            else close(myin()[0]);
            dup2(myout()[1], STDOUT_FILENO);
        }
        // daemon process
        else{
            char* pwd=getcwd(NULL, 0);
            std::string oldpath=std::string(pwd)+'/';
            args.front()=oldpath+args.front();
            free(pwd);

            pid_t pid1=fork();
            if(pid1<0){
                log("Daemonize: fork() failed\n");
                exit(-1);
            }
            // terminate parent process
            else if(pid1>0) exit(0);

            My::closeLog();
            My::initLog(oldpath);
#ifndef __RELEASE
            log("Daemonize: fork() success\n");
#endif
            // new session
            if(setsid()<0){
                log("Daemonize: setsid() failed\n");
                exit(-1);
            }
#ifndef __RELEASE
            log("Daemonize: setsid() success\n");
#endif
            // ignore signals
            signal(SIGCHLD, SIG_IGN);
            signal(SIGHUP, SIG_IGN);

            // unmask the file mode
            umask(0);

            // change cwd to root dir
            chdir("/");

            // close all std fd
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
#ifndef __RELEASE
            log("successfully daemonized\n");
#endif
        }

        CStyleArray wrapped(args);

#ifndef __RELEASE
        log(wrapped[0]); log("\n");
#endif
        if(execvp(wrapped[0], const_cast<char* const*>(wrapped.data()))<0){
            log("Terminal::execute failed\n");
            exit(-1);
        }
    }
    else if(pid==-1){
        log("fork() failed\n");
        return -1;
    }
    closein();

    if(!isWait) return 0;

    int status=0;
    waitpid(pid, &status, 0);
#ifndef __RELEASE
    log(status); log("\n");
#endif
    return status;
}

#endif // __INC_TERMINAL_TPP