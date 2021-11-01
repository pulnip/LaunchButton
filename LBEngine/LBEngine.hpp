#ifndef __INC_LBENGINE_HPP
#define __INC_LBENGINE_HPP

namespace My{
    class LBEngine{
    public:
        using ElapsedTime=double;

    private:
        bool isActive=false;

        WINDOW *base;
        Size baseSize={0, 0};

        std::list<Button> buttons;

        std::ofstream log;

    public:
        LBEngine(){
            log.open("LBEngine.log");
        }
        ~LBEngine(){
            log.close();
        }

    private:
    /** @brief Main Engine Thread */
        void EngineThread(void);
        int OnCreate(void);
        int OnUpdate(ElapsedTime dt);
        int OnDestroy(void);

    public:
    /** @brief start the Engine */
        int start(void);
    };
}

#endif