#ifndef __INC_BUTTON_TPP
#define __INC_BUTTON_TPP

unsigned int My::Button::newId=1;

void My::Button::setColor(const short FG, const short BG){
    color.FG=FG;
    color.BG=BG;

    init_pair(id, FG, BG);

    wbkgd(window, COLOR_PAIR(id));
}

const int My::Button::refresh(void) const{
    wrefresh(window);

    return 0;
}

const int My::Button::execute(void) const{
    return 0;
}

#endif