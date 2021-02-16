#pragma once

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

namespace UEngine
{
    namespace WinMemoryLeak
    {
        inline void Detect(long goToLine = -1)
        {
            //Also need this for memory leak code stuff
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
            _CrtSetBreakAlloc(goToLine); //Important!
        }
    } // namespace WMemoryLeak
}


#endif // _DEBUG
