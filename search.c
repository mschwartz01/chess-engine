#include <limits.h>
#include "engine.h"

int evaluation(struct position *pos) {
	unsigned long long piece;
	int value = 0;
	// add value for whtie pieces
	for (piece = pos->board.P; piece; piece >>= 1) {
		if (piece & 1) {
			value += 100;
		}
	}
	for (piece = pos->board.N; piece; piece >>= 1) {
		if (piece & 1) {
			value += 300;
		}
	}
	for (piece = pos->board.B; piece; piece >>= 1) {
		if (piece & 1) {
			value += 300;
		}
	}
	for (piece = pos->board.R; piece; piece >>= 1) {
		if (piece & 1) {
			value += 500;
		}
	}
	for (piece = pos->board.Q; piece; piece >>= 1) {
		if (piece & 1) {
			value += 900;
		}
	}

	// subtract value for black pieces
	for (piece = pos->board.p; piece; piece >>= 1) {
		if (piece & 1) {
			value -= 100;
		}
	}
	for (piece = pos->board.n; piece; piece >>= 1) {
		if (piece & 1) {
			value -= 300;
		}
	}
	for (piece = pos->board.b; piece; piece >>= 1) {
		if (piece & 1) {
			value -= 300;
		}
	}
	for (piece = pos->board.r; piece; piece >>= 1) {
		if (piece & 1) {
			value -= 500;
		}
	}
	for (piece = pos->board.q; piece; piece >>= 1) {
		if (piece & 1) {
			value -= 900;
		}
	}
	return value;
}

struct move_score maximizer(struct position *pos, int depth, int alpha, int beta) {
	if (depth == 0) {
		return maximizer_quiescence(pos, alpha, beta);
	}
	struct move moves[MAX_POSSIBLE_MOVES];
	int num_moves;
	struct move_score best_move = {{0, 0, 0}, INT_MIN};
	get_moves(pos, moves, &num_moves);
	for (int i = 0; i < num_moves; i++) {
		make_move(pos, &moves[i]);
		struct move_score value = {moves[i], minimizer(pos, depth - 1, alpha, beta).score};
		if (value.score > best_move.score) {
			best_move = value;
		}
		take_back_move(pos);
		if (best_move.score >= beta) {
			return best_move;
		}
		if (best_move.score > alpha) {
			alpha = best_move.score;
		}
	}
	return best_move;
}

struct move_score minimizer(struct position *pos, int depth, int alpha, int beta) {
	if (depth == 0) {
		return minimizer_quiescence(pos, alpha, beta);
	}
	struct move moves[MAX_POSSIBLE_MOVES];
	int num_moves;
	struct move_score best_move = {{0, 0, 0}, INT_MAX};
	get_moves(pos, moves, &num_moves);
	for (int i = 0; i < num_moves; i++) {
		make_move(pos, &moves[i]);
		struct move_score value = {moves[i], maximizer(pos, depth - 1, alpha, beta).score};
		if (value.score < best_move.score) {
			best_move = value;
		}
		take_back_move(pos);
		if (best_move.score <= alpha) {
			return best_move;
		}
		if (best_move.score < beta) {
			beta = best_move.score;
		}
	}
	return best_move;
}

struct move_score maximizer_quiescence(struct position *pos, int alpha, int beta) {
	struct move moves[MAX_POSSIBLE_MOVES];
	int num_moves;
	struct move_score best_move = {{0, 0, 0}, INT_MIN};
	get_capture_moves(pos, moves, &num_moves);
	int current_score = evaluation(pos);
	if (num_moves == 0) {
		return (struct move_score) {{0, 0, 0}, current_score};
	}
	for (int i = 0; i < num_moves; i++) {
		make_move(pos, &moves[i]);
		struct move_score value = {moves[i], minimizer_quiescence(pos, alpha, beta).score};
		if (value.score > best_move.score) {
			best_move = value;
		}
		take_back_move(pos);
		if (best_move.score >= beta) {
			return best_move;
		}
		if (best_move.score > alpha) {
			alpha = best_move.score;
		}
	}
	return current_score >= best_move.score ? (struct move_score) {{0, 0, 0}, current_score} : best_move;
}

struct move_score minimizer_quiescence(struct position *pos, int alpha, int beta) {
	struct move moves[MAX_POSSIBLE_MOVES];
	int num_moves;
	struct move_score best_move = {{0, 0, 0}, INT_MAX};
	get_capture_moves(pos, moves, &num_moves);
	int current_score = evaluation(pos);
	if (num_moves == 0) {
		return (struct move_score) {{0, 0, 0}, current_score};
	}
	for (int i = 0; i < num_moves; i++) {
		make_move(pos, &moves[i]);
		struct move_score value = {moves[i], maximizer_quiescence(pos, alpha, beta).score};
		if (value.score < best_move.score) {
			best_move = value;
		}
		take_back_move(pos);
		if (best_move.score <= alpha) {
			return best_move;
		}
		if (best_move.score < beta) {
			beta = best_move.score;
		}
	}
	return current_score <= best_move.score ? (struct move_score) {{0, 0, 0}, current_score} : best_move;
}
