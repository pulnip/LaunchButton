#ifndef __INC_LBENGINE_TPP
#define __INC_LBENGINE_TPP

void My::LBEngine::EngineThread(void){
    if(OnCreate()){
        log("OnCreate() failed\n");
        isActive=false;
    }

    auto t1=std::chrono::system_clock::now();
    auto t2=std::chrono::system_clock::now();

    while(isActive){
        t2=std::chrono::system_clock::now();
        ElapsedTime dt=(t2-t1).count();
        t1=t2;

        if(OnUpdate(dt)) isActive=false;
    }

    if(OnDestroy()){
        log("OnDestroy() failed\n");
    }
}

int My::LBEngine::OnCreate(void){
    // Actually, Return value of initscr() is equal to "stdscr".
    base=initscr();
    getmaxyx(base, baseSize.height, baseSize.width);

    if(has_colors()==FALSE){
        puts("Terminal does not support colors");
        return 1;
    }
    start_color();
    noecho();

    keypad(base, TRUE);
    mousemask(ALL_MOUSE_EVENTS|REPORT_MOUSE_POSITION, NULL);

    Button &helloworld=buttons.emplace_back();
    
    prompt("");
    helloworld.draw();

    refresh();
    helloworld.refresh();

    return 0;
}

int My::LBEngine::OnUpdate(LBEngine::ElapsedTime dt){
    static std::string cmd;

    refresh();
    for(Button &button:buttons){
        button.refresh();
    }

    int key=wgetch(base);
    switch(key){
    case KEY_MOUSE:{
        MEVENT mevent;
        if(getmouse(&mevent)==OK){
#ifndef __RELEASE
            log(mevent.x); log(" "); log(mevent.y); log(" ");
            log(mevent.bstate&BUTTON1_CLICKED ); log(" ");
            log(mevent.bstate&BUTTON1_PRESSED ); log(" ");
            log(mevent.bstate&BUTTON1_RELEASED); log("\n");
#endif
            click(&mevent);
        }
    }
    break;
    case KEY_RESIZE:
        isActive=false;
        return 0;
    case '\n':
        evaluateCommand(cmd);
        cmd.clear();
    break;
    case KEY_BACKSPACE:
        if(cmd.size())
            cmd=cmd.substr(0, cmd.size()-1);
    break;
    default:
        cmd+=key;
    }

    clear();

    prompt(cmd);
    if((key=='\n') || (key==KEY_MOUSE)){
        std::string line;
        move(1, 0);
        while(std::getline(terminal.outbuffer, line, '\n')){
            printw((line+'\n').c_str());
        }
        terminal.outbuffer.clear();
    }
    for(Button &button:buttons){
        button.draw();
    }

    move(0, 10);

    refresh();
    for(Button &button:buttons){
        button.refresh();
    }

    return 0;
}

int My::LBEngine::OnDestroy(void){
    endwin();

    return 0;
}

int My::LBEngine::start(void){
    isActive=true;
    std::thread t(&LBEngine::EngineThread, this);
#ifndef __RELEASE
    log("Successfully create engine thread\n");
#endif
    t.join();
#ifndef __RELEASE
    log("Engine thread ends\n");
#endif
    return 0;
}

int My::LBEngine::prompt(const std::string &cmd){
    move(0, 0);
    printw("[prompt]: ");
    printw(cmd.c_str());
    
    return 0;
}

void My::LBEngine::makeButton(const MEVENT* const mevent){
    std::string title, cmd;
    size_t temp;
    
    clear();
    move(0, 0);
    printw("title? ");
    while(true){
        int key=wgetch(base);
        switch(key){
        case KEY_MOUSE:
        case KEY_RESIZE:
        case '\n':
        break;
        case KEY_BACKSPACE:
            if(title.size())
                title=title.substr(0, title.size()-1);
        break;
        default:
            title+=key;
        }
        if(key=='\n') break;

        clear();
        printw("title? ");
        printw(title.c_str());
        refresh();
    }

    if(!title.compare("exit")) return;

    clear();
    move(0, 0);
    printw("command? ");
    while(true){
        int key=wgetch(base);
        switch(key){
        case KEY_MOUSE:
        case KEY_RESIZE:
        case '\n':
        break;
        case KEY_BACKSPACE:
            if(cmd.size())
                cmd=cmd.substr(0, cmd.size()-1);
        break;
        default:
            cmd+=key;
        }
        if(key=='\n') break;

        clear();
        printw("command? ");
        printw(cmd.c_str());
        refresh();
    }

    Button& newButton=buttons.emplace_back();
    newButton.setCommand(cmd);
    newButton.setTitle(title);
    newButton.setPos(mevent->x, mevent->y);
}

int My::LBEngine::click(const MEVENT* const mevent){
    static auto before=buttons.rend();

    auto button=std::find_if(
        buttons.rbegin(), buttons.rend(),
        [mevent](const My::Button& b){
            My::Pos pos={mevent->x, mevent->y};
            return b.isInside(pos);
        }
    );

    // clicked (not press & release) -> execute command
    if(mevent->bstate&BUTTON1_CLICKED){
        if(button!=buttons.rend()){
            My::Pos pos={mevent->x, mevent->y};
            if(button->isX(pos)){
                buttons.erase(--(button.base()));
                before=buttons.rend();
            }
            else{
                terminal.run(button->getCommand());
            }
        }
        else{
            makeButton(mevent);
        }
    }
    // pressed -> wait for moving
    else if(mevent->bstate&BUTTON1_PRESSED){
        if(button!=buttons.rend()){
            before=button;
        }
    }
    // released (pos change) -> move button
    else if(mevent->bstate&BUTTON1_RELEASED){
        if(before!=buttons.rend()){
            before->setPos(mevent->x, mevent->y);
        }
    }

    return 0;
}

int My::LBEngine::evaluateCommand(const std::string &cmd){
    if(!cmd.compare("exit")){
        isActive=false;
        return 0;
    }
    else{
        std::vector<std::string> delims={"="};
        auto i=in(cmd, delims);
        if(i==0){
            terminal.addEnvArgs(toStringPair(cmd, '='));
        }
    }

    return terminal.run(cmd);
}

#endif // __INC_LBENGINE_TPP