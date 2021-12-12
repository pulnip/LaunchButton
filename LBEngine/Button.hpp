#ifndef __INC_BUTTON_HPP
#define __INC_BUTTON_HPP

namespace My{
    extern int logfd;

    class Button{
    private:
        static unsigned int newId;
        unsigned int id;

        WINDOW* window;
        Pos pos;

    public:
        void setPos(const int x, const int y);
        void move  (const int dx, const int dy){ setPos(pos.x+dx, pos.y+dy); }
        const Pos& getPos(void) const{ return pos; }

    private:
        Size size;
        Color color;
    public:
        void setColor(const short FG, const short BG);

    private:
        std::string title;
        std::string command;
    public:
        void setCommand(const std::string &cmd){ command=cmd; }
        const std::string& getCommand(void){ return command; }
        void setTitle(const std::string &_title){ title.clear(); title=_title; }
        const std::string& getTitle(void){ return title; }

    public:
        Button(
            const  std::string &title="BUTTON"   , const  std::string &cmd   ="echo Hello, World!",
            const unsigned int  width=13         , const unsigned int  height=3                   ,
            const          int  x    =5          , const          int  y     =2                   ,
            const        short  FG   =COLOR_BLACK, const        short  BG    =COLOR_WHITE
        )
        :id(newId++), pos{x, y}, size{width, height}, color{FG, BG},
        title(title), command(cmd),
        window(newwin(height, width, y, x)){
            init_pair(id, FG, BG);
            wbkgd(window, COLOR_PAIR(id));
            wborder(window, '|', '|', '-', '-', '*', 'x', '+', '+');
            mvwprintw(window, 1, 1, title.c_str()); 

            wrefresh(window);
        }
        Button(const Button &that)
        :id(newId++), pos(that.pos), size(that.size), color(that.color),
        title(that.title), command(that.command),
        window(newwin(size.height, size.width, pos.y, pos.x)){
            init_pair(id, color.FG, color.BG);
            wbkgd(window, COLOR_PAIR(id));
            wborder(window, '|', '|', '-', '-', '*', 'x', '+', '+');
            mvwprintw(window, 1, 1, title.c_str()); 

            wrefresh(window);
        }

        int close(void){ delwin(window); return 0; }
    
        int draw(void){
            wclear(window);
            wborder(window, '|', '|', '-', '-', '*', 'x', '+', '+');
            mvwprintw(window, 1, 1, title.c_str()); 

            return 0;
        }

        int refresh(void) {
            wrefresh(window);
            
            return 0;
        }

        bool isInside(const Pos& thatPos) const;
        bool isX(const Pos& thatPos) const;
    };
}

#endif // __INC_BUTTON_HPP