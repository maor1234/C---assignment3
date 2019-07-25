#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define COLS_NUM 7
#define ROWS_NUM 6
#define SEQ_LEN 4
#define P1_TOKEN 'X'
#define P2_TOKEN 'o'
#define EMPTY_SPOT ' '
#define UNDO_CODE 0 // the undo of the player
#define INVALID_MOVE -1
#define UNDO_STACK_SIZE 42
#define MS_VALID_MOVE 0
#define MS_OUT_OF_RANGE 1
#define MS_UNDO_ON_EMPTY 2
#define MS_FULL_COL 3
// define possible game statuses
#define GS_FULL_BOARD 0
#define GS_WIN 1
#define GS_ONGOING 2

bool check_winning_row(char board[ROWS_NUM][COLS_NUM],
                       int row, int col);
bool check_winning_col(char board[ROWS_NUM][COLS_NUM],
                       int row, int col);
bool check_winning_slash(char board[ROWS_NUM][COLS_NUM],
                        int row, int col);
bool check_winning_backslash(char board[ROWS_NUM][COLS_NUM],
                             int row, int col);

void init_board(char board[ROWS_NUM][COLS_NUM])
{
    for (int i = 0; i < ROWS_NUM; i++)
    {
        for (int j = 0; j < COLS_NUM; j++)
        {
            board[i][j] = EMPTY_SPOT;
        }
    }
}

void print_board(char board[ROWS_NUM][COLS_NUM])
{/* prints connect-4 board */
	for (int i=ROWS_NUM - 1;i >= 0; i--)
	{
	    printf("|");
		for (int j = 0; j < COLS_NUM; j++)
		{
		    printf("%c|",board[i][j]);
		}
		if (i > 0)
            printf("\n");
	}

	printf("\n");
	for (int i = 0; i <= 2*COLS_NUM; i++)
        printf("-");
    printf("\n");

	for (int i = 1; i <= COLS_NUM; i++)
	{
	    printf(" %d", i);
	}
	printf("\n");
}

void add_token(char board[ROWS_NUM][COLS_NUM],
               int col, char token_char,
               int undo_stack[UNDO_STACK_SIZE], int num_of_moves)
{//add a new token to board.
    int topmost_index = topmost_token_index(board, col);
    board[topmost_index + 1][col] = token_char;
    undo_stack[num_of_moves] = col * COLS_NUM + topmost_index + 1;
}

int topmost_token_index(char board[ROWS_NUM][COLS_NUM], int col)
{// return the index of the topmost token in the current column.
    for (int i = 0; i < ROWS_NUM; i++)
    {
        if (board[i][col] == EMPTY_SPOT)
        {
            return i - 1;
        }
    }
    return ROWS_NUM - 1;
}

