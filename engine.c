#include <stdio.h>

const unsigned long long RANK_1 = 0x00000000000000FF;
const unsigned long long RANK_2 = 0x000000000000FF00;
const unsigned long long RANK_4 = 0x00000000FF000000;
const unsigned long long RANK_5 = 0x000000FF00000000;
const unsigned long long RANK_7 = 0x00FF000000000000;
const unsigned long long RANK_8 = 0xFF00000000000000;
const unsigned long long FILE_A = 0x8080808080808080;
const unsigned long long FILE_B = 0x4040404040404040;
const unsigned long long FILE_G = 0x0202020202020202;
const unsigned long long FILE_H = 0x0101010101010101;
const unsigned long long A1 = 0x0000000000000080;
const unsigned long long B1 = 0x0000000000000040;
const unsigned long long C1 = 0x0000000000000020;
const unsigned long long D1 = 0x0000000000000010;
const unsigned long long E1 = 0x0000000000000008;
const unsigned long long F1 = 0x0000000000000004;
const unsigned long long G1 = 0x0000000000000002;
const unsigned long long H1 = 0x0000000000000001;
const unsigned long long A8 = 0x8000000000000000;
const unsigned long long B8 = 0x4000000000000000;
const unsigned long long C8 = 0x2000000000000000;
const unsigned long long D8 = 0x1000000000000000;
const unsigned long long E8 = 0x0800000000000000;
const unsigned long long F8 = 0x0400000000000000;
const unsigned long long G8 = 0x0200000000000000;
const unsigned long long H8 = 0x0100000000000000;

const unsigned long long BIT = 1;
const int MAX_POSSIBLE_MOVES = 250;
const int MAX_MADE_MOVES = 200;

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

void print_board(struct position *);

void get_moves(struct position *pos, struct move moves[], int *num_moves);

struct position parse_fen(char *fen);

void make_move(struct position *pos, struct move *move);

void take_back_move(struct position *pos);

void perft(int depth, struct position *pos, int *move_count);

int main() {
	struct position starting_pos = {
		{
			0x000000000000FF00, 0x0000000000000042, 0x0000000000000024, 0x0000000000000081, 0x0000000000000010, 0x0000000000000008,
			0x00FF000000000000, 0x4200000000000000, 0x2400000000000000, 0x8100000000000000, 0x1000000000000000, 0x0800000000000000,
			0x000000000000FFFF, 0xFFFF000000000000
		},
		WHITE,
		15,
		-1
	};
	// 0  1  2  3  4  5  6  7
	// 8  9  10 11 12 13 14 15
	// 16 17 18 19 20 21 22 23
	// 24 25 26 27 28 29 30 31
	// 32 33 34 35 36 37 38 39
	// 40 41 42 43 44 45 46 47
	// 48 49 50 51 52 53 54 55
	// 56 57 58 59 60 61 62 63
	struct position p = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	int perft_move_count;
	for (int i = 1; i < 5; i++) {
		perft_move_count = 0;
		perft(i, &p, &perft_move_count);
		printf("%d\n", perft_move_count);
	}
	return 0;
}

void print_board(struct position *pos) {
	char board[] = "________________________________________________________________";
	unsigned long long piece;
	int location;
	struct board *pos_board = &(pos->board);

	for (piece = pos_board->P, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'P';
		}
	}
	for (piece = pos_board->N, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'N';
		}
	}
	for (piece = pos_board->B, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'B';
		}
	}
	for (piece = pos_board->R, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'R';
		}
	}
	for (piece = pos_board->Q, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'Q';
		}
	}
	for (piece = pos_board->K, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'K';
		}
	}

	for (piece = pos_board->p, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'p';
		}
	}
	for (piece = pos_board->n, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'n';
		}
	}
	for (piece = pos_board->b, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'b';
		}
	}
	for (piece = pos_board->r, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'r';
		}
	}
	for (piece = pos_board->q, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'q';
		}
	}
	for (piece = pos_board->k, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'k';
		}
	}

	for (int i = 0; i < 64; i++) {
		printf("%c", board[i]);
		if (i % 8 == 7) {
			printf("\n");
		}
	}
}

