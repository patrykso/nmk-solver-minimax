#include <iostream>
#include <cstring>

class GameState {
public:
    int n, m, k, activePlayer, otherPlayer;
    int **board;
    GameState(int n, int m, int k, int activePlayer) {
        this->n = n; //rows
        this->m = m; //columns
        this->k = k;
        this->activePlayer = activePlayer;
        if (activePlayer == 1) this->otherPlayer = 2;
        else otherPlayer = 1;

        board = new int * [n];
        if(board) {
            for(int i = 0; i < n; i++) board[i] = new int[m];
        }

    }
    ~GameState() {
        for(int i = 0; i < n; i++) delete[] board[i];
        delete[] board;
    }
};

void genAllPosMov(int n, int m, int k, int activePlayer);
void genAllPosMovCutIfGameOver(int n, int m, int k, int activePlayer);
void solveGameState(int n, int m, int k, int activePlayer);
void readState(GameState &game);
void printBoardMod(GameState &game, int x, int y);
bool checkWin(GameState &game, int i, int j, bool invertPlayer);
bool searchUpDown(GameState &game, int i, int j, int activePlayer);
bool searchLeftRight(GameState &game, int i, int j, int activePlayer);
bool searchDiagonalDownUp(GameState &game, int i, int j, int activePlayer);
bool searchDiagonalUpDown(GameState &game, int i, int j, int activePlayer);
int minMax(GameState &game, int activePlayer);
int isGameOver(GameState &game);
int isGameOverThenSelectWinner(GameState &game);

int main() {
    char command[40];
    while (1 == scanf("%s", &command)) {
        int n, m, k, activeplayer;
        if (!strcmp(command, "GEN_ALL_POS_MOV")) {
            scanf("%i %i %i %i", &n, &m, &k, &activeplayer);
            genAllPosMov(n, m, k, activeplayer);
        }
        else if (!strcmp(command, "GEN_ALL_POS_MOV_CUT_IF_GAME_OVER")) {
            scanf("%i %i %i %i", &n, &m, &k, &activeplayer);
            genAllPosMovCutIfGameOver(n, m, k, activeplayer);
        }
        else if (!strcmp(command, "SOLVE_GAME_STATE")) {
            scanf("%i %i %i %i", &n, &m, &k, &activeplayer);
            solveGameState(n, m, k, activeplayer);
        }
        else if (!strcmp(command, "q")) {
            break;
        }
    }
    return 0;
}

int evaluate(GameState &game) {
    for (int i = 0; i < game.n; i++) {
        for (int j = 0; j < game.m; j++) {
            if (game.board[i][j] == game.activePlayer) {
                if (checkWin(game, i, j, false)) { //check if activeplayer has won
                    return 1;
                }
            }
            else if (game.board[i][j] == game.otherPlayer) {
                if (checkWin(game, i, j, true)) { //check if otherplayer has won
                    return -1;
                }
            }
        }
    }
    return 0;
}

int minMax(GameState &game, int activePlayer) {
    //set players
    int otherPlayer;
    if (activePlayer == 1) otherPlayer = 2;
    else otherPlayer = 1;
    //evaluate
    int score = evaluate(game);
    if (score == 1 || score == -1) return score;
    //check for tie
    int counter = 0;
    for (int i = 0; i < game.n; i++) {
        for (int j = 0; j < game.m; j++) {
            if (game.board[i][j] == 0) counter++;
        }
    }
    if (counter == 0) return 0;
    //generate all possible moves and find best
    bool flagBest = false; //if this flag is set to true, minmax is not going to happen
    if(activePlayer == game.activePlayer) {
        int best = -777;
        for(int i = 0; i < game.n; i++) {
            for (int j = 0; j < game.m; j++) {
                if (game.board[i][j] == 0 && !flagBest) {
                    game.board[i][j] = activePlayer;
                    int current = minMax(game, otherPlayer);
                    if (current > best) best = current;
                    game.board[i][j] = 0;
                    if (best == 1) flagBest = true;
                }
            }
        }
        return best;
    }
    else {
        int best = 777;
        for(int i = 0; i < game.n; i++) {
            for (int j = 0; j < game.m; j++) {
                if (game.board[i][j] == 0 && !flagBest) {
                    game.board[i][j] = activePlayer;
                    int current = minMax(game, otherPlayer);
                    if (current < best) best = current;
                    game.board[i][j] = 0;
                    if (best == -1) flagBest = true;
                }
            }
        }
        return best;
    }
}

