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

### How To Run Example Games
1. Go to UEngine folder and open UEngins.sln
2. Build UEngine library
3. Go back to previous folder, and go to Examples folder
4. Open Examples.sln
5. Build the game you want to play

Keys for all games are either wasd or arrow keys to move, and space for jump, '1' for resetting games
For puzzle game, '2' and '3' are disable/enable image of the puzzle you are moving

## Credit
- RunGame art credit by o_robster

## Library License
- DirectXTK (WICTextureLoader, DDSTextureLoader)<br/>Copyright (c) 2012-2021 Microsoft Corp
  The MIT License (MIT)
- ImGui<br/>The MIT License (MIT)
  Copyright (c) 2014-2021 Omar Cornut
