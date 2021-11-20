/*
	* HEADER SUDOKU
	* Author : Jonas S.
	* Date   : 20/11/2021
	! DESCRIPTION
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef _SUDOKU_H_
#define _SUDOKU_H_


// DEFINES
#define BOARD_SIZE 3
#define SCREEN_SIZE 1000
#define FONT_SIZE 80
#define SEP 15

#define COLOR_SEP RED
#define COLOR_SSEP BLUE


typedef struct
{
	uint16_t size, w, h; // Size of one box side 3 is normal 
	uint16_t* data;
}board;

// PROTOTYPE FUNCTIONS
board* create_board(uint16_t size);
void destroy_board(board* b);
void fill_board(board b, uint32_t val);
void show_board(board b);

#endif