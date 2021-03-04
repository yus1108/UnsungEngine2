# UnsungEngine2

### UE2 (UnsungEngine2) is the improved version of UnsungEngine, which I made 3 years ago.
- C++ based program
- Component based engine like Unity
- Designed in multi-threaded architecture to maximize frame rate as rendering and updating happen simultaneously.
- Implemented modular libraries like WinApplication, DXRendering, Utility (ThreadPool, Math, and Time)
- Implemented resource managers for multi-threaded game loop sequence
- Implemented DebugRenderer
- Implemented basic 2D collision check functions
- Implemented dynamic spatial partitioning and fixed time-rate update to optimize collision check
- Implemented image texture loading using WICTextureLoader and DDSTextureLoader from DirectXTK

The engine is built in ISO C++ 17 Standatd. If the engine doesn't work properly try to change language standard.

## Libraries License
- DirectXTK (WICTextureLoader, DDSTextureLoader) Copyright (c) 2012-2021 Microsoft Corp
