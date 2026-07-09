All possible piece moves from a specific position on the board are finite and static. This sub-project examines the efficiency of pre-calculating all such possible "tours" for each piece at each position of the board, and determins if it is more efficient to use this dataset rather than calculate moves ad hoc.

For example, a bishop must remain on it's origial color - black or white - and can move diagonally any number of spaces. Here "any" can be no more than seven in any of the four possible directions.

```
  a  b  c  d  e  f  g  h
8 .  .  .  .  .  .  .  x 8                         
7 .  .  .  .  .  .  x  . 7                         
6 .  .  .  .  .  x  .  . 6                         
5 .  .  .  .  x  .  .  . 5                         
4 .  .  .  x  .  .  .  . 4                         
3 .  .  x  .  .  .  .  . 3                         
2 .  x  .  .  .  .  .  . 2                         
1 b  .  .  .  .  .  .  . 1                         
  a  b  c  d  e  f  g  h 
```

Tour for bishop origin a1
```
a1 -> b2 -> c3 -> d4 -> e5 -> f6 -> g7 -> h8
```
As a position can be packed into an octet with high nibble being rank and low nibble being file, then this tour can be stored in uint64.

```
  b     2    c   3     d    4   e      5   f    6    g    7    h    8   unused
|0010 0010|0011 0011|0100 0100|0101 0101|0110 0110|0111 0111|1000 1000|0000 0000|  => 0x2233445566778800ull
```

Note we don't store the origin square as the tour is indexed by piece type and origin square.

```
// Tour is a union that might provide easy octet indexing into the 64-bit quad-word. Need to experiment with compiler explorer to determine if
// this is better than just  auto p = (uint8_t*)&quadWord and ++p

union Tour {
   uint64_t qw;        // quad-word 64 bits
   uint8_t  oct[8];    // octet index into qw
};

= or =

typedef uint64_t Tour;

typedef std::map< Direction, Tour > TourVector;

typedef std::map< Position, TourVector > TourSet;

const std::map< PieceType, TourSet> AllTours;
```

But this is only for a given direction - Up-and-Right or DUR.
```
DUL  DUP  DUR
DL    +   DR
DDL  DDN  DDR
```
In this way a tour resembles a vector with direction and range.

So for each piece type, and for each position on the board, generate the vectors for that piece for each direction and that collection is called a tour.



Knights are Different


