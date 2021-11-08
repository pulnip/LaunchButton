#ifndef __INC_BUTTON_TPP
#define __INC_BUTTON_TPP

std::vector<std::string> My::split(const std::string &str, char delim=' '){
    std::vector<std::string> result;

    std::stringstream ss(str+delim);
    std::string temp;
    while(std::getline(ss, temp, delim)){
        result.push_back(temp);
    }

    return result;
}

unsigned int My::Button::newId=1;

void My::Button::setPos(const int x, const int y){
    pos.x=x;
    pos.y=y;

    mvwin(window, y, x);
}

void My::Button::setColor(const short FG, const short BG){
    color.FG=FG;
    color.BG=BG;

    init_pair(id, FG, BG);
    wbkgd(window, COLOR_PAIR(id));
}

int My::Button::refresh(void) const{
    wrefresh(window);

    return 0;
}

int My::Button::execute(void){
    if(isCmdChanged){
        auto _args=split(command);
        std::copy(
            _args.begin(), _args.end(),
            args.begin()
        );
        wrapped=args;
    }
    pid_t pid=fork();
    if(pid==0){
        if(execvp(wrapped[0], const_cast<char* const*>(wrapped.data()))<0){
            std::cerr<<"execvp() error!"<<std::endl;
        }
    }
    return 0;
}

bool My::Button::isInside(const Pos& thatPos) const{
    return (pos.x<=thatPos.x) && (thatPos.x<=pos.x+size.width) &&
           (pos.y<=thatPos.y) && (thatPos.y<=pos.y+size.height);
}

#endif