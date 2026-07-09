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
//
// after some twiddling with CE https://godbolt.org/z/dqshcz3Mq it looks like the pointer increment route is way more efficient. Will need
// to look at this for some of the moby unions currently in reoce.

//union Tour {
//   uint64_t qw;        // quad-word 64 bits
//   uint8_t  oct[8];    // octet index into qw
//};
//
//= or =

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

enum Direction {
   DUL = 0,
   DUP,
   DUR,
   DL,
   DR,
   DDL,
   DDN,
   DDR,
   DIR_CNT
}

```
In this way a tour resembles a vector with direction and range.

So for each piece type, and for each position on the board, generate the vectors for that piece for each direction and that collection is called a tour.

A more complex bishop tour from the center of the board would be:

```
  a  b  c  d  e  f  g  h
8 .  .  .  .  .  .  .  x 8                         
7 x  .  .  .  .  .  x  . 7                         
6 .  x  .  .  .  x  .  . 6                         
5 .  .  x  .  x  .  .  . 5                         
4 .  .  .  b  .  .  .  . 4                         
3 .  .  x  .  x  .  .  . 3                         
2 .  x  .  .  .  x  .  . 2                         
1 x  .  .  .  .  .  x  . 1                         
  a  b  c  d  e  f  g  h 

AllTours: [
   PIECE_BISHOP: TourSet [
      D4: TourVector[
         DUL: 0x3526170000000000ull,   // c5 -> b6 -> a7
         DUR: 0x5566778800000000ull,   // e5 -> f6 -> g7 -> h8
         DDL: 0x3322110000000000ull,   // c3 -> b2 -> a1
         DDR: 0x5362710000000000ull,   // e3 -> f2 -> g1
      ]
   ]
]
```
But this seems a little redundant. The vector that goes from a1 -> h8 can be used for any diagonally moving piece - including queens - and it seems that what matters
is where the vectors intersect that allows utility. So if there are two vectors:

v1: a1->b2->c3->d4->e5->f6->g7->h8
v2: a7->b6->c5->d4->e3->f2->g1

And bishop is at d4, then we can use that as a starting point in each of the vectors and work outwards toward the board edges.

How to determine the intersect point? v1[3] == v2[3] is a coincidence in this case and does not hold for other intersecting vectors. The point of intersection - of course - is where
a position in v1 and v2 are the same. But perhaps that's the solution. Instead of storing four vectors - one for each possible direction - we store two rays and give the intersection point,
or for a given position, specify the two rays that apply and the indexes into each ray where the intersection is. So:
```
d5: v1:3, v2:3
```
Bishops and rooks have two rays, but queens and kins have eight (which can just be the union of the rook and bishop rays at a given location.)

The importance of working outward is that we need to determine the closest neighbor in each direction to accumulate move and capture possibilities. 
We can determine the Manhatten distance between any two pieces efficiently, so maybe it's better to just work from one end of the ray to the other, 
keep track of the closest pieces encountered. We still need to know the approach vector, and will have to toggle it once we pass our own position, but
introduces more problems thatn it solves. For one, when moving toward the subject piece, we need to keep track of the closest piece, but when moving
away the first piece we encounter will always be the closest piece, so no need to look further. Also, we need to know about proximate empty spaces
which the subject piece could move to, so this approach will not work. We will still need to start from the peice and work outwards.

Store all possible rays, and for a given position, pull all rays that contain the position and then split each into two vectors at the position.
But how is this better than storing them split as above? It saves some memory, but maybe not much. Perhaps generate the dataset for the above 
solution and then see if optimizations can be made.



Knights are Different