void solveGameState(int n, int m, int k, int activePlayer) {
    GameState game(n, m, k, activePlayer);
    readState(game);
    if (isGameOverThenSelectWinner(game) == -1) return; //returns -1 if game is over and number of free board points in other cases

    int best = -7777;
    bool flagBest = false; //if this flag is set to true, minmax is not going to happen
    for (int i = 0; i < game.n; i++) {
        for (int j = 0; j < game.m; j++) {
            if (game.board[i][j] == 0 && !flagBest) {
                game.board[i][j] = game.activePlayer;
                int score = minMax(game, game.otherPlayer);
                if (score > best) best = score;
                game.board[i][j] = 0;
                if(best == 1) flagBest = true;
            }
        }
    }
    if (best == 0) printf("BOTH_PLAYERS_TIE\n");
    else if (best == 1) {
        if(game.activePlayer == 1) printf("FIRST_PLAYER_WINS\n");
        else printf("SECOND_PLAYER_WINS\n");
    }
    else {
        if(game.otherPlayer == 1) printf("FIRST_PLAYER_WINS\n");
        else printf("SECOND_PLAYER_WINS\n");
    }
}

void readState(GameState &game) {
    for(int i = 0; i < game.n; i++) {
        for(int j = 0; j < game.m; j++) {
            scanf("%i", &game.board[i][j]);
        }
    }
}

void printBoardMod(GameState &game, int x, int y) {
    for (int i = 0; i < game.n; i++) {
        if(i > 0) printf("\n");
        for (int j = 0; j < game.m; j++) {
            if (i == x && j == y) printf("%i ", game.activePlayer);
            else printf("%i ", game.board[i][j]);
        }
    }
    printf("\n\n");
}

bool searchUpDown(GameState &game, int i, int j, int activePlayer) {
    int counter = game.k - 1;
    bool firstDirection = true; //up
    bool secondDirection = true; //down
    for(int c = 1; c < game.k; c++) {
        if (i - c >= 0 && firstDirection) { //check whether not out of bounds; going up
            if (game.board[i - c][j] == activePlayer) {
                counter--;
                if (counter == 0) return true;
            }
            else {
                firstDirection = false;
                if (!secondDirection) break;
            }
        }
        if (i + c < game.n && secondDirection) { //check whether not out of bounds; going down
            if (game.board[i + c][j] == activePlayer) {
                counter--;
                if (counter == 0) return true;
            }
            else {
                secondDirection = false;
                if (!firstDirection) break;
            }
        }
    }
    return false;
}

bool searchLeftRight(GameState &game, int i, int j, int activePlayer) {
    int counter = game.k - 1;
    bool firstDirection = true; //left
    bool secondDirection = true; //right
    for(int c = 1; c < game.k; c++) {
        if (j - c >= 0 && firstDirection) { //check whether not out of bounds; going left
            if (game.board[i][j - c] == activePlayer) {
                counter--;
                if (counter == 0) return true;
            }
            else {
                firstDirection = false;
                if (!secondDirection) break;
            }
        }
        if (j + c < game.m && secondDirection) { //check whether not out of bounds; going right
            if (game.board[i][j + c] == activePlayer) {
                counter--;
                if (counter == 0) return true;
            }
            else {
                secondDirection = false;
                if (!firstDirection) break;
            }
        }
    }
    return false;
}

bool searchDiagonalDownUp(GameState &game, int i, int j, int activePlayer) {
    int counter = game.k - 1;
    bool firstDirection = true; //up-right
    bool secondDirection = true; //down-left
    for(int c = 1; c < game.k; c++) {
        if ((i - c >= 0) && (j + c < game.m) && firstDirection) { //check whether not out of bounds; going up-right
            if (game.board[i - c][j + c] == activePlayer) {
                counter--;
                if (counter == 0) return true;
            }
            else {
                firstDirection = false;
                if (!secondDirection) break;
            }
        }
        if ((i + c < game.n) && (j - c >= 0) && secondDirection) { //check whether not out of bounds; going down-left
            if (game.board[i + c][j - c] == activePlayer) {
                counter--;
                if (counter == 0) return true;
            }
            else {
                secondDirection = false;
                if (!firstDirection) break;
            }
        }
    }
    return false;
}

