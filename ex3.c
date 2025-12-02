#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

// for function declarations
#define SEQUENCEOF2 2
#define SEQUENCEOF3 3
#define SEQUENCEOF4 4

// checking if a column is full
int isColumnFull(char[][COLS], int, int);

// checking if the board is full
int isBoardFull(char[][COLS], int, int);


int isInBounds(int, int, int, int);

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char[][COLS], int, int);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char[][COLS], int, int, char);

// Checkfor victory functions:
int checkInRow(char[][COLS], int, int, int, char);
int checkInColumn(char[][COLS], int, int, int, char);
int checkInDiagonal(char[][COLS], int, int, int, char);
int checkInAntiDiagonal(char[][COLS], int, int, int, char);

/* Check for victory from last move at (row, col). Return 1 if win, 0 otherwise */
int isWinning(char[][COLS], int, int, char);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char[][COLS], int, int);

/* Computer*- these are functions that are related to the computer*/
// bot priorities for choosing a column
int computerChoose(char board[][COLS], char playerToken, char opponentToken);

// Organizing columns in order of preference (center to edges)
void arrayOfOptions(int cols, int order[]);

// checking if a move creates a sequence of `sequence` tokens
int checkThree(char board[][COLS], int, int, int, char);

/* Main game loop */
void runConnectFour(char[][COLS], int, int, int, int);

/* Initialize the board to empty */
void initBoard(char[][COLS], int, int);

// Print the board to the console
void printBoard(char[][COLS], int, int);

// Get player type (human or computer)
int getPlayerType(int);

