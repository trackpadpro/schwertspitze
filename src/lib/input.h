#if !defined(SCHWERTSPITZE_INPUT_H)
    #define SCHWERTSPITZE_INPUT_H

    #include <vector>
    #include <string>
    #include <map>
    #include <GLFW/glfw3.h>
    #include "isteaminput.h"

namespace input
{

enum command
{
    none        = 0x00,
    up          = 0x01,
    down        = 0x02,
    left        = 0x04,
    upLeft      = up | left,
    downLeft    = down | left,
    right       = 0x08,
    upRight     = up | right,
    downRight   = down | right,
    attack      = 0x10,
    aUp         = attack | up,
    aDown       = attack | down,
    aLeft       = attack | left,
    aUpLeft     = attack | upLeft,
    aDownLeft   = attack | downLeft,
    aRight      = attack | right,
    aUpRight    = attack | upRight,
    aDownRight  = attack | downRight,
    escape      = 0xff,
};
    
class Input
{
    public:
        Input(GLFWwindow* summoner, const std::string& cfg);
        command fetch();
    private:
        GLFWwindow* window;
        std::map<command, std::vector<short>> keyring;
};

} //namespace input
#endif