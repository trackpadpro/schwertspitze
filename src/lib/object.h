#if !defined(SCHWERTSPITZE_OBJECT_H)
    #define SCHWERTSPITZE_OBJECT_H

    #include <vector>
    #include <atomic>
    #include <mutex>
    #include <GL/glew.h>

    class Object
    {
        public:
            Object();
            Object(const std::vector<GLfloat>& objVertices);
            virtual std::vector<GLfloat> getVertices();
            virtual void update();
        protected:
            std::vector<GLfloat> vertices;
            std::atomic<bool> needsUpdate;
            std::mutex mutObj;
            float x, y, dx, dy, ddx, ddy;
    };

    class Character: public Object
    {
        public:
            Character(const GLfloat& xpos, const GLfloat& ypos);
            virtual void update();
    };

    class Player: public Character
    {
        public:
            using Character::Character;
            void input(const char& commandBit);
    };

    enum menuOption
    {
        mPlay = 0,
        mKeybinds = 10,
        mKeybindSelection = 11,
        mFullscreen = 20,
        mVolume = 30,
        mVolumeSlider = 31,
        mExit = 40,
        mExitConfirm = 41,
        mHidden = 99,
    };

    class Menu: public Object
    {
        public:
            Menu();
            void update();
            char input(const char& commandBit);
            /*  ~   exit
                p   play
                f   fullscreen  */
        private:
            menuOption state;
    };

    class Icon: public Object
    {
        public:

        private:

    };
#endif
