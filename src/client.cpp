#include <vector>
#include <thread>
#include <mutex>
#include "steam_api.h"

#include <iostream>

int main()
{
    if(SteamAPI_RestartAppIfNecessary(480))
    {
        std::cout<<"failed\n";
        return 1;
    }
    

    return 0;
}
