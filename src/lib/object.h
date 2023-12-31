#if !defined(SCHWERTSPITZE_OBJECT_H)
    #define SCHWERTSPITZE_OBJECT_H

    #include <vector>
    #include <GL/glew.h>

    class Object
    {
        public:
            Object(const std::vector<GLfloat>& objVertices);
        protected:
            std::vector<GLfloat> vertices;
    };
#endif
