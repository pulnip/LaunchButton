#ifndef __INC_LBENGINE_HPP
#define __INC_LBENGINE_HPP

namespace My{
    class LBEngine{
    public:
        using ElapsedTime=double;

    private:
        int OnCreate(void);
        int OnUpdate(ElapsedTime dt);
        int OnDestroy(void);

    private:
        bool isActive=false;

        WINDOW *base;
        Size baseSize={0, 0};

        std::list<Button> buttons;

    public:
    /** @brief start the Engine */
        int start(void);
    /** @brief Main Engine Thread */
        void EngineThread(void);
    };
}

#endif