#ifndef __INC_LBENGINE_HPP
#define __INC_LBENGINE_HPP

namespace My{
    class LBEngine{
    public:
        using ElapsedTime=double;

    private:
        bool isActive=false;

        WINDOW *base=nullptr;
        Size baseSize={0, 0};

        std::list<Button> buttons;

    public:
        LBEngine(){ log("Engine Created\n"); }
        ~LBEngine(){ log("Engine Closed\n"); }

    private:
    /** @brief Main Engine Thread */
        void EngineThread(void);
        int OnCreate(void);
        int OnUpdate(ElapsedTime dt);
        int OnDestroy(void);

        int prompt(void);
        int click(MEVENT *mevent);

    public:
    /** @brief start the Engine */
        int start(void);
    };
}

#endif