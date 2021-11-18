#ifndef __INC_BUTTON_HPP
#define __INC_BUTTON_HPP

namespace My{
    extern int logfd;

    class strvecWrapper{
    private:
        size_t n;
        const char** pT;

    public:
        strvecWrapper()=default;
        strvecWrapper(const std::vector<std::string>& that)
        :n(that.size()), pT(new const char*[n+1]){
            for(int i=0; i<n; ++i){
                pT[i]=that[i].data();
            }
            pT[n]=NULL;
        }
        strvecWrapper& operator=(const std::vector<std::string>& that){
            delete[] pT;
            n=that.size();
            pT=new const char*[n+1];
            for(int i=0; i<n; ++i){
                pT[i]=that[i].data();
            }
            pT=NULL;

            return *this;
        }
        strvecWrapper(const strvecWrapper &that)
        :n(that.n), pT(new const char*[n+1]){
            for(int i=0; i<n; ++i){
                pT[i]=that[i];
            }
            pT[n]=NULL;
        }
        strvecWrapper operator=(const strvecWrapper &that){
            if(this!=&that){
                delete[] pT;
                n=that.n;
                pT=new const char*[n+1];
                for(int i=0; i<n; ++i){
                    pT[i]=that[i];
                }
                pT[n]=NULL;
            }

            return *this;
        }
        const char** data(void) const{ return pT; }
        const char*& operator[](int idx) const{ return pT[idx]; }
        ~strvecWrapper(){ delete[] pT; }
    };
    std::vector<std::string> split(const std::string &str, char delim=' ');

    class Button{
    private:
        static unsigned int newId;
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
        std::string title;
        std::string command;
    public:
        void setCommand(const std::string &cmd){ command=cmd; isCmdChanged=true; }
    private:
        bool isCmdChanged=false;
        std::vector<std::string> args;
        strvecWrapper wrapped;

    public:
        Button(
            const  std::string &title="BUTTON"   , const  std::string &cmd   ="echo Hello, World!",
            const unsigned int  width=13         , const unsigned int  height=3                   ,
            const          int  x    =5          , const          int  y     =2                   ,
            const        short  FG   =COLOR_BLACK, const        short  BG    =COLOR_WHITE
        )
        :window(newwin(height, width, y, x)),
        id(newId++), pos{x, y}, size{width, height}, color{FG, BG},
        title(title), command(cmd), args(split(cmd)), wrapped(args){
            init_pair(id, FG, BG);
            wbkgd(window, COLOR_PAIR(id));
            wborder(window, '|', '|', '-', '-', '*', 'x', '+', '+');
            mvwprintw(window, 1, 1, title.c_str()); 

            wrefresh(window);
        }
        Button(const Button &that){
            #warning "Not Impl"
        }

        int close(void){ delwin(window); return 0; }
    
        int draw(void){
            wborder(window, '|', '|', '-', '-', '*', 'x', '+', '+');
            mvwprintw(window, 1, 1, title.c_str()); 

            return 0;
        }

        int refresh(void) {
            wrefresh(window);
            
            return 0;
        }
        int execute(void);

        bool isInside(const Pos& thatPos) const;
    };
}

#endif