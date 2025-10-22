#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "bitmanipulation.h"
#include "gameboard.h"


//----------------------------------HELPERS----------------------------------


uint64_t board_occupied(const GameBoard *board)
{
	//returns mask of either square being occupied so we can find what is not occupied
	return (board->red | board->black);
}

uint64_t board_empty(const GameBoard *board)
{
	//returns mask of 1 in every spot that is empty
	return ~(board->red | board->black);
}

int game_board_to_index(char *user_move)
{
	if(user_move == NULL)
	{
		//return something weird so we know there is a problem
		return -1;
	}
	
	char file = tolower((user_move[0]));
	char rank = user_move[1];
	
	int col = -1;
	int row = -1;
	
	for(int i = 0; i < 8; i++)
	{
		//find index for character
		if(file == ('a' + i))
		{
			col = i;
			break;
		}
	}
	
	for(int i = 0; i < 8; i++)
	{
		if(rank == ('1' + i))
		{
			row = i;
			break;
		}
	}
	
	//something went wrong in logic col and row never set
	if(col == -1 || row == -1)
	{
		return  -1;
	}
	
	//index from our coordinate position
	return (row * 8  + col);
}

//----------------------------------BOARD PREPARATION----------------------------------

//empties the board
void reset_game_board(GameBoard *board)
{
	if(board == NULL)
	{
		return;
	}
	
	board->red = board->red & 0;
	board->black = board->black & 0;
	board->king_places = board->king_places & 0;
	board->user_turn = true;
}

//initializes the board for a new game
void game_board_start(GameBoard *board)
{
	board->user_turn = true;
	
	const uint8_t EVEN_ROW_MASK = 0b01010101;
	const uint8_t ODD_ROW_MASK = 0b10101010;
	
	//creating black's starting positions
	for(int row = 0; row < 3; row++)
	{
		uint64_t mask;
		if(row % 2 == 0)
		{
			mask = (uint64_t)EVEN_ROW_MASK;
		}
		else
		{
			mask = (uint64_t)ODD_ROW_MASK;
		}
		
		board->black |= mask << (row * 8);
	}
	
	//creating red's starting positions
	for(int row = 5; row < 8; row++)
	{
		uint64_t mask;
		if(row % 2 == 0)
		{
			mask = (uint64_t)EVEN_ROW_MASK;
		}
		else
		{
			mask = (uint64_t)ODD_ROW_MASK;
		}
		
		board->red |= mask << (row * 8);
	}
}


//----------------------------------MOVEMENT AND CAPTURES----------------------------------

//check for all possible moves for a side so we can decide if the game has ended if one side has no moves and it is their turn
uint64_t find_possible_moves(const GameBoard *board, bool bBlackMoves)
{
    uint64_t occupied = board_occupied(board);
    uint64_t empty = board_empty(board);

	uint64_t current_side;
	uint64_t current_opponent;
	
	if(bBlackMoves)
	{
		current_side = board->black;
		current_opponent = board->red;
	}
	else
	{
		current_side = board->red;
		current_opponent = board->black;
	}
	
	uint64_t kings = current_side & board->king_places;
	uint64_t pieces = current_side & ~board->king_places;
	
	uint64_t basic_moves = 0;
	uint64_t king_moves = 0;
	uint64_t basic_captures = 0;
	uint64_t king_captures = 0;

	//find all the basic moves for whichever side we currently are
	if(bBlackMoves)
	{
		// Black moves upward
		basic_moves = (((pieces & notA) << 7) | ((pieces & notH) << 9)) & empty;
	}
	else
	{
		// Red moves downward
		basic_moves = (((pieces & notH) >> 7) | ((pieces & notA) >> 9)) & empty;
	}
	
	//checks for kings moves in both directions
	king_moves = ((((kings & notA) << 7) | ((kings & notH) << 9)) & empty) | 
				 ((((kings & notH) >> 7) | ((kings & notA) >> 9)) & empty); 
	
	if(bBlackMoves)
	{
		//captures moving up/black
		basic_captures = 
            ((((pieces & notAB) << 14) & empty) & (current_opponent << 7)) |
            ((((pieces & notGH) << 18) & empty) & (current_opponent << 9));
	}
	else
	{
		//captures moving down/red
		basic_captures = 
            ((((pieces & notAB) << 14) & empty) & (current_opponent << 7)) |
            ((((pieces & notGH) << 18) & empty) & (current_opponent << 9));
	}
	
	//check for captures in all 4 directions
	king_captures = ((((kings & notAB) << 14) & empty) & (current_opponent << 7)) |
					((((kings & notGH) << 18) & empty) & (current_opponent << 9)) | 
					((((kings & notGH) >> 14) & empty) & (current_opponent >> 7)) |
					((((kings & notAB) >> 18) & empty) & (current_opponent >> 9));
	return 	basic_moves | king_moves | basic_captures | king_captures;
}

