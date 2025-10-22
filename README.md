# CS 3503 bitboard game project
## Author
Jerod Gibbs

## Description
Bitboard checkers game that can be played local by 2 players or for playing against yourself.

## What ’s New
includes the ability to move pieces, promote pieces, win the game and reset the board to play another match if wanted.
there is no double jump implmentation.
Outside of the game itself some bit manipulation utils used in the creation of the game are included as well in the bitmanipulation class. 

## Build Instructions
‘‘‘bash
make
./game
‘‘‘

‘‘‘windows
gcc -o game gameboard.c bitmanipulation.c main.c
game.exe
‘‘‘

if you have bash on windows
make
./game.exe

## Testing the game
the game starts with black going first, red is also played by the user or by a friend enter values like a6 to move a piece you need to move diagonally.
