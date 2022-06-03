# nmk-solver-minimax
### Simple program to solve [NMK game](https://en.wikipedia.org/wiki/M,n,k-game) made for educational purposes.
\
**Interface:**
\
`SOLVE_GAME_STATE N M K ActivePlayer` - solve game and print whether its a tie, first player's win or second player's win.
\
eg. SOLVE_GAME_STATE 3 3 3 2
\
1 0 0
\
0 0 0
\
0 0 0
\
`GEN_ALL_POS_MOV N M K ActivePlayer` - generate all possible moves.
\
`GEN_ALL_POS_MOV_CUT_IF_GAME_OVER N M K ActivePlayer` - generate all possible moves, but in case of finding a winning move, print just that one.

