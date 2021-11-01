#ifndef __INC_BUTTON_HPP
#define __INC_BUTTON_HPP

namespace My{
    class Button{
    private:
        static unsigned int newId;
    private:
        unsigned int id;

        WINDOW* window;
        Pos pos;
    public:
        void setPos(const int x, const int y);
        void move  (const int x, const int y){ setPos(pos.x+x, pos.y+y); }
        const Pos& getPos(void) const{ return pos; }
    private:
        Size size;
        Color color;
    public:
        void setColor(const short FG, const short BG);
    private:
        char title[12];
        char command[256];

    public:
        Button(
            const         char *title            , const         char *cmd               ,
            const unsigned int  width            , const unsigned int  height=1          ,
            const          int  x    =0          , const          int  y     =0          ,
            const        short  FG   =COLOR_WHITE, const        short  BG    =COLOR_BLACK
        )
        :window(newwin(height, width, y, x)),
        id(newId++), pos{x, y}, size{width, height}, color{FG, BG}{
            init_pair(id, FG, BG);
            wbkgd(window, COLOR_PAIR(id));

            strncpy(this->title, title, 11);
            this->title[11]='\0';
            strncpy(command, cmd, 255);
            this->command[255]='\0';
        }
        ~Button(){ delwin(window); }

        const int refresh(void) const;
        const int execute(void) const;

        const bool isInside(const Pos& thatPos) const;
    };
}

#endif