#pragma once

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#endif // _DEBUG

namespace UEngine
{
    namespace WinMemoryLeak
    {
#ifdef _DEBUG
        inline void Detect(long goToLine = -1)
        {
            //Also need this for memory leak code stuff
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
            _CrtSetBreakAlloc(goToLine); //Important!
        }
#else // _DEBUG
        inline void Detect(long goToLine = -1) {}
#endif
    } // namespace WMemoryLeak
}