/*calling the main function--written by author-
basically  getting player types and printing the board, the run function is called here*/
int main()
{
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

// written by author- printing the board
void printBoard(char board[][COLS], int rows, int cols)
{
    printf("\n");
    for (int r = 0; r < rows; r++)
    {
        printf("|");
        for (int c = 0; c < cols; c++)
        {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++)
    {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

// same for getting player type
int getPlayerType(int playerNumber)
{
    char ch;
    while (1)
    {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1)
        {
            printf("Input error. Try again.\n");
            while (getchar() != '\n')
                ; // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H')
            return HUMAN;
        if (ch == 'c' || ch == 'C')
            return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n')
            ; // clear rest of input
    }
}

// main game loop function
void runConnectFour(char board[][COLS], int rows, int cols, int p1Type, int p2Type)
{
    while (1)
    {
        int humanChoiceCol = 0, humanChoiceRow = 0;

        /* ==== Player 1 turn ==== */
        if (p1Type == HUMAN)
        {
            printf("Player 1 (%c) turn.\n", TOKEN_P1);
            humanChoiceCol = humanChoose(board, cols, rows);
            printf("Enter column (1-%d):\n ", cols);
            humanChoiceRow = makeMove(board, rows, humanChoiceCol, TOKEN_P1);

            printBoard(board, rows, cols);

            if (isWinning(board, humanChoiceRow, humanChoiceCol, TOKEN_P1))
            {
                printf("Player 1 (%c) wins!\n", TOKEN_P1);
                break;
            }
            else if (isBoardFull(board, rows, cols))
            {
                printf("Board full and no winner. It's a tie!\n");
                break;
            }
        }
        else if (p1Type == COMPUTER)
        {
            printf("Player 1 (%c) turn.\n", TOKEN_P1);

            int pcCol = computerChoose(board, TOKEN_P1, TOKEN_P2);
            int pcRow = makeMove(board, rows, pcCol, TOKEN_P1);

            printBoard(board, rows, cols);

            if (isWinning(board, pcRow, pcCol, TOKEN_P1))
            {
                printf("Player 1 (%c) wins!\n", TOKEN_P1);
                break;
            }
            else if (isBoardFull(board, rows, cols))
            {
                printf("Board full and no winner. It's a tie!\n");
                break;
            }
        }

        /* ==== Player 2 turn ==== */
        if (p2Type == HUMAN)
        {
            printf("Player 2 (%c) turn.\n", TOKEN_P2);
            humanChoiceCol = humanChoose(board, cols, rows);
            printf("Enter column (1-%d):\n ", cols);
            humanChoiceRow = makeMove(board, rows, humanChoiceCol, TOKEN_P2);

            printBoard(board, rows, cols);

            if (isWinning(board, humanChoiceRow, humanChoiceCol, TOKEN_P2))
            {
                printf("Player 2 (%c) wins!\n", TOKEN_P2);
                break;
            }
            else if (isBoardFull(board, rows, cols))
            {
                printf("Board full and no winner. It's a tie!\n");
                break;
            }
        }
        else if (p2Type == COMPUTER)
        {
            printf("Player 2 (%c) turn.\n", TOKEN_P2);
            int pcCol = computerChoose(board, TOKEN_P2, TOKEN_P1);
            int pcRow = makeMove(board, rows, pcCol, TOKEN_P2);

            printBoard(board, rows, cols);

            if (isWinning(board, pcRow, pcCol, TOKEN_P2))
            {
                printf("Player 2 (%c) wins!\n", TOKEN_P2);
                break;
            }
            else if (isBoardFull(board, rows, cols))
            {
                printf("Board full and no winner. It's a tie!\n");
                break;
            }
        }
    }
}

int isBoardFull(char board[][COLS], int rows, int cols)
{
    // Check each column to see if it's full
    for (int c = 0; c < cols; c++)
    {
        if (!isColumnFull(board, rows, c))
            return 0; // There is at least one non-full column
    }
    return 1; // All columns are full
}

// FUNCTION: initializing the board
void initBoard(char board[][COLS], int rows, int cols)
{
    // Initialize all cells to EMPTY
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            board[r][c] = EMPTY;
        }
    }
}

// FUNCTION: checking if a column is full
int isColumnFull(char board[][COLS], int rows, int col)
{
    for (int r = 0; r < rows; r++)
    {
        if (board[r][col] == EMPTY)
        {
            return 0; // Column is not full
        }
    }
    return 1; // Column is full
}

int getFreeRow(char board[][COLS], int rows, int col)
{
    for (int r = rows - 1; r >= 0; r--)
    {
        if (board[r][col] == EMPTY)
        {
            return r; // Return the first empty row from the bottom
        }
    }
    return -1; // Column is full
}

// function for human to choose a column
int humanChoose(char board[][COLS], int cols, int rows)
{
    //(void)board; 

    while (1)
    {
        int col;
        int validInput = scanf("%d", &col);
        if (validInput != 1)
        {
            printf("Enter column (1-%d): Invalid input. Enter a number.\n", cols);

            while (getchar() != '\n');
            continue;
        }
        col--; // Convert to 0-based index
        if (col < 0 || col >= cols)
        {
            printf("Enter column (1-%d): Invalid column. Choose between 1 and %d.\n", cols, cols);
            continue;
        }
        if (isColumnFull(board, rows, col))
        {
            printf("Enter column (1-%d): Column %d is full. Choose another column.\n", cols, col + 1);
            continue;
        }
        return col;
    }
}

// Function: for the computer to organize columns in order of preference (center to edges)
void arrayOfOptions(int cols, int order[])
{
    int idx = 0; // index for order array

    if (cols % 2 == 1)
    {
        int center = cols / 2;
        order[idx++] = center;

        for (int d = 1; idx < cols; d++)
        {
            if (center - d >= 0)
                order[idx++] = center - d;

            if (center + d < cols)
                order[idx++] = center + d;
        }
    }
    else
    {
        int leftCenter = cols / 2 - 1;
        int rightCenter = cols / 2;

        for (int d = 0; idx < cols; d++)
        {
            if (leftCenter - d >= 0)
                order[idx++] = leftCenter - d;

            if (rightCenter + d < cols)
                order[idx++] = rightCenter + d;
        }
    }
}

// Function: computer's choice of column based on priorities
int computerChoose(char board[][COLS], char playerToken, char opponentToken)
{
    int order[COLS];
    arrayOfOptions(COLS, order);

    // 1. Winning move
    for (int i = 0; i < COLS; i++)
    {
        int col = order[i];
        if (isColumnFull(board, ROWS, col))
            continue;

        int row = getFreeRow(board, ROWS, col);
        if (row == -1)
            continue;

        board[row][col] = playerToken;
        int win = isWinning(board, row, col, playerToken);
        board[row][col] = EMPTY;

        if (win)
        {
            printf("Computer chose column %d\n", col + 1);
            return col;
        }
    }

    // 2. Block opponent's winning move
    for (int i = 0; i < COLS; i++)
    {
        int col = order[i];
        if (isColumnFull(board, ROWS, col))
            continue;

        int row = getFreeRow(board, ROWS, col);
        if (row == -1)
            continue;

        board[row][col] = opponentToken;
        int oppWin = isWinning(board, row, col, opponentToken);
        board[row][col] = EMPTY;

        if (oppWin)
        {
            printf("Computer chose column %d\n", col + 1);
            return col;
        }
    }

    // 3. Creating a sequence of three (for the computer)
    for (int i = 0; i < COLS; i++)
    {
        int col = order[i];
        if (isColumnFull(board, ROWS, col))
            continue;

        int row = getFreeRow(board, ROWS, col);
        if (row == -1)
            continue;

        board[row][col] = playerToken;
        if (checkThree(board, row, col, SEQUENCEOF3, playerToken))
        {
            board[row][col] = EMPTY;
            printf("Computer chose column %d\n", col + 1);
            return col;
        }
        board[row][col] = EMPTY;
    }

    // 4. Blocking opponent’s sequence of three
    for (int i = 0; i < COLS; i++)
    {
        int col = order[i];
        if (isColumnFull(board, ROWS, col))
            continue;

        int row = getFreeRow(board, ROWS, col);
        if (row == -1)
            continue;

        board[row][col] = opponentToken;

        if (checkThree(board, row, col, SEQUENCEOF3, opponentToken))
        {
            board[row][col] = EMPTY;
            printf("Computer chose column %d\n", col + 1);
            return col;
        }

        board[row][col] = EMPTY;
    }

    // 5. Default: first legal column according to order[]
    for (int i = 0; i < COLS; i++)
    {
        int col = order[i];
        if (!isColumnFull(board, ROWS, col))
        {
            printf("Computer chose column %d\n", col + 1);
            return col;
        }
    }

    return 0;
}

// checking for a horizontal win starting from where the token was placed
int checkInRow(char board[][COLS], int row, int col, int sequence, char token)
{
    int tempColLeft = col;
    int tempColRight = col;

    // extend to both directions
    while ((tempColLeft >= 0 && board[row][tempColLeft] == token) ||
           (tempColRight < COLS && board[row][tempColRight] == token))
    {
        if (tempColLeft >= 0 && board[row][tempColLeft] == token)
            tempColLeft--;
        if (tempColRight < COLS && board[row][tempColRight] == token)
            tempColRight++;
    }

    int leftCount = col - tempColLeft - 1;
    int rightCount = tempColRight - col - 1;

    return (leftCount + rightCount + 1) >= sequence;
}

// check vertical wins - count up and down from the current position
int checkInColumn(char board[][COLS], int row, int col, int sequence, char token)
{
    int tempRow = row;

    while (tempRow >= 0 && board[tempRow][col] == token)
        tempRow--;
    int upCount = row - tempRow - 1;

    tempRow = row;
    while (tempRow < ROWS && board[tempRow][col] == token)
        tempRow++;
    int downCount = tempRow - row - 1;

    return (upCount + downCount + 1) >= sequence;
}

// diagonal check - the \ direction (top-left to bottom-right)
int checkInDiagonal(char board[][COLS], int row, int col, int sequence, char token)
{
    int tempRow = row;
    int tempCol = col;

    while (tempRow >= 0 && tempCol >= 0 && board[tempRow][tempCol] == token)
    {
        tempRow--;
        tempCol--;
    }
    int upLeftCount = row - tempRow - 1;

    tempRow = row;
    tempCol = col;
    while (tempRow < ROWS && tempCol < COLS && board[tempRow][tempCol] == token)
    {
        tempRow++;
        tempCol++;
    }
    int downRightCount = tempRow - row - 1;

    return (upLeftCount + downRightCount + 1) >= sequence;
}

// other diagonal - the / direction (top-right to bottom-left)
int checkInAntiDiagonal(char board[][COLS], int row, int col, int sequence, char token)
{
    int tempRow = row;
    int tempCol = col;

    while (tempRow >= 0 && tempCol < COLS && board[tempRow][tempCol] == token)
    {
        tempRow--;
        tempCol++;
    }
    int upRightCount = row - tempRow - 1;

    tempRow = row;
    tempCol = col;
    while (tempRow < ROWS && tempCol >= 0 && board[tempRow][tempCol] == token)
    {
        tempRow++;
        tempCol--;
    }
    int downLeftCount = tempRow - row - 1;

    return (upRightCount + downLeftCount + 1) >= sequence;
}

// main victory checker - tries all four possible directions
int isWinning(char board[][COLS], int row, int col, char token)
{
    if (checkInRow(board, row, col, SEQUENCEOF4, token) ||
        checkInColumn(board, row, col, SEQUENCEOF4, token) ||
        checkInDiagonal(board, row, col, SEQUENCEOF4, token) ||
        checkInAntiDiagonal(board, row, col, SEQUENCEOF4, token))
        return 1;
    return 0;
}

int checkThree(char board[][COLS], int row, int col, int sequence, char token)
{
    if (checkInRow(board, row, col, sequence, token) ||
        checkInColumn(board, row, col, sequence, token) ||
        checkInDiagonal(board, row, col, sequence, token) ||
        checkInAntiDiagonal(board, row, col, sequence, token))
        return 1;
    return 0;
}

// placing the token in the board according to the column chosen
int makeMove(char board[][COLS], int rows, int col, char token)
{
    if (isColumnFull(board, rows, col))
    {
        return -1; // Column is full
    }

    int row = getFreeRow(board, rows, col);
    if (row == -1)
    {
        return -1;
    }

    board[row][col] = token;
    return row;
}
