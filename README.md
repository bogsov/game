# Mastermind

This is a naive implementation of the Mastermind game.

## Build


```
cd game/
/usr/bin/cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang-8 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++-8  -Bbuild -G Ninja
cd build/
cmake ../
ninja mastermind
ninja mastermindCheat
ninja mastermindTest
```

## Possible TODOs
- More tests, maybe with mocks
- Multiplayer mode
- Some GUI