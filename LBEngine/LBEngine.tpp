#ifndef __INC_LBENGINE_TPP
#define __INC_LBENGINE_TPP

int My::LBEngine::start(void){
    isActive=true;
    std::thread t(&LBEngine::EngineThread, this);

    t.join();

    return 0;
}

void My::LBEngine::EngineThread(void){
    if(OnCreate()){
        isActive=false;
    }

    auto t1=std::chrono::system_clock::now();
    auto t2=std::chrono::system_clock::now();

    while(isActive){
        t2=std::chrono::system_clock::now();
        ElapsedTime dt=(t2-t1).count();
        t1=t2;

        if(OnUpdate(dt)){
            isActive=false;
        }
    }

    if(OnDestroy()){}
}

int My::LBEngine::OnCreate(void){
    base=initscr();
    getmaxyx(base, baseSize.height, baseSize.width);

    if(has_colors()==FALSE){
        puts("Terminal does not support colors");
        return 1;
    }
    start_color();
    noecho();

    refresh();

    return 0;
}

int My::LBEngine::OnUpdate(LBEngine::ElapsedTime dt){
    std::for_each(buttons.begin(), buttons.end(), [](const Button& b){
        b.refresh();
    });
}

int My::LBEngine::OnDestroy(void){
    endwin();

    return 0;
}

#endif