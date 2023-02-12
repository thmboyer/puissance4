#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>

int W, H, N, S;
int DX[4] = {1, 0, 1, -1};
int DY[4] = {0, 1, 1, 1};

// AI variables
int COEFF[5] = {0, 1, 5, 20, 10000};
int FEAR = 5;
int RAND_FACTOR = 4;

void debug(int board[7][6]) {
    for (int y = H-1; y >= 0; --y) {
        printf(">");
        for (int x = 0; x < W; ++x) {
            if (board[x][y]) {
                if ((board[x][y] == 1 && S == 1) || (board[x][y] == -1 && S == 2))
                    printf(" 1");
                else
                    printf(" 2");
            } else {
                printf(" .");
            }
        }
        printf("\n");
    }
    printf("> \n");
}

int scoreFunction(int board[7][6], int x, int y) {
    int score = 0;
    int xo, yo;
    int xn, yn;
    int player;
    int streak;
    for (int dir = 0; dir < 4; ++dir) {
        xo = x;
        yo = y;
        for (int offset = 0; offset < 4; ++offset) {
            player = 0;
            streak = 1;
            xn = xo;
            yn = yo;
            for (int step = 0; step < 4; ++step) {
                if (xn < 0 || xn >= W || yn < 0 || yn >= H) {
                    streak = 0;
                    break;
                }
                if (board[xn][yn]) {
                    if (!player) {
                        player = board[xn][yn];
                    }
                    if (board[xn][yn]) {
                        if (board[xn][yn] == player) {
                            ++ streak;
                        } else {
                            streak = 0;
                            break;
                        }
                    }
                }
                xn += DX[dir];
                yn += DY[dir];
            }
            score += COEFF[streak];
            xo -= DX[dir];
            yo -= DY[dir];
        }
    }
    if (RAND_FACTOR) {
        score += rand() % RAND_FACTOR - RAND_FACTOR / 2;
    }
    return score;
}


int bestMove(int board[7][6]) {

    int xMaxs[7];
    int nbXMaxs = 0;
    int y;
    int score, scoreCurrent, scoreAbove;
    int scoreMax = INT_MIN;

    for (int x = 0; x < W; ++x) {

        for (y = 0; y < H && board[x][y]; ++y);
        if (y == H) {
            continue;
        }

        scoreCurrent = scoreFunction(board, x, y);

        score = scoreCurrent;
        if (y + 1 < H) {
            scoreAbove = scoreFunction(board, x, y + 1);
            score -= scoreAbove / FEAR;
        }

        printf("> CONV x=%d s=%d s1=%d s1=%d\n", x, score, scoreCurrent, scoreAbove);

        if (score >= scoreMax) {
            if (score > scoreMax) {
                scoreMax = score;
                nbXMaxs = 0;
            }
            xMaxs[nbXMaxs] = x;
            ++ nbXMaxs;
        }
    }
    return xMaxs[rand() % nbXMaxs];
}


int main()
{
    scanf("%d %d %d %d", &W, &H, &N, &S);
    int board[7][6] = {0};
    int player = (S == 1) ? 1 : -1;
    int x, y;
    srand(time(NULL) * (time_t) &board);

    while (1) {

        if (player == 1) {
            x = bestMove(board);
            printf("%d\n", x); 
        } else {
            scanf("%d", &x);
        }

        for (y = 0; y < H && board[x][y]; ++y);
        board[x][y] = player;


        player = -player;

    }
}
