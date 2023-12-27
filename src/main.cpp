#include <vector>
#include <thread>
#include <mutex>
#include "steam_api.h"

int main()
{
    if(SteamAPI_RestartAppIfNecessary(480))
    {
        return 1;
    }
    

    return 0;
}
