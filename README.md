# ROECE
Ridiculously Over-Engineered Chess Engine

## Authority
The behavior of this chess engine is bound by the United States Chess Federation (USCF) Offical Rules of Chess, 7th ed., as amended for 2024. https://www.uschess.org

Rules are cited in the code.


## Search Spaces

The search space for a chess game is enormouse. By some estimations the number of possible chess positions is 

For a given position, each piece is examined for legal moves. Each move is applied to the current position to generate a new position.


```
add position p to work{}
set space{} empty
while work{} is not empty:
   p <- work.front
   add p to space{}
   for each piece:
      for each legal move for that piece:
         apply move to p to generate p'
         if p' is not in space{}
            if p' is a new root position:
                add p' to root{}
            else
                add p' to work{}
```

As positions are generated, the search space is queried to see if the position has been encountered before. This is a repeat (or loop) and is not considered further.

A search space is a set of positions where none of the following occurs:
- a pawn move
- a piece capture or promotion

These two events create a new search space where no position in the "parent" search space can possiblly occur, so each search space can be traversed in isolation.


## Cross Reference

Edges between positions (p and p') are needed to maintain progress of the game. Some positions have as high as 80 moves
- for p a flat file contains the move and p'
- for p' a flat file contains the move and p


p  --> move | p' --> p'
p' --> move | p  --> p