bool searchDiagonalUpDown(GameState &game, int i, int j, int activePlayer) {
    int counter = game.k - 1;
    bool firstDirection = true; //up-left
    bool secondDirection = true; //down-right
    for(int c = 1; c < game.k; c++) {
        if ((i - c >= 0) && (j - c >= 0) && firstDirection) { //check whether not out of bounds; going up-left
            if (game.board[i - c][j - c] == activePlayer) {
                counter--;
                if (counter == 0) return true;
            }
            else {
                firstDirection = false;
                if (!secondDirection) break;
            }
        }
        if ((i + c < game.n) && (j + c < game.m) && secondDirection) { //check whether not out of bounds; going down-right
            if (game.board[i + c][j + c] == activePlayer) {
                counter--;
                if (counter == 0) return true;
            }
            else {
                secondDirection = false;
                if (!firstDirection) break;
            }
        }
    }
    return false;
}

bool checkWin(GameState &game, int i, int j, bool invertPlayer) {
    int activePlayer;
    if (invertPlayer) activePlayer = game.otherPlayer;
    else activePlayer = game.activePlayer;

    if(searchUpDown(game, i, j, activePlayer)) return true;
    if(searchLeftRight(game, i, j, activePlayer)) return true;
    if(searchDiagonalDownUp(game, i, j, activePlayer)) return true;
    if(searchDiagonalUpDown(game, i, j, activePlayer)) return true;
    return false;
}

void genAllPosMov(int n, int m, int k, int activePlayer) {
    GameState game(n, m, k, activePlayer);
    readState(game);
    int counter = isGameOver(game); //returns -1 if game is over and counter value in other cases
    if (counter == -1) return;

    printf("%i\n", counter);
    for(int i = 0; i < game.n; i++) { //print possible moves
        for (int j = 0; j < game.m; j++) {
            if (game.board[i][j] == 0) {
                printBoardMod(game, i, j);
            }
            else {
                continue;
            }
        }
    }
}

void genAllPosMovCutIfGameOver(int n, int m, int k, int activePlayer) {
    GameState game(n, m, k, activePlayer);
    readState(game);
    int counter = isGameOver(game); //returns -1 if game is over and counter value in other cases
    if (counter == -1) return;

    for (int i = 0; i < game.n; i++) { //check whether winning move exists
        for (int j = 0; j < game.m; j++) {
            if (checkWin(game, i, j, false) && game.board[i][j] == 0) {
                printf("1\n");
                printBoardMod(game, i, j);
                return;
            }
        }
    }

    printf("%i\n", counter);
    for(int i = 0; i < game.n; i++) { //print possible moves
        for (int j = 0; j < game.m; j++) {
            if (game.board[i][j] == 0) {
                printBoardMod(game, i, j);
            }
            else {
                continue;
            }
        }
    }
}

int isGameOverThenSelectWinner(GameState &game) {
    int counter = 0; //counts number of free board points
    for (int i = 0; i < game.n; i++) {
        for (int j = 0; j < game.m; j++) {
            if (game.board[i][j] == game.activePlayer) {
                if (checkWin(game, i, j, false)) { //check whether game's not over
                    if(game.activePlayer == 1) printf("FIRST_PLAYER_WINS\n");
                    else printf("SECOND_PLAYER_WINS\n");
                    return -1;
                }
            }
            else if (game.board[i][j] == game.otherPlayer) {
                if (checkWin(game, i, j, true)) { //check whether game's not over
                    if(game.otherPlayer == 1) printf("FIRST_PLAYER_WINS\n");
                    else printf("SECOND_PLAYER_WINS\n");
                    return -1;
                }
            }
            else if (game.board[i][j] == 0) {
                counter++;
            }
        }
    }
    if (counter == 0) { //tie
        printf("BOTH_PLAYERS_TIE\n");
        return -1;
    }
    return counter; //returns -1 if game is over
}

int isGameOver(GameState &game) {
    int counter = 0; //counts number of free board points
    for (int i = 0; i < game.n; i++) {
        for (int j = 0; j < game.m; j++) {
            if (game.board[i][j] == game.activePlayer) {
                if (checkWin(game, i, j, false)) { //check whether game's not over
                    printf("0\n");
                    return -1;
                }
            }
            else if (game.board[i][j] == game.otherPlayer) {
                if (checkWin(game, i, j, true)) { //check whether game's not over
                    printf("0\n");
                    return -1;
                }
            }
            else if (game.board[i][j] == 0) {
                counter++;
            }
        }
    }

    if (counter == 0) { //tie
        printf("0\n");
        return -1;
    }
    return counter; //returns -1 if game is over
}