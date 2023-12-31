#if !defined(SCHWERTSPITZE_INPUT_H)
    #define SCHWERTSPITZE_INPUT_H

    #include <vector>
    #include <string>
    #include <map>
    #include <GLFW/glfw3.h>
    #include "isteaminput.h"

    class Input
    {
        public:
            Input(GLFWwindow* summoner, const std::string& cfg);
            char fetch();
            /*  ~           escape
                X           attack
                w           up
                a           left
                s           down
                d           right
                q           left+up
                z           left+down
                e           right+up
                c           right+down
                uppercase   +attack     */
        private:
            GLFWwindow* window;
            std::map<char, std::vector<short>> keyring;
            /*  0   escape
                1   attack
                w   up
                a   left
                s   down
                d   right   */
    };
#endif