bool check_winning_move(char board[ROWS_NUM][COLS_NUM],
                        int col)
{// return true if the last move was a victory move.
    int row = topmost_token_index(board, col);
    if (check_winning_row(board, row, col) ||
        check_winning_col(board, row, col) ||
        check_winning_slash(board, row, col) ||
        check_winning_backslash(board, row, col))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool check_winning_row(char board[ROWS_NUM][COLS_NUM],
                       int row, int col)
{//return true if the last move create a victory row.
    int tokens_in_row = 1;
    char last_token = board[row][col];

    for (int i = col - 1; i >= 0; i--)
    {
        if (board[row][i] != last_token)
            break;
        tokens_in_row++;
    }
    for (int i = col + 1; i < COLS_NUM; i++)
    {
        if (board[row][i] != last_token)
            break;
        tokens_in_row++;
    }
    return tokens_in_row >= SEQ_LEN;
}

bool check_winning_col(char board[ROWS_NUM][COLS_NUM],
                       int row, int col)
{//return true if the last move create a victory column.
    int tokens_in_col = 1;
    char last_token = board[row][col];

    for (int i = row - 1; i >= 0; i--)
    {
        if (board[i][col] != last_token)
            break;
        tokens_in_col++;
    }
    return tokens_in_col >= SEQ_LEN;
}

bool check_winning_slash(char board[ROWS_NUM][COLS_NUM],
                         int row, int col)
{//return true if the last move create a victory
    int tokens_in_slash = 1;
    char last_token = board[row][col];

    for (int i = 1; i <= col && i <= row; i++)
    {
        if (board[row - i][col - i] != last_token)
            break;
        tokens_in_slash++;
    }
    for (int i = 1; row + i < ROWS_NUM && col + i < COLS_NUM; i++)
    {
        if (board[row + i][col + i] != last_token)
            break;
        tokens_in_slash++;
    }
    return tokens_in_slash >= SEQ_LEN;
}

bool check_winning_backslash(char board[ROWS_NUM][COLS_NUM],
                             int row, int col)
{//return true if the last move created a victory diagonal(\).
    int tokens_in_backslash = 1;
    char last_token = board[row][col];

    for (int i = 1; i + col < COLS_NUM && i <= row; i++)
    {
        if (board[row - i][col + i] != last_token)
            break;
        tokens_in_backslash++;
    }
    for (int i = 1; row + i < ROWS_NUM && i <= col; i++)
    {
        if (board[row + i][col - i] != last_token)
            break;
        tokens_in_backslash++;
    }
    return tokens_in_backslash >= SEQ_LEN;
}


bool is_board_full(char board[ROWS_NUM][COLS_NUM])
{//returns true if the game board no empty.
    for (int i = 0; i < COLS_NUM; i++)
    {
        if (board[ROWS_NUM - 1][i] == EMPTY_SPOT)
            return false;
    }
    return true;
}

bool is_column_full(char board[ROWS_NUM][COLS_NUM], int col)
{//return true if in the column in board is full
    if (col < 0 || col > COLS_NUM)
    {
        printf("Invalid input, this column is full");
        return true;
    }
    return board[ROWS_NUM - 1][col] != EMPTY_SPOT;
}

bool is_board_empty(char board[ROWS_NUM][COLS_NUM])
{//returns true if the game board is without tokens
    for (int i = 0; i < COLS_NUM; i++)
    {
        if (board[0][i] != EMPTY_SPOT)
            return false;
    }
    return true;
}

char check_move(char board[ROWS_NUM][COLS_NUM], int move)
{//check the current move and return the result code
    if ((move < 1 || move > COLS_NUM) && move != UNDO_CODE)
    {
        return MS_OUT_OF_RANGE;
    }
    if (move == UNDO_CODE)
    {
        if (!is_board_empty(board))
        {
            return MS_VALID_MOVE;
        }
        else
        {
            return MS_UNDO_ON_EMPTY;
        }
    }
    if (is_column_full(board, move - 1))
    {
        return MS_FULL_COL;
    }
    return MS_VALID_MOVE;
}

void print_move_error(int move, char move_status)
{//prints errors message
    printf("Invalid input");
    switch (move_status)
    {
        case MS_OUT_OF_RANGE: printf(", the number must be between 1 to 7"); break;
        case MS_UNDO_ON_EMPTY: printf("Board is empty - can't undo!");
                            break;
        case MS_FULL_COL: printf(" this column %d is full.Please choose another one:", move); break;
        default: printf("Invalid input");
    }
    printf("\n ,Please choose another one:\n");
}

int scan_move(char board[ROWS_NUM][COLS_NUM], char curr_player_token)
{//ask for and get current move from the player n
    int move = INVALID_MOVE;
    printf("Player %c, please enter a column number ", curr_player_token);
    printf("(or %d to undo):", UNDO_CODE);
    if (scanf("%d", &move) != 1)
    {
        printf("Invalid input, bye-bye!");
        return INVALID_MOVE;
    }
    while (check_move(board, move) != MS_VALID_MOVE)
    {
        print_move_error(move, check_move(board, move));
        if (scanf("%d", &move) != 1)
        {
            printf("Invalid input, bye-bye!");
            return INVALID_MOVE;
        }
    }
    return move;
}

int make_move(char board[ROWS_NUM][COLS_NUM],
               int undo_stack[UNDO_STACK_SIZE], int num_of_moves,
               char curr_player_token, int move)
{//make a single move (add token or undo) and return updated num_of_moves.
    if (move == UNDO_CODE)
    {
        undo(board, undo_stack, num_of_moves);
        return num_of_moves - 1;
    }
    else
    {
        add_token(board, move - 1, curr_player_token,
                  undo_stack, num_of_moves);
        return num_of_moves + 1;
    }
}

void undo(char board[ROWS_NUM][COLS_NUM],
               int undo_stack[UNDO_STACK_SIZE], int num_of_moves)
{//the undo.
    int last_move_entry = undo_stack[num_of_moves - 1];
    int last_move_row = last_move_entry % COLS_NUM;
    int last_move_col = last_move_entry / COLS_NUM;

    board[last_move_row][last_move_col] = EMPTY_SPOT;
}

char game_status(char board[ROWS_NUM][COLS_NUM], int col)
{//return the status of the game in every step.
    if (col >= 0 && col < COLS_NUM && check_winning_move(board, col))
    {
        return GS_WIN;
    }
    if (is_board_full(board))
    {
        return GS_FULL_BOARD;
    }
    return GS_ONGOING;
}

void print_end_message(char game_status, char curr_token)
{//prints a message that mean if some player is won
    switch (game_status)
    {
        case GS_FULL_BOARD: printf("It's a tie.\n"); break;
        case GS_WIN: printf("Player %c wins!\n", curr_token);
                     break;
        default: printf("Error in print_end_message: \
                        game has not ended\n"); break;
    }
}

bool play_turn(char board[ROWS_NUM][COLS_NUM], char curr_token,
               int undo_stack[UNDO_STACK_SIZE], int *num_of_moves)
{//play one turn of the game. if the game ended return false ;
    print_board(board);
    int move = scan_move(board, curr_token);
    if (move == INVALID_MOVE)
        return false;
    *num_of_moves = make_move(board, undo_stack, *num_of_moves,
                              curr_token, move);
    if (game_status(board, move - 1) != GS_ONGOING)
    {
        print_board(board);
        print_end_message(game_status(board, move - 1), curr_token);
        return false;
    }
    return true;
}
int main()
{//the main send a charboard game to the functions
    char board[ROWS_NUM][COLS_NUM];
    char curr_token = P1_TOKEN;
    int undo_stack[UNDO_STACK_SIZE] = {0}, num_of_moves = 0;

    init_board(board);
    printf("Welcome!!!\n\n");

    while (play_turn(board, curr_token, undo_stack, &num_of_moves))
    {
        curr_token = (curr_token==P1_TOKEN) ? P2_TOKEN : P1_TOKEN;
    }
    return 0;
}

