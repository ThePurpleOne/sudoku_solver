/*
	* CODE SUDOKU
	* Author : Jonas S.
	* Date   : 20/11/2021
	! SUDOKU GAME
*/

#include "../include/sudoku.h"

#include <math.h>

#define TERM_GREEN  "\x1B[32m"
#define TERM_RED "\x1B[31m"
#define TERM_WHITE "\x1B[37m"



// ---------------------------
// ---------- UTILZ ----------
// ---------------------------

/**
 * @brief Get the index for a flat array from x y
 * 
 * @param x
 * @param y
 * @return uint32_t
 */
uint32_t index_to_flat(int x, int y)
{
	return y * BOARD_SIZE + x; 
}

/**
 * @brief Malloc and check for an error
 * 
 * @param n
 * @return void*
 */
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

/**
 * @brief Free the momory allocated to a board
 * 
 * @param b
 */
void destroy_board(board* b)
{
	free(b->data);
	free(b);
}

// ------------------------------------
// ------------------------------------
// ------------------------------------


// --------------------------------------
// ---------- BOARD MANAGEMENT ----------
// --------------------------------------

/**
 * @brief Set a number in the board
 * 
 * @param b 
 * @param x
 * @param y
 * @param val
 */
bool try_set_number(board b, uint32_t x, uint32_t y, uint32_t val)
{
	if(val == 0)
	{
		b.data[index_to_flat(x, y)] = val;
		return false;
	}

	if(x > b.w || y > b.h)
		return false;
	
	if (!is_valid(b, val, x, y))
		return false;

	b.data[index_to_flat(x, y)] = val;
	return true;
}

/**
 * @brief Allocate memory for a board and returns a pointer to it
 * 
 * @param size
 * @return board*
 */
board* create_board(uint16_t size)
{
	board* b = safe_malloc(sizeof(board));
	b->size = size;
	b->w = b->h = b->size;
	b->data = safe_malloc( (b->w)*(b->h) * sizeof(uint16_t) );
	return b;
}

/**
 * @brief Fill a given board with a value
 * 
 * @param b
 * @param val
 */
void fill_board(board b, uint32_t val)
{
	for (int i = 0; i < b.w; i++)
	{
		for (int j = 0; j < b.h; j++)
		{
			b.data[index_to_flat(i, j)] = val;
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
			number = b.data[index_to_flat(i, j)];
			xp = cell_s * i + cell_s / 2 - FONT_SIZE / 4;
			yp = cell_s * j + cell_s / 2 - FONT_SIZE / 2;
			if(number != 0 && number < 10)
			{
				sprintf(text, "%d", number);
				DrawText(text, xp, yp, FONT_SIZE, WHITE);
			}
		}
	}
}

// ------------------------------------
// ------------------------------------
// ------------------------------------



// --------------------------------------
// --------------- SUDOKU ---------------
// --------------------------------------

/**
 * @brief Change the value aimed by the mouse
 * 
 * @param mouse_pos
 * @param ascii_value
 */
void change_value_mouse(board b, Vector2 mouse_pos, uint32_t ascii_value)
{

	// KEY_ZERO            = 48,       // Key: 0
	// KEY_ONE             = 49,       // Key: 1
	// KEY_TWO             = 50,       // Key: 2
	// KEY_THREE           = 51,       // Key: 3
	// KEY_FOUR            = 52,       // Key: 4
	// KEY_FIVE            = 53,       // Key: 5
	// KEY_SIX             = 54,       // Key: 6
	// KEY_SEVEN           = 55,       // Key: 7
	// KEY_EIGHT           = 56,       // Key: 8
	// KEY_NINE            = 57,       // Key: 9


	// KEY_KP_0            = 320,      // Key: Keypad 0
	// KEY_KP_1            = 321,      // Key: Keypad 1
	// KEY_KP_2            = 322,      // Key: Keypad 2
	// KEY_KP_3            = 323,      // Key: Keypad 3
	// KEY_KP_4            = 324,      // Key: Keypad 4
	// KEY_KP_5            = 325,      // Key: Keypad 5
	// KEY_KP_6            = 326,      // Key: Keypad 6
	// KEY_KP_7            = 327,      // Key: Keypad 7
	// KEY_KP_8            = 328,      // Key: Keypad 8
	// KEY_KP_9            = 329,      // Key: Keypad 9

	// GET POS FROM MOUSE X AND Y
	uint32_t xpos = mouse_pos.x * BOARD_SIZE / SCREEN_SIZE;
	uint32_t ypos = mouse_pos.y * BOARD_SIZE / SCREEN_SIZE;

	// Check if value is valid
	// Takes keypad too
	if( ( (ascii_value >= 48 ) && (ascii_value < 57) ) ||
		( (ascii_value >= 320) && (ascii_value <= 329) ))
	{
		try_set_number(b, xpos, ypos, ascii_value % 272 % 48);
	}

}

/**
 * @brief Check if a number can be placed at a pos
 * 
 * @param b
 * @param n
 * @param x
 * @param y
 * @return true
 * @return false
 */
bool is_valid(board b, uint32_t n, uint32_t x, uint32_t y)
{
	// Check row
	for (int i = 0; i < b.w; i++)
	{
		if( n == b.data[index_to_flat(i, y)])
			return false;
	}

	// Check col
	for (int i = 0; i < b.h; i++)
	{
		if( n == b.data[index_to_flat(x, i)])
			return false;
	}

	//Check square - should use sqrt of size but fuckit
	int sq_x = x / 3;
	int sq_y = y / 3;

	for (int i = sq_x * 3; i < sq_x * 3 + 3; i++)
	{
		for (int j = sq_y * 3; j < sq_y * 3 + 3; j++)
		{
			if( n == b.data[index_to_flat(i, j)])
				return false;
		}
	}

	return true;
}


/**
 * @brief Get empty coord
 * 		  Returns false if not found
 * @param b
 * @param x
 * @param y
 * @return true
 * @return false
 */
bool get_empty_index(board b, uint32_t *x, uint32_t *y)
{
	for (int i = 0; i < b.w; i++)
	{
		for (int j = 0; j < b.h; j++)
		{
			if( b.data[index_to_flat(i, j)] == 0)
			{
				*x = i;
				*y = j;
				return true;
			}				
		}
	}
	return false; // solved
}

/**
 * @brief Solve a board, 
 * 
 * @param b
 * @return true
 * @return false
 */
bool solve_board(board b)
{
	uint32_t empty_x, empty_y;
	if(!get_empty_index(b, &empty_x, &empty_y))
	{   
		return true; // FINISHED SOLVING
	}

	// Try values from [1 to 9] in that empty pos
	for (int i = 0; i < 10; i++)
	{
		// Try to put i in empty pos
		if (try_set_number(b, empty_x, empty_y, i)) // Worked
		{
			if(solve_board(b)) // Try again with updated board
			{
				return true;
			}
			else // Can't place anything anymore
			{
				// Backtrack - Reset the last element we tried
				try_set_number(b, empty_x, empty_y, 0);
			}
		}
	}
	return false;
}

// ------------------------------------
// ------------------------------------
// ------------------------------------