//function for checking if a move is valid or not
static uint64_t game_check_if_move_valid(GameBoard *board, int piece_index, bool bBlackToMove)
{
	uint64_t piece = 1ULL << piece_index;
	uint64_t occupied = board_occupied(board);
	uint64_t empty = board_empty(board);
	
	uint64_t current_side = 0;
	uint64_t current_opponent = 0;
	
	if(bBlackToMove)
	{
		current_side = board->black;
		current_opponent = board->red;
	}
	else
	{
		current_side = board->red;
		current_opponent = board->black;
	}
	
	//check if its our piece
	if((current_side & piece) == 0)
	{
		return false; 
	}
	
	uint64_t king = board->king_places & piece;
	
	uint64_t moves = 0;
	uint64_t captures = 0;
	
	//moves up black/king
	if(bBlackToMove || king)
	{
		//shifts for black side 
		moves |= (((piece & notA) << 7) & empty);
		moves |= (((piece & notH) << 9) & empty);
		
        uint64_t dest = ((piece & notAB) << 14) & empty;
        if ((current_opponent & (piece << 7)) != 0) 
		{
			captures |= dest;
		}

        dest = (((piece & notGH) << 18) & empty);
        if ((current_opponent & (piece << 9)) != 0)
		{
			captures |= dest;
		}
	}
	
	//moves down red/king
	if((!bBlackToMove) || king)
	{
		//shifts for red side
		moves |= (((piece & notH) >> 7) & empty);
        moves |= (((piece & notA) >> 9) & empty);

        uint64_t dest = (((piece & notGH) >> 14) & empty);
        if ((current_opponent & (piece >> 7)) != 0)
		{
			captures |= dest;
		}

        dest = (((piece & notAB) >> 18) & empty);
        if ((current_opponent & (piece >> 9)) != 0)
		{
			captures |= dest;
		}
	}
	
	//return all the legal moves for the piece
	return moves | captures;
}

static uint64_t game_jump_piece(uint64_t piece, uint64_t destination)
{
	if(destination == (piece >> 14))
	{
		return piece >> 7;
	}
	if(destination == (piece >> 18))
	{
		return piece >> 9;
	}
	if((destination >> 14) == piece)
	{
		return destination >> 7;
	}
	if((destination >> 18) == piece)
	{
		return destination >> 9;
	}
	
	return 0;
}

bool apply_move(GameBoard *board, int piece_index, int destination_index, bool bBlackToMove)
{
	//gathering information so we can check if the move is valid
	uint64_t legal = game_check_if_move_valid(board, piece_index, bBlackToMove);
	uint64_t piece = 1ULL << piece_index;
	uint64_t destination = 1ULL << destination_index;
	
	//not a legal move
	if((legal & destination) == 0)
	{
		return false;
	}
	
	uint64_t *current_team;
	uint64_t *current_opponent;
	
	//make it easier to write logic for this section by having direct reference to current team and opponent regardly to whose turn it is
	if(bBlackToMove)
	{
		current_team = &board->black;
		current_opponent = &board->red;
	}
	else
	{
		current_team = &board->red;
		current_opponent = &board->black;
	}
	
	//make the move
	*current_team &= ~piece;
	*current_team |= destination;
	
	//if it is a king update the king board as well
	if((board->king_places & piece) != 0)
	{
		board->king_places &= ~piece;
		board->king_places |= destination;
	}
	
	//check if we need to use the jump logic
	uint64_t jumped = game_jump_piece(piece, destination);
	if(jumped != 0 && ((*current_opponent & jumped) != 0))
	{
		*current_opponent &= ~jumped;
		board->king_places &= ~jumped;
	}
	
	if(bBlackToMove)
	{
		if((destination & RANK7) != 0)
		{
			board->king_places |= destination;
		}
	}
	else if((destination & RANK0) != 0)
	{
		board->king_places |= destination;
	}
	
	return true;
}

//----------------------------------GAME END----------------------------------

bool game_check_victory(const GameBoard *board, bool black_turn)
{
	//if either black or red is out of pieces the game is over
	if(board->black == 0)
	{
		return true;
	}
	if(board->red == 0)
	{
		return true;
	}
	
	if(black_turn)
	{
		//current player black does not have moves so game is over
		if(find_possible_moves(board, true) == 0)
		{
			return true;
		}
	}
	else
	{
		//current player red does not have moves so game is over
		if(find_possible_moves(board, false) == 0)
		{
			return true;
		}
	}
	
	//game is still going if one side still has pieces and either side still has moves to make
	return false;
}


//----------------------------------DISPLAY AND DEBUG----------------------------------

//prints just the basic binary of the specified board
static void game_print_binary(uint64_t value)
{
	for(int i = 63; i >= 0; i--)
	{
		//shift to the right one until we reach the end
		putchar(((value >> i) & 1ULL) ? '1' : '0');
		//spacer if there is more to come
		if(i % 4 == 0 && i != 0) 
		{
			putchar('_');
		}
	}
}

//debug statement for printing the bitboard themselves
void game_print_bitboards(const GameBoard *board)
{
	printf("red : "); game_print_binary(board->red); printf("\n");
	printf("black : "); game_print_binary(board->black); printf("\n");
	printf("kings : "); game_print_binary(board->king_places); printf("\n");
}

//print so the user knows what the board position currently is
void game_print_board(const GameBoard *board)
{
	printf("\n   a b c d e f g h\n");
	for(int row = 7; row >= 0; --row)
	{
		printf("%d  ", (row+1));
		int col;
		for(col = 0; col < 8; ++col)
		{
			int index = row * 8 + col;
			char char_to_set = '.';
			uint64_t currentVal = BITVALUE(index);
			if(board->red & currentVal)
			{
				if(board->king_places & currentVal)
				{
					char_to_set = 'R';
				}
				else
				{
					char_to_set = 'r';
				}
			}
			else if(board->black & currentVal)
			{
				if(board->king_places & currentVal)
				{
					char_to_set = 'B';
				}
				else
				{
					char_to_set = 'b';
				}	
			}
			printf("%c ", char_to_set);
		}
		printf("\n");
	}
}