struct position parse_fen(char *fen) {
	struct position pos;
	struct board board = {
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0
	};
	int location = 0;
	// while (*fen != '\0')
	while (location < 64) {
		switch (*fen) {
			case '/':
				fen++;
				break;
			case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
				location += (*fen) - '0';
				fen++;
				break;
			case 'P':
				board.P |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'N':
				board.N |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'B':
				board.B |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'R':
				board.R |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'Q':
				board.Q |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'K':
				board.K |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'p':
				board.p |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'n':
				board.n |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'b':
				board.b |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'r':
				board.r |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'q':
				board.q |= BIT << (63 - location);
				location++;
				fen++;
				break;
			case 'k':
				board.k |= BIT << (63 - location);
				location++;
				fen++;
				break;
		}
	}
	board.white = board.P | board.N | board.B | board.R | board.Q | board.K;
	board.black = board.p | board.n | board.b | board.r | board.q | board.k;
	pos.board = board;
	fen++;
	pos.turn = (*fen++ == 'w') ? WHITE : BLACK;
	fen++;
	int castle_permissions = 0;
	for (; *fen != ' '; fen++) {
		switch (*fen) {
			case 'K':
				castle_permissions += 8;
				break;
			case 'Q':
				castle_permissions += 4;
				break;
			case 'k':
				castle_permissions += 2;
				break;
			case 'q':
				castle_permissions += 1;
				break;
		}
	}
	fen++;
	if (*fen == '-') {
		pos.en_passant = -1;
		fen++;
	} else {
		int file = *fen++ - 'a';
		int rank = '8' - *fen++;
		pos.en_passant = file + (8 * rank);
	}
	pos.castle_permissions = castle_permissions;
	return pos;
}

int is_attacked(unsigned long long square_board, struct board *board, int turn) {
	unsigned long long opponent_pawn_board, opponent_knight_board, opponent_bishop_board, opponent_rook_board, opponent_queen_board, opponent_king_board,
					   player_left_file, player_right_file;
	if (turn == WHITE) {
		opponent_pawn_board = board->p;
		opponent_knight_board = board->n;
		opponent_bishop_board = board->b;
		opponent_rook_board = board->r;
		opponent_queen_board = board->q;
		opponent_king_board = board->k;
		player_left_file = FILE_A;
		player_right_file = FILE_H;
	} else {
		opponent_pawn_board = board->P;
		opponent_knight_board = board->N;
		opponent_bishop_board = board->B;
		opponent_rook_board = board->R;
		opponent_queen_board = board->Q;
		opponent_king_board = board->K;
		player_left_file = FILE_H;
		player_right_file = FILE_A;
	}
	if (square_board & (turn == WHITE ? opponent_pawn_board >> 7 : opponent_pawn_board << 7) & ~player_right_file) {
		return 1;
	}
	if (square_board & (turn == WHITE ? opponent_pawn_board >> 9 : opponent_pawn_board << 9) & ~player_left_file) {
		return 1;
	}
	if ((((square_board << 17) & ~FILE_H) |
		((square_board << 15) & ~FILE_A) |
		((square_board << 10) & ~(FILE_G | FILE_H)) |
		((square_board << 6) & ~(FILE_A | FILE_B)) |
		((square_board >> 6) & ~(FILE_G | FILE_H)) |
		((square_board >> 10) & ~(FILE_A | FILE_B)) |
		((square_board >> 15) & ~FILE_H) |
		((square_board >> 17) & ~FILE_A)) &
		opponent_knight_board) {
		return 1;
	}
	unsigned long long board_copy;
	for (board_copy = square_board << 8; board_copy; board_copy <<= 8) {
		if (board_copy & (board->white | board->black)) {
			if (board_copy & (opponent_rook_board | opponent_queen_board)) {
				return 1;
			} else {
				break;
			}
		}
	}
	for (board_copy = square_board >> 8; board_copy; board_copy >>= 8) {
		if (board_copy & (board->white | board->black)) {
			if (board_copy & (opponent_rook_board | opponent_queen_board)) {
				return 1;
			} else {
				break;
			}
		}
	}
	for (board_copy = square_board << 1; board_copy & ~FILE_H; board_copy <<= 1) {
		if (board_copy & (board->white | board->black)) {
			if (board_copy & (opponent_rook_board | opponent_queen_board)) {
				return 1;
			} else {
				break;
			}
		}
	}
	for (board_copy = square_board >> 1; board_copy & ~FILE_A; board_copy >>= 1) {
		if (board_copy & (board->white | board->black)) {
			if (board_copy & (opponent_rook_board | opponent_queen_board)) {
				return 1;
			} else {
				break;
			}
		}
	}
	for (board_copy = square_board << 9; board_copy & ~FILE_H; board_copy <<= 9) {
		if (board_copy & (board->white | board->black)) {
			if (board_copy & (opponent_bishop_board | opponent_queen_board)) {
				return 1;
			} else {
				break;
			}
		}
	}
	for (board_copy = square_board << 7; board_copy & ~FILE_A; board_copy <<= 7) {
		if (board_copy & (board->white | board->black)) {
			if (board_copy & (opponent_bishop_board | opponent_queen_board)) {
				return 1;
			} else {
				break;
			}
		}
	}
	for (board_copy = square_board >> 7; board_copy & ~FILE_H; board_copy >>= 7) {
		if (board_copy & (board->white | board->black)) {
			if (board_copy & (opponent_bishop_board | opponent_queen_board)) {
				return 1;
			} else {
				break;
			}
		}
	}
	for (board_copy = square_board >> 9; board_copy & ~FILE_A; board_copy >>= 9) {
		if (board_copy & (board->white | board->black)) {
			if (board_copy & (opponent_bishop_board | opponent_queen_board)) {
				return 1;
			} else {
				break;
			}
		}
	}
	if ((((square_board << 9) & ~FILE_H) |
	   (square_board << 8) |
	   ((square_board << 7) & ~FILE_A) |
	   ((square_board << 1) & ~FILE_H) |
	   ((square_board >> 1) & ~FILE_A) |
	   ((square_board >> 7) & ~FILE_H) |
	   (square_board >> 8) |
	   ((square_board >> 9)  & ~FILE_A)) &
	    opponent_king_board ) {
		return 1;
	}
	return 0;
}

