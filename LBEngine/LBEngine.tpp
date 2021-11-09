#ifndef __INC_LBENGINE_TPP
#define __INC_LBENGINE_TPP

void My::LBEngine::EngineThread(void){
    if(OnCreate()) isActive=false;

    auto t1=std::chrono::system_clock::now();
    auto t2=std::chrono::system_clock::now();

    while(isActive){
        t2=std::chrono::system_clock::now();
        ElapsedTime dt=(t2-t1).count();
        t1=t2;

        if(OnUpdate(dt)) isActive=false;
    }

    if(OnDestroy()){ }
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
    echo();

    keypad(base, TRUE);
    mousemask(ALL_MOUSE_EVENTS|REPORT_MOUSE_POSITION, NULL);

    buttons.push_back(Button());

    prompt();
    (*buttons.begin()).draw();

    refresh();
    (*buttons.begin()).refresh();

    return 0;
}

int My::LBEngine::OnUpdate(LBEngine::ElapsedTime dt){    
    int key=wgetch(base);
    switch(key){
    case KEY_MOUSE:
        MEVENT mevent;
        if(getmouse(&mevent)==OK){
            #warning "LOGGING"
            log<<mevent.x<<' '<<mevent.y<<std::endl;
            break;
        }
    case KEY_RESIZE:
        isActive=false;
        return 0;
    case '\n':
        clear();

        prompt();
        for(Button &button:buttons){
            button.draw();
        }
    break;
    case KEY_BACKSPACE:
        printw("\b");
    }

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

    t.join();

    return 0;
}

int My::LBEngine::prompt(void){
    printw("[prompt]: ");
    
    return 0;
}

int My::LBEngine::click(MEVENT* mevent){
    auto itClickedButton=std::find_if(
        buttons.rbegin(), buttons.rend(),
        [mevent](const My::Button& b){
            My::Pos pos={mevent->x, mevent->y};
            return b.isInside(pos);
    });
    if(itClickedButton==buttons.rend()) return 1;
    
    #error "NO IMPL";
    // TODO
    // keyDown -> wait for moving/executing
    // keyDown & keyUp (pos not change)-> execute command
    // keyDown & keyUp (pos change) -> move button

    return 0;
}

#endif