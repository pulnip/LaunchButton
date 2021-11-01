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
        Size size;
        Color color;
        void setColor(const short FG, const short BG);

        char title[12];
        char command[256];

    public:
        Button(
            const char*  title                  , const char*  cmd                     ,
            const unsigned int width            , const unsigned int height=1          ,
            const int          x    =0          , const int          y     =0          ,
            const short        FG   =COLOR_WHITE, const short        BG    =COLOR_BLACK
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

        const int refresh(void) const;
        const int execute(void) const;
    };
}

#endif