void make_move(struct position *pos, struct move *move) {
	struct board *board = &(pos->board);
	struct move_info info;
	info.move = *move;
	info.captured_piece = NONE;
	info.en_passant = pos->en_passant;
	info.castle_permissions = pos->castle_permissions;
	unsigned long long from_board = BIT << (63 - move->from);
	unsigned long long to_board = BIT << (63 - move->to);
	int player, opponent, direction;
	unsigned long long *player_board, *player_pawn_board, *player_knight_board, *player_bishop_board, *player_rook_board, *player_queen_board, *player_king_board,
					   *opponent_board, *opponent_pawn_board, *opponent_knight_board, *opponent_bishop_board, *opponent_rook_board, *opponent_queen_board, *opponent_king_board,
					   A_playerside, D_playerside, F_playerside, H_playerside;
	if (pos->turn == WHITE) {
		player = WHITE;
		player_board = &board->white;
		player_pawn_board = &board->P;
		player_knight_board = &board->N;
		player_bishop_board = &board->B;
		player_rook_board = &board->R;
		player_queen_board = &board->Q;
		player_king_board = &board->K;
		opponent = BLACK;
		opponent_board = &board->black;
		opponent_pawn_board = &board->p;
		opponent_knight_board = &board->n;
		opponent_bishop_board = &board->b;
		opponent_rook_board = &board->r;
		opponent_queen_board = &board->q;
		opponent_king_board = &board->k;
		A_playerside = A1;
		D_playerside = D1;
		F_playerside = F1;
		H_playerside = H1;
		direction = 1;
	} else {
		player = BLACK;
		player_board = &board->black;
		player_pawn_board = &board->p;
		player_knight_board = &board->n;
		player_bishop_board = &board->b;
		player_rook_board = &board->r;
		player_queen_board = &board->q;
		player_king_board = &board->k;
		opponent = WHITE;
		opponent_board = &board->white;
		opponent_pawn_board = &board->P;
		opponent_knight_board = &board->N;
		opponent_bishop_board = &board->B;
		opponent_rook_board = &board->R;
		opponent_queen_board = &board->Q;
		opponent_king_board = &board->K;
		A_playerside = A8;
		D_playerside = D8;
		F_playerside = F8;
		H_playerside = H8;
		direction = -1;
	}
	if ((from_board & *player_pawn_board) && (move->from - direction * 16 == move->to)) {
		pos->en_passant = move->from - direction * 8;
	} else {
		pos->en_passant = -1;
	}
	if ((from_board & *player_pawn_board) && (move->to == info.en_passant)) {
		*opponent_pawn_board &= ~(player == WHITE ? to_board >> 8 : to_board << 8);
		*opponent_board &= ~(player == WHITE ? to_board >> 8 : to_board << 8);
		info.captured_piece = PAWN_EN_PASSANT;
	}
	if (from_board & *player_pawn_board) {
		*player_pawn_board &= ~from_board;
		switch (move->promote) {
			case NONE:
				*player_pawn_board |= to_board;
				break;
			case KNIGHT:
				*player_knight_board |= to_board;
				break;
			case BISHOP:
				*player_bishop_board |= to_board;
				break;
			case ROOK:
				*player_rook_board |= to_board;
				break;
			case QUEEN:
				*player_queen_board |= to_board;
				break;
		}
		info.moved_piece = PAWN;
	}
	if (from_board & *player_knight_board) {
		*player_knight_board &= ~from_board;
		*player_knight_board |= to_board;
		info.moved_piece = KNIGHT;
	}
	if (from_board & *player_bishop_board) {
		*player_bishop_board &= ~from_board;
		*player_bishop_board |= to_board;
		info.moved_piece = BISHOP;
	}
	if (from_board & *player_rook_board) {
		*player_rook_board &= ~from_board;
		*player_rook_board |= to_board;
		info.moved_piece = ROOK;
	}
	if (from_board & *player_queen_board) {
		*player_queen_board &= ~from_board;
		*player_queen_board |= to_board;
		info.moved_piece = QUEEN;
	}
	if (from_board & *player_king_board) {
		*player_king_board &= ~from_board;
		*player_king_board |= to_board;
		if (from_board >> 2 == to_board) {
			*player_rook_board &= ~H_playerside;
			*player_rook_board |= F_playerside;
			*player_board &= ~H_playerside;
			*player_board |= F_playerside;
			info.moved_piece = KING_CASTLE;
		} else if (from_board << 2 == to_board) {
			*player_rook_board &= ~A_playerside;
			*player_rook_board |= D_playerside;
			*player_board &= ~A_playerside;
			*player_board |= D_playerside;
			info.moved_piece = KING_CASTLE;
		} else {
			info.moved_piece = KING;
		}
	}
	*player_board &= ~from_board;
	*player_board |= to_board;

	if (to_board & *opponent_pawn_board) {
		*opponent_pawn_board &= ~to_board;
		info.captured_piece = PAWN;
	} else if (to_board & *opponent_knight_board) {
		*opponent_knight_board &= ~to_board;
		info.captured_piece = KNIGHT;
	} else if (to_board & *opponent_bishop_board) {
		*opponent_bishop_board &= ~to_board;
		info.captured_piece = BISHOP;
	} else if (to_board & *opponent_rook_board) {
		*opponent_rook_board &= ~to_board;
		info.captured_piece = ROOK;
	} else if (to_board & *opponent_queen_board) {
		*opponent_queen_board &= ~to_board;
		info.captured_piece = QUEEN;
	} else if (to_board & *opponent_king_board) {
		*opponent_king_board &= ~to_board;
		info.captured_piece = KING;
	}
	*opponent_board &= ~to_board;
	info.move_color = player;
	pos->turn = opponent;
	if (move->from == 60) {
		pos->castle_permissions &= 3;
	} else if (move->from == 63 || move->to == 63) {
		pos->castle_permissions &= 7;
	} else if (move->from == 56 || move->to == 56) {
		pos->castle_permissions &= 11;
	} else if (move->from == 4) {
		pos->castle_permissions &= 12;
	} else if (move->from == 7 || move->to == 7) {
		pos->castle_permissions &= 13;
	} else if (move->from == 0 || move->to == 0) {
		pos->castle_permissions &= 14;
	}
	made_moves[num_made_moves++] = info;
}

