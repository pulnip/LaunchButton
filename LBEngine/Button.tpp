#ifndef __INC_BUTTON_TPP
#define __INC_BUTTON_TPP

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

bool My::Button::isInside(const Pos& thatPos) const{
    return (pos.x<=thatPos.x) && (thatPos.x<=pos.x+size.width) &&
           (pos.y<=thatPos.y) && (thatPos.y<=pos.y+size.height);
}

bool My::Button::isX(const Pos& thatPos) const{
    return ((pos.x+size.width-1)==thatPos.x) &&
            (pos.y==thatPos.y);
}

#endif // __INC_BUTTON_TPP