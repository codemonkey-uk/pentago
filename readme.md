# Pentago

## Statement Of Intent

In this project I will explore two complementary research spaces relating to the board game Pentago.

https://en.wikipedia.org/wiki/Pentago

This game is a two player game of 5 in a row played on a 6x6 grid,w here the 6x6 grid is made of a 2x2 box of 3x3 spaces.

I will explore data structures and algorithms for efficient storage and evaluation of the game state.

And I will explore approaches for implementing AIs for the game.

Each board space can be in one of three states: empty / black / white.

On each turn an empty space becomes black or white (according the turn).
Then one of the 4 3x3 grids is rotated 90 degrees in either direction.

## Storing Game State

The game is won by the player who forms a line of 5 in a row of their colour, horizontally, vertically, or diagonally.
```
empty: 00
black: 01
white: 10
(invalid state: 11)

2 bits per space. 

by corner (4 lots of 3x3): 
2x9 = 18 bits = 3x bytes, 24 bit.
 x4 = 12 bytes

line (6 rows of 6):
2x6 = 12 bits  = 2x bytes, 16 bit.
 x6 = 12 bytes
```
My initial instinct on this is to store moves in a byte array. I will explore both quadrant and line based representations.

Note: Although the game is solved, with 3,009,081,623,421,558 possible board configurations, at 12b per board, it would take over 32841TB of memory to store a full move database.

## Storing Moves

Each move must be a location on the board. The colour is determined by the turn order.
Every move must include a rotation, one of 4 quadrants.
```
6x6 grid, 36 positions, 6 bits stores the location.
4 quadrants, 2 directions of rotation, 8 rotations, 3 bits of rotation.
9 bits per move (with turn implied, 10 bits to store player).
```
My initial instinct on this is to store moves in a uint_16.

