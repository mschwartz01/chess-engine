#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>

/*
**************
** engine.c **
**************
*/
#define MAX_MADE_MOVES 200

const unsigned long long RANK_1;
const unsigned long long RANK_2;
const unsigned long long RANK_4;
const unsigned long long RANK_5;
const unsigned long long RANK_7;
const unsigned long long RANK_8;
const unsigned long long FILE_A;
const unsigned long long FILE_B;
const unsigned long long FILE_G;
const unsigned long long FILE_H;
const unsigned long long A1;
const unsigned long long B1;
const unsigned long long C1;
const unsigned long long D1;
const unsigned long long E1;
const unsigned long long F1;
const unsigned long long G1;
const unsigned long long H1;
const unsigned long long A8;
const unsigned long long B8;
const unsigned long long C8;
const unsigned long long D8;
const unsigned long long E8;
const unsigned long long F8;
const unsigned long long G8;
const unsigned long long H8;

const unsigned long long BIT;

const int MAX_POSSIBLE_MOVES;

enum turn {WHITE, BLACK};

enum pieces {NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PAWN_EN_PASSANT, KING_CASTLE};

struct board {
	unsigned long long P, N, B, R, Q, K;
	unsigned long long p, n, b, r, q, k;
	unsigned long long white, black;
};

struct position {
	struct board board;
	int turn;
	int castle_permissions; //KQkq
	int en_passant; //-1 for no square, otherwise 0-63
};

struct move {
	int from, to;
	int promote;
};

struct move_info {
	struct move move;
	int move_color;
	int moved_piece;
	int captured_piece;
	int en_passant;
	int castle_permissions;
};

struct move_info made_moves[MAX_MADE_MOVES];

int num_made_moves;

void print_board(struct position *pos);

struct position parse_fen(char *fen);

int is_attacked(unsigned long long square_board, struct board *board, int turn);

void make_move(struct position *pos, struct move *move);

void take_back_move(struct position *pos);

void get_moves(struct position *pos, struct move moves[], int *num_moves);

void get_capture_moves(struct position *pos, struct move moves[], int *num_moves);

/*
*************
** perft.c **
*************
*/
void perft(int depth, struct position *pos, int *move_count);

void run_perft_tests(char *file);

void run_perft_tests_from_file(FILE *file);

/*
**************
** search.c **
**************
*/
struct move_score {
	struct move move;
	int score;
};

int evaluation(struct position *pos);

struct move_score maximizer(struct position *pos, int depth, int alpha, int beta);

struct move_score minimizer(struct position *pos, int depth, int alpha, int beta);

struct move_score maximizer_quiescence(struct position *pos, int alpha, int beta);

struct move_score minimizer_quiescence(struct position *pos, int alpha, int beta);

#endif
