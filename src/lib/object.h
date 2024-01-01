#if !defined(SCHWERTSPITZE_OBJECT_H)
    #define SCHWERTSPITZE_OBJECT_H

    #include <vector>
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
    };

    class Character: public Object
    {
        public:
            Character(const GLfloat& x, const GLfloat& y);
            virtual void update();
    };

    class Player: public Character
    {
        public:
            using Character::Character;
    };
#endif
