/*
	* CODE MAIN
	* Author : Jonas S.
	* Date   : 20/11/2021
	! Sudoku solver
*/

#include <stdio.h>
#include "../include/sudoku.h"

#include "raylib.h"

int main()
{
	const int screenWidth = SCREEN_SIZE;
	const int screenHeight = SCREEN_SIZE;

	InitWindow(screenWidth, screenHeight, "SUDOKU SOLVER");
	
	SetTargetFPS(60);

	board* b1 = create_board(BOARD_SIZE);
	fill_board(*b1, 0);

	while (!WindowShouldClose())
	{
		BeginDrawing(); // ! DRAWING

		ClearBackground(DARKGRAY);
		show_board(*b1);
		DrawFPS(10, 10); 
		
		change_value_mouse(*b1, GetMousePosition(), GetKeyPressed());

		if(IsKeyPressed(' '))
		{
			// uint32_t x, y;
			// get_empty_index(*b1, &x, &y);
			// printf("VAL[%d, %d]\n", x, y);
			solve_board(*b1);
		}

		EndDrawing(); // ! END DRAWING
	}

	CloseWindow();

	destroy_board(b1);
}