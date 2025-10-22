#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "gameboard.h"
#include "bitmanipulation.h"

//board driver
int main(void)
{
	GameBoard game_board; 
	reset_game_board(&game_board);
	game_board_start(&game_board);
	
	bool b_running = true;
	
	while(b_running)
	{
		char user_input[32];
		bool b_current_match = true;
		bool b_black_turn = true;
		while(b_current_match)
		{
			game_print_board(&game_board);
			bool b_user_deciding = true;
			if(b_black_turn)
			{
				printf("Please enter the cordinate position of the piece you would like to move, you are playing as black!\n");
			}
			else
			{
				printf("Please enter the cordinate position of the piece you would like to move, you are playing as red!\n");
			}		
			int selected_piece = 0;
			int selected_destination = 0;
			//create a user_inputing to collect user input
			fgets(user_input, sizeof(user_input), stdin);
			printf("User selected piece: %s\n", user_input);
			selected_piece = game_board_to_index(user_input);
			printf("Select destination\n");
			fgets(user_input, sizeof(user_input), stdin);
			selected_destination =  game_board_to_index(user_input);
			if(apply_move(&game_board, selected_piece, selected_destination, b_black_turn))
			{
				b_black_turn = !b_black_turn;
				if(game_check_victory(&game_board, b_black_turn))
				{
					if(!b_black_turn)
					{
						printf("Black wins!\n");
					}
					else
					{
						printf("Red wins!\n");
					}
					b_current_match = false;
				}
			}
			else
			{
				printf("Attempted illegal move\n");
			}
		}
		puts("would you like to exit (q)? enter any other key to play again");
		fgets(user_input, sizeof(user_input), stdin);
		user_input[strcspn(user_input, "\n")] = '\0';
		if(!strcmp(user_input, "q") || !strcmp(user_input, "Q"))
		{
			b_running = false;
		}
		else
		{
			reset_game_board(&game_board);
			game_board_start(&game_board);
		}
	}	
	return 0;
}

/*
if(!user_inputcmp(user_input, "k") || !user_inputcmp(user_input, "K"))
{
	uint64_t availMoves = find_possible_moves(&game_board, true);
	uint64_t emptySpaces = board_empty(&game_board);
	printf("avail: ");
	PrintBinary(availMoves);
	printf("Board empty: ");
	PrintBinary(emptySpaces);
	printf("avail & empty spaces: ");
	PrintBinary((availMoves & emptySpaces));
}
*/