/*
	* CODE SUDOKU
	* Author : Jonas S.
	* Date   : 20/11/2021
	! SUDOKU GAME
*/

#include "../include/sudoku.h"

#include "raylib.h"
#include <math.h>

// UTILS
void* safe_malloc(size_t n)
{
	void* p = malloc(n);
	if(p == NULL)
	{
		printf("ERROR ALLOCATING MEMORY\n");
		exit(0);
	}
	return p;
}

board* create_board(uint16_t size)
{
	board* b = safe_malloc(sizeof(board));
	b->size = size;
	b->w = b->h = b->size * b->size;
	b->data = safe_malloc( (b->w)*(b->h) * sizeof(uint16_t) );
	return b;
}

void fill_board(board b, uint32_t val)
{
	for (int i = 0; i < b.w; i++)
	{
		for (int j = 0; j < b.h; j++)
		{
			b.data[i * b.w + j] = val;
		}
	}

	// SHOW IN TERM
	// for (int i = 0; i < b.w; i++)
	// {
	// 	for (int j = 0; j < b.h; j++)
	// 	{
	// 		printf("|%ld|", b.data[i * b.w + j]);
	// 	}
	// 	printf("\n");
	// }
}

void destroy_board(board* b)
{
	free(b->data);
	free(b);
}

/**
 * @brief HARD CODED AND UGLY
 * 
 * @param b
 */
void show_board(board b)
{
	// Draw rectangles
	uint32_t cell_s = SCREEN_SIZE / b.w;	
	for(int y = 0; y < b.w; y++)
	{
		for (int x = 0; x < b.h; x++)
		{
			DrawRectangleLinesEx(	(Rectangle){x * cell_s,
											y * cell_s,
											cell_s,
											cell_s},
									4,
									COLOR_SSEP);
		}
	}
	
	// Draw separations
	for (int i = 1; i < 3; i++)
	{
		// Horizontal lines
		DrawLineEx(	(Vector2){i * (3*cell_s), 0},
					(Vector2){i * (3*cell_s), SCREEN_SIZE},
					SEP,
					COLOR_SEP);

		// Vertical lines
		DrawLineEx(	(Vector2){0, i * (3*cell_s)},
					(Vector2){SCREEN_SIZE, i * (3*cell_s)},
					SEP,
					COLOR_SEP);
	}

	int xp, yp;
	int number;
	char text[2];
	// Draw numbers
	for (int i = 0; i < b.w; i++)
	{
		for (int j = 0; j < b.h; j++)
		{
			number = b.data[i * b.w + j];
			xp = cell_s * i + cell_s / 2 - FONT_SIZE / 4;
			yp = cell_s * j + cell_s / 2 - FONT_SIZE / 2;
			if(number != 0 && number < 10)
			{
				sprintf(text, "%ld", number);
				DrawText(text, xp, yp, FONT_SIZE, WHITE);
			}
		}
	}
}