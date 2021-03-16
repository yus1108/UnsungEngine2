# UnsungEngine2

### UE2 (UnsungEngine2) is the improved version of UnsungEngine, which I made 3 years ago.
- C++ based program
- Component based engine like Unity
- Designed in multi-threaded architecture to maximize frame rate as rendering and updating happen simultaneously.
- Implemented modular libraries like WinApplication, DXRendering, Utility (ThreadPool, Math, and Time)
- Implemented resource managers for multi-threaded game loop sequence and for reusability
- Implemented DebugRenderer
- Implemented basic 2D collision check functions
- Implemented dynamic spatial partitioning and fixed time-rate update to optimize collision check
- Implemented image texture loading using WICTextureLoader and DDSTextureLoader from DirectXTK
- Added example games (air hockey, puzzle, run game, shooting game)
- Integrated GUI library(ImGui) to make editor
- Integrated User Custom Component Script system
- Integrated Scene save and load using xml files and custom serializer and serializedType classes and using TinyXML library

The engine is built in ISO C++ 17 Standatd. If the engine doesn't work properly try to change language standard.

## Credit
- RunGame art credit by o_robster

## Library License
- DirectXTK (WICTextureLoader, DDSTextureLoader)<br/>Copyright (c) 2012-2021 Microsoft Corp
  The MIT License (MIT)
- ImGui<br/>The MIT License (MIT)
  Copyright (c) 2014-2021 Omar Cornut
