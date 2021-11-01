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
        void setColor(short FG, short BG);

        char title[12];
        char command[256];

    public:
        Button(
            const char*  title            , const char*  cmd               ,
            unsigned int width            , unsigned int height=1          ,
            int          x    =0          , int          y     =0          ,
            short        FG   =COLOR_WHITE, short        BG    =COLOR_BLACK
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

        int refresh(void) const;
        int click(void);
    };
}

#endif