void take_back_move(struct position *pos) {
	struct board *board = &(pos->board);
	struct move_info info = made_moves[--num_made_moves];
	unsigned long long from_board = BIT << (63 - info.move.from);
	unsigned long long to_board = BIT << (63 - info.move.to);
	pos->en_passant = info.en_passant;
	pos->castle_permissions = info.castle_permissions;
	unsigned long long *player_board, *player_pawn_board, *player_knight_board, *player_bishop_board, *player_rook_board, *player_queen_board, *player_king_board,
					   *opponent_board, *opponent_pawn_board, *opponent_knight_board, *opponent_bishop_board, *opponent_rook_board, *opponent_queen_board, *opponent_king_board,
					   A_playerside, D_playerside, F_playerside, H_playerside;
	int player, king_start_location;
	if (info.move_color == WHITE) {
		player = WHITE;
		player_board = &board->white;
		player_pawn_board = &board->P;
		player_knight_board = &board->N;
		player_bishop_board = &board->B;
		player_rook_board = &board->R;
		player_queen_board = &board->Q;
		player_king_board = &board->K;
		opponent_board = &board->black;
		opponent_pawn_board = &board->p;
		opponent_knight_board = &board->n;
		opponent_bishop_board = &board->b;
		opponent_rook_board = &board->r;
		opponent_queen_board = &board->q;
		opponent_king_board = &board->k;
		king_start_location = 60;
		A_playerside = A1;
		D_playerside = D1;
		F_playerside = F1;
		H_playerside = H1;
	} else {
		player = BLACK;
		player_board = &board->black;
		player_pawn_board = &board->p;
		player_knight_board = &board->n;
		player_bishop_board = &board->b;
		player_rook_board = &board->r;
		player_queen_board = &board->q;
		player_king_board = &board->k;
		opponent_board = &board->white;
		opponent_pawn_board = &board->P;
		opponent_knight_board = &board->N;
		opponent_bishop_board = &board->B;
		opponent_rook_board = &board->R;
		opponent_queen_board = &board->Q;
		opponent_king_board = &board->K;
		king_start_location = 4;
		A_playerside = A8;
		D_playerside = D8;
		F_playerside = F8;
		H_playerside = H8;
	}
	switch (info.moved_piece) {
		case PAWN:
			*player_pawn_board |= from_board;
			switch (info.move.promote) {
				case NONE:
					*player_pawn_board &= ~to_board;
					break;
				case KNIGHT:
					*player_knight_board &= ~to_board;
					break;
				case BISHOP:
					*player_bishop_board &= ~to_board;
					break;
				case ROOK:
					*player_rook_board &= ~to_board;
					break;
				case QUEEN:
					*player_queen_board &= ~to_board;
					break;
			}
			break;
		case KNIGHT:
			*player_knight_board |= from_board;
			*player_knight_board &= ~to_board;
			break;
		case BISHOP:
			*player_bishop_board |= from_board;
			*player_bishop_board &= ~to_board;
			break;
		case ROOK:
			*player_rook_board |= from_board;
			*player_rook_board &= ~to_board;
			break;
		case QUEEN:
			*player_queen_board |= from_board;
			*player_queen_board &= ~to_board;
			break;
		case KING:
			*player_king_board |= from_board;
			*player_king_board &= ~to_board;
			break;
		case KING_CASTLE:
			*player_king_board |= from_board;
			*player_king_board &= ~to_board;
			if (info.move.to == king_start_location + 2) {
				*player_rook_board |= H_playerside;
				*player_rook_board &= ~F_playerside;
				*player_board |= H_playerside;
				*player_board &= ~F_playerside;
			} else if (info.move.to == king_start_location - 2) {
				*player_rook_board |= A_playerside;
				*player_rook_board &= ~D_playerside;
				*player_board |= A_playerside;
				*player_board &= ~D_playerside;
			}
			break;
	}
	*player_board |= from_board;
	*player_board &= ~to_board;
	if (info.captured_piece) {
		switch (info.captured_piece) {
			case PAWN:
				*opponent_pawn_board |= to_board;
				*opponent_board |= to_board;
				break;
			case KNIGHT:
				*opponent_knight_board |= to_board;
				*opponent_board |= to_board;
				break;
			case BISHOP:
				*opponent_bishop_board |= to_board;
				*opponent_board |= to_board;
				break;
			case ROOK:
				*opponent_rook_board |= to_board;
				*opponent_board |= to_board;
				break;
			case QUEEN:
				*opponent_queen_board |= to_board;
				*opponent_board |= to_board;
				break;
			case KING:
				*opponent_king_board |= to_board;
				*opponent_board |= to_board;
				break;
			case PAWN_EN_PASSANT:
				*opponent_pawn_board |= (player == WHITE ? to_board >> 8 : to_board << 8);
				*opponent_board |= (player == WHITE ? to_board >> 8 : to_board << 8);
				break;
		}
	}
	pos->turn = player;
}

