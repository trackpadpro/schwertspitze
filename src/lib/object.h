#if !defined(SCHWERTSPITZE_OBJECT_H)
    #define SCHWERTSPITZE_OBJECT_H

    #include <vector>
    #include <atomic>
    #include <mutex>
    #include <GL/glew.h>
    #include "input.h"

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
            void input(const input::command& commandByte);
    };

    namespace menu
    {

    enum menuOption
    {
        play                = 0x00,
        keybinds            = 0x10,
        keybindSelection    = 0x11,
        fullscreen          = 0x20,
        volume              = 0x30,
        volumeSlider        = 0x31,
        exit                = 0x40,
        exitConfirm         = 0x41,
        hidden              = 0xff,
    };

    class Menu: public Object
    {
        public:
            Menu();
            void update();
            menuOption input(const input::command& commandByte);
        private:
            menuOption state;
    };

    } //namespace menu

    class Icon: public Object
    {
        public:

        private:

    };
#endif
