# ROECE
Ridiculously Over-Engineered Chess Engine

## Authority
The behavior of this chess engine is bound by the United States Chess Federation (USCF) Offical Rules of Chess, 7th ed., as amended for 2024. https://www.uschess.org

Rules are cited in the code.

## On-Move vs. Have-Move
According to rule 6B the player "on-move" is the side that has just completed their move. For our purposes, the side that "has the move" or "has-move" means the side that is about to move. 

## Search Spaces
The search space for a chess game is gargantuan, but finite. For purposes of generating unique positions, the verification search space can be segregated as follows:
1. Pawn moves/promotions and piece captures result in a unique "root" position that cannot be encountered again in the current search space.
2. A given position can be further segregated by "has-move" status. That is, all positions with white "has-move" cannot occur in the search space of black "has-move" because in the former all generated positions result from white moves, and the latter by black moves. This is also true for generated root positions.
3. Game clocks should not be considered for the uniquness of a position.
4. en-passant is not indivually considered, since it results from a pawn move and is already a unique root position.
5. Only pieces for the has-move side are considered.

So a search space can be divided into the following tables:
1. White have-move root positions
2. White have-move positions
3. Black have-move root positions
4. Black have-move positions

This limits the size of the sub-search spaces to check a given position for collisions. 

Roots must be globally scrutinized, as roots created in one search space can occur in another search space. Consider:
1. a3 d6
2. d3 a6
   
Generates the same root position:
1. d3 a6
2. a3 d6
   
And are identical, and that the board position is the same, and the white is on-move. This can be segregated by piece count for each side. Both of the positions above have 16 black and 16 white pieces on board (1616), but if the piece count were 1615, then would not be comparable. So we can divide root positions into "hives" which can be searched based on piece count. Since neither side can have zero pieces, we only need consider ranges of 16..1, which are 15 possible counts per side, so 15^2 (225) hives are required. This might be simplified by using 30 pools - total piece count - but testing will prove this out. From above observations, this must
be doubled to account for have-move sides.

hive 2..32 black
     2..32 white

roots - white - 2..32 - hash table
        black - 2..32 - hash table



```
add position p to work{}
set white_root_space{} empty
set white_space{} empty
set black_root_space{} empty
set black_space{} empty
while work{} is not empty:
    p <- work.front
    add p to have-move space{}
    for each have-move piece:
        for each legal move m for that piece:
            apply m to p to generate p'
            if m results in a root position:
                if p' is not in have-move root_space:
                    add p' to have-move root_space
            else
                if p' is not in have-move space:
                    add p' to have-move space
                    add p' to work{}
                endif
            endif
        endfor
    endfor
endwhile
```
## Cross Reference
Edges between positions (p and p') are needed to maintain progress of the game. Some positions have as high as 80 moves
- for p a flat file contains the move and p'
- for p' a flat file contains the move and p


p  --> move | p' --> p'
p' --> move | p  --> p

# Search Space Zero
FEN: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

| Stat | Metric |
| ---- | -----: |
| Maximum Moves in a position      |          34|
| Minimum                          |          16|
| Legal moves in this position     | 145'937'007|
| Number of white root collisions  |   5'656'866|
| Number of white domain collisions|  22'312'637|
| Number of black root collisions  |   5'016'112|
| Number of black domain collisions|  22'312'785|
| Number of new roots              |  85'622'011|
| Number of new positions          |   5'656'865|

Running time:
* real    91m31.923s
* user    258m42.208s
* sys     188m4.779s

# Search Space One
FEN: "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1"

* Maximum Moves in a position      :
* Minimum                          :
* Legal moves in this position     :
* Number of white root collisions  :
* Number of white domain collisions:
* Number of black root collisions  :
* Number of black domain collisions:
* Number of new roots              :
* Number of new positions          :

Running time:
* real
* user
* sys