void get_moves(struct position *pos, struct move moves[], int *num_moves) {
	struct board *board = &(pos->board);
	unsigned long long moves_board;
	int location;
	*num_moves = 0;
	int player, king_start_location, kingside_castle_location, queenside_castle_location, direction;
	unsigned long long player_board, player_pawn_board, player_knight_board, player_bishop_board, player_rook_board, player_queen_board, player_king_board,
					   opponent_board, B_playerside, C_playerside, D_playerside, E_playerside, F_playerside, G_playerside,
					   promotion_rank, player_pawn_forward_2_rank, player_left_file, player_right_file;
	if (pos->turn == WHITE) {
		player = WHITE;
		player_board = board->white;
		player_pawn_board = board->P;
		player_knight_board = board->N;
		player_bishop_board = board->B;
		player_rook_board = board->R;
		player_queen_board = board->Q;
		player_king_board = board->K;
		opponent_board = board->black;
		king_start_location = 60;
		B_playerside = B1;
		C_playerside = C1;
		D_playerside = D1;
		E_playerside = E1;
		F_playerside = F1;
		G_playerside = G1;
		promotion_rank = RANK_8;
		player_pawn_forward_2_rank = RANK_4;
		player_left_file = FILE_A;
		player_right_file = FILE_H;
		direction = 1;
	} else {
		player = BLACK;
		player_board = board->black;
		player_pawn_board = board->p;
		player_knight_board = board->n;
		player_bishop_board = board->b;
		player_rook_board = board->r;
		player_queen_board = board->q;
		player_king_board = board->k;
		opponent_board = board->white;
		king_start_location = 4;
		B_playerside = B8;
		C_playerside = C8;
		D_playerside = D8;
		E_playerside = E8;
		F_playerside = F8;
		G_playerside = G8;
		promotion_rank = RANK_1;
		player_pawn_forward_2_rank = RANK_5;
		player_left_file = FILE_H;
		player_right_file = FILE_A;
		direction = -1;
	}
	// pawn forward one step, no promotion
	moves_board = (player == WHITE ? player_pawn_board << 8 : player_pawn_board >> 8) & ~(player_board | opponent_board) & ~promotion_rank;
	for (location = 0; moves_board; moves_board >>= 1, location++) {
		if (moves_board & 1) {
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 8, 63 - location, 0};
		}
	}
	// pawn forward two steps
	moves_board = (player == WHITE ? player_pawn_board << 8 : player_pawn_board >> 8) & ~(player_board | opponent_board) & ~promotion_rank;
	moves_board = (player == WHITE ? moves_board << 8 : moves_board >> 8) & ~(player_board | opponent_board) & player_pawn_forward_2_rank;
	for (location = 0; moves_board; moves_board >>= 1, location++) {
		if (moves_board & 1) {
			moves[(*num_moves)++] = (struct  move) {63 - location + direction * 16, 63 - location, 0};
		}
	}
	// pawn capture left, no promotion
	moves_board = (player == WHITE ? player_pawn_board << 9 : player_pawn_board >> 9) & opponent_board & ~player_right_file & ~promotion_rank;
	for (location = 0; moves_board; moves_board >>= 1, location++) {
		if (moves_board & 1) {
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 9, 63 - location, 0};
		}
	}
	// pawn capture right, no promotion
	moves_board = (player == WHITE ? player_pawn_board << 7 : player_pawn_board >> 7) & opponent_board & ~player_left_file & ~promotion_rank;
	for (location = 0; moves_board; moves_board >>= 1, location++) {
		if (moves_board & 1) {
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 7, 63 - location, 0};
		}
	}
	if (pos->en_passant != -1) {
		if (player_pawn_board & (BIT << (63 - pos->en_passant - direction * 7)) & ~player_right_file) {
			moves[(*num_moves)++] = (struct move) {pos->en_passant + direction * 7, pos->en_passant, 0};
		}
		if (player_pawn_board & (BIT << (63 - pos->en_passant - direction * 9)) & ~player_left_file) {
			moves[(*num_moves)++] = (struct move) {pos->en_passant + direction * 9, pos->en_passant, 0};
		}
	}
	// pawn promotion
	moves_board = (player == WHITE ? player_pawn_board << 8 : player_pawn_board >> 8) & ~(player_board | opponent_board) & promotion_rank;
	for (location = 0; moves_board; moves_board >>= 1, location++) {
		if (moves_board & 1) {
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 8, 63 - location, KNIGHT};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 8, 63 - location, BISHOP};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 8, 63 - location, ROOK};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 8, 63 - location, QUEEN};
		}
	}
	moves_board = (player == WHITE ? player_pawn_board << 9 : player_pawn_board >> 9) & opponent_board & ~player_right_file & promotion_rank;
	for (location = 0; moves_board; moves_board >>= 1, location++) {
		if (moves_board & 1) {
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 9, 63 - location, KNIGHT};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 9, 63 - location, BISHOP};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 9, 63 - location, ROOK};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 9, 63 - location, QUEEN};
		}
	}
	moves_board = (player == WHITE ? player_pawn_board << 7 : player_pawn_board >> 7) & opponent_board & ~player_left_file & promotion_rank;
	for (location = 0; moves_board; moves_board >>= 1, location++) {
		if (moves_board & 1) {
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 7, 63 - location, KNIGHT};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 7, 63 - location, BISHOP};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 7, 63 - location, ROOK};
			moves[(*num_moves)++] = (struct move) {63 - location + direction * 7, 63 - location, QUEEN};
		}
	}
	// knight
	unsigned long long knight_location;
	for (int i = 0; i < 64; i++) {
		if ((knight_location = player_knight_board & (BIT << i))){
			moves_board = (((knight_location << 17) & ~FILE_H) |
						  ((knight_location << 15) & ~FILE_A) |
						  ((knight_location << 10) & ~(FILE_G | FILE_H)) |
						  ((knight_location << 6) & ~(FILE_A | FILE_B)) |
						  ((knight_location >> 6) & ~(FILE_G | FILE_H)) |
						  ((knight_location >> 10) & ~(FILE_A | FILE_B)) |
						  ((knight_location >> 15) & ~FILE_H) |
						  ((knight_location >> 17) & ~FILE_A)) &
						  ~(player_board);
			for (location = 0; moves_board; moves_board >>= 1, location++) {
				if (moves_board & 1) {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - location, 0};
				}
			}
		}
	}
	// bishop
	unsigned long long bishop_location;
	for (int i = 0; i < 64; i++) {
		if ((bishop_location = player_bishop_board & (BIT << i))) {
			int j;
			// up-left
			for (bishop_location <<= 9, j = 1; bishop_location & ~FILE_H; bishop_location <<= 9, j++) {
				if (bishop_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i - (9 * j), 0};
					if (bishop_location & opponent_board) {
						break;
					}
				}
			}
			// up-right
			bishop_location = player_bishop_board & (BIT << i);
			for (bishop_location <<= 7, j = 1; bishop_location & ~FILE_A; bishop_location <<= 7, j++) {
				if (bishop_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i - (7 * j), 0};
					if (bishop_location & opponent_board) {
						break;
					}
				}
			}
			// down-left
			bishop_location = player_bishop_board & (BIT << i);
			for (bishop_location >>= 7, j = 1; bishop_location & ~FILE_H; bishop_location >>= 7, j++) {
				if (bishop_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i + (7 * j), 0};
					if (bishop_location & opponent_board) {
						break;
					}
				}
			}
			// down-right
			bishop_location = player_bishop_board & (BIT << i);
			for (bishop_location >>= 9, j = 1; bishop_location & ~FILE_A; bishop_location >>= 9, j++) {
				if (bishop_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i + (9 * j), 0};
					if (bishop_location & opponent_board) {
						break;
					}
				}
			}
		}
	}
	// rook
	unsigned long long rook_location;
	for (int i = 0; i < 64; i++) {
		if ((rook_location = player_rook_board & (BIT << i))) {
			int j;
			// left
			for (rook_location <<= 1, j = 1; rook_location & ~FILE_H; rook_location <<= 1, j++) {
				if (rook_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i - j, 0};
					if (rook_location & opponent_board) {
						break;
					}
				}
			}
			// right
			rook_location = player_rook_board & (BIT << i);
			for (rook_location >>= 1, j = 1; rook_location & ~FILE_A; rook_location >>= 1, j++) {
				if (rook_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i + j, 0};
					if (rook_location & opponent_board) {
						break;
					}
				}
			}
			// up
			rook_location = player_rook_board & (BIT << i);
			for (rook_location <<= 8, j = 1; rook_location; rook_location <<= 8, j++) {
				if (rook_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i - (8 * j), 0};
					if (rook_location & opponent_board) {
						break;
					}
				}
			}
			// down
			rook_location = player_rook_board & (BIT << i);
			for (rook_location >>= 8, j = 1; rook_location; rook_location >>= 8, j++) {
				if (rook_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i + (8 * j), 0};
					if (rook_location & opponent_board) {
						break;
					}
				}
			}
		}
	}
	// queen
	unsigned long long queen_location;
	for (int i = 0; i < 64; i++) {
		if ((queen_location = player_queen_board & (BIT << i))) {
			int j;
			// up-left
			for (queen_location <<= 9, j = 1; queen_location & ~FILE_H; queen_location <<= 9, j++) {
				if (queen_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i - (9 * j), 0};
					if (queen_location & opponent_board) {
						break;
					}
				}
			}
			// up-right
			queen_location = player_queen_board & (BIT << i);
			for (queen_location <<= 7, j = 1; queen_location & ~FILE_A; queen_location <<= 7, j++) {
				if (queen_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i - (7 * j), 0};
					if (queen_location & opponent_board) {
						break;
					}
				}
			}
			// down-left
			queen_location = player_queen_board & (BIT << i);
			for (queen_location >>= 7, j = 1; queen_location & ~FILE_H; queen_location >>= 7, j++) {
				if (queen_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i + (7 * j), 0};
					if (queen_location & opponent_board) {
						break;
					}
				}
			}
			// down-right
			queen_location = player_queen_board & (BIT << i);
			for (queen_location >>= 9, j = 1; queen_location & ~FILE_A; queen_location >>= 9, j++) {
				if (queen_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i + (9 * j), 0};
					if (queen_location & opponent_board) {
						break;
					}
				}
			}
			// left
			queen_location = player_queen_board & (BIT << i);
			for (queen_location <<= 1, j = 1; queen_location & ~FILE_H; queen_location <<= 1, j++) {
				if (queen_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i - j, 0};
					if (queen_location & opponent_board) {
						break;
					}
				}
			}
			// right
			queen_location = player_queen_board & (BIT << i);
			for (queen_location >>= 1, j = 1; queen_location & ~FILE_A; queen_location >>= 1, j++) {
				if (queen_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i + j, 0};
					if (queen_location & opponent_board) {
						break;
					}
				}
			}
			// up
			queen_location = player_queen_board & (BIT << i);
			for (queen_location <<= 8, j = 1; queen_location; queen_location <<= 8, j++) {
				if (queen_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i - (8 * j), 0};
					if (queen_location & opponent_board) {
						break;
					}
				}
			}
			// down
			queen_location = player_queen_board & (BIT << i);
			for (queen_location >>= 8, j = 1; queen_location; queen_location >>= 8, j++) {
				if (queen_location & player_board) {
					break;
				} else {
					moves[(*num_moves)++] = (struct move) {63 - i, 63 - i + (8 * j), 0};
					if (queen_location & opponent_board) {
						break;
					}
				}
			}
		}
	}
	// king
	unsigned long long king_location;
	king_location = 0;
	while (!(player_king_board & (BIT << king_location))) {
		king_location++;
	}
	if ((player_king_board << 9) & ~FILE_H & ~(player_board) && !is_attacked(player_king_board << 9, board, player)) {
		moves[(*num_moves)++] = (struct move) {63 - king_location, 63 - king_location - 9, 0};
	}
	if ((player_king_board << 8) & ~(player_board) && !is_attacked(player_king_board << 8, board, player)) {
		moves[(*num_moves)++] = (struct move) {63 - king_location, 63 - king_location - 8, 0};
	}
	if ((player_king_board << 7) & ~FILE_A & ~(player_board) && !is_attacked(player_king_board << 7, board, player)) {
		moves[(*num_moves)++] = (struct move) {63 - king_location, 63 - king_location - 7, 0};
	}
	if ((player_king_board << 1) & ~FILE_H & ~(player_board) && !is_attacked(player_king_board << 1, board, player)) {
		moves[(*num_moves)++] = (struct move) {63 - king_location, 63 - king_location - 1, 0};
	}
	if ((player_king_board >> 1) & ~FILE_A & ~(player_board) && !is_attacked(player_king_board >> 1, board, player)) {
		moves[(*num_moves)++] = (struct move) {63 - king_location, 63 - king_location + 1, 0};
	}
	if ((player_king_board >> 7) & ~FILE_H & ~(player_board) && !is_attacked(player_king_board >> 7, board, player)) {
		moves[(*num_moves)++] = (struct move) {63 - king_location, 63 - king_location + 7, 0};
	}
	if ((player_king_board >> 8) & ~(player_board) && !is_attacked(player_king_board >> 8, board, player)) {
		moves[(*num_moves)++] = (struct move) {63 - king_location, 63 - king_location + 8, 0};
	}
	if ((player_king_board >> 9) & ~FILE_A & ~(player_board) && !is_attacked(player_king_board >> 9, board, player)) {
		moves[(*num_moves)++] = (struct move) {63 - king_location, 63 - king_location + 9, 0};
	}
	// castle
	if (pos->castle_permissions & (pos->turn == WHITE ? 8 : 2) && !((player_board | opponent_board) & F_playerside) && !((player_board | opponent_board) & G_playerside) && !is_attacked(E_playerside, board, player) && !is_attacked(F_playerside, board, player) && !is_attacked(G_playerside, board, player)) {
		moves[(*num_moves)++] = (struct move) {king_start_location, king_start_location + 2, 0};
	}
	if (pos->castle_permissions & (pos->turn == WHITE ? 4 : 1) && !((player_board | opponent_board) & B_playerside) && !((player_board | opponent_board) & C_playerside) && !((player_board | opponent_board) & D_playerside) && !is_attacked(C_playerside, board, player) && !is_attacked(D_playerside, board, player) && !is_attacked(E_playerside, board, player)) {
		moves[(*num_moves)++] = (struct move) {king_start_location, king_start_location - 2, 0};
	}
	int j = *num_moves;
	*num_moves = 0;
	for (int i = 0; i < j; i++) {
		make_move(pos, &moves[i]);
		if (!is_attacked(player == WHITE ? pos->board.K : pos->board.k, board, player)) {
			moves[(*num_moves)++] = moves[i];
		}
		take_back_move(pos);
	}
	printf("%d\n", *num_moves);
	for (int i = 0; i < *num_moves; i++) {
		printf("%d %d\n", moves[i].from, moves[i].to);
	}
}

void perft(int depth, struct position *pos, int *move_count) {
	if (depth == 0) {
		(*move_count)++;
		return;
	}
	struct move moves[MAX_POSSIBLE_MOVES];
	int num_moves;
	get_moves(pos, moves, &num_moves);
	for (int i = 0; i < num_moves; i++) {
		make_move(pos, &moves[i]);
		perft(depth - 1, pos, move_count);
		take_back_move(pos);
	}
}
