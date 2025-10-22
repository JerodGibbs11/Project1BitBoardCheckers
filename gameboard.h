#ifndef GAMEBOARD_H
#define GAMEBOARD_H


#define BITVALUE(i) (1ULL << (i))

typedef struct
{
    uint64_t red;
    uint64_t black;
    uint64_t king_places;
	uint64_t black_moves;
	uint64_t red_moves;
    bool     user_turn;
} GameBoard;


//masks to remove columns to prevent issues while bit shifting
static const uint64_t notA = 0b1111111011111110111111101111111011111110111111101111111011111110ULL;
static const uint64_t notH = 0b0111111101111111011111110111111101111111011111110111111101111111ULL;

//masks columsn to prevent issues with bit shifting while checking for captures
static const uint64_t notAB = 0b1111110011111100111111001111110011111100111111001111110011111100ULL;
static const uint64_t notGH = 0b0011111100111111001111110011111100111111001111110011111100111111ULL;

//top and bottom ranks mask
static const uint64_t RANK0 = 0b0000000000000000000000000000000000000000000000000000000011111111ULL;
static const uint64_t RANK7 = 0b1111111100000000000000000000000000000000000000000000000000000000ULL;

//----------------------------------MOVEMENT AND CAPTURES----------------------------------
static uint64_t game_check_if_move_valid(GameBoard *board, int piece_index, bool bBlackToMove);
static uint64_t game_jump_piece(uint64_t piece, uint64_t destination);
bool apply_move(GameBoard *board, int piece_index, int destination_index, bool bBlackToMove);
static uint64_t game_jump_piece(uint64_t piece, uint64_t destination);
uint64_t find_possible_moves(const GameBoard *board, bool bBlackMoves);

//----------------------------------BOARD PREPARATION----------------------------------
void reset_game_board(GameBoard *board);
void game_board_start(GameBoard *board);

//----------------------------------HELPERS----------------------------------
uint64_t board_empty(const GameBoard *board);
uint64_t board_occupied(const GameBoard *board);
int game_board_to_index(char *user_move);

//----------------------------------GAME END----------------------------------
bool game_check_victory(const GameBoard *board, bool black_turn);

//----------------------------------DISPLAY AND DEBUG----------------------------------
void game_print_bitboards(const GameBoard *board);
void game_print_board(const GameBoard *board);
static void game_print_binary(uint64_t value);

#endif