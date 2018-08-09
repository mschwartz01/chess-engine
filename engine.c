#include <stdio.h>

struct position {
	unsigned long long P, N, B, R, Q, K;
	unsigned long long p, n, b, r, q, k;
};

void print_board(struct position *);

int main() {
	struct position starting_pos = {
		0x000000000000FF00, 0x0000000000000042, 0x0000000000000024, 0x0000000000000081, 0x0000000000000010, 0x0000000000000008,
		0x00FF000000000000, 0x4200000000000000, 0x2400000000000000, 0x8100000000000000, 0x1000000000000000, 0x0800000000000000
	};
	print_board(&starting_pos);
	return 0;
}

void print_board(struct position *pos) {
	char board[] = "________________________________________________________________";
	unsigned long long piece;
	int location;

	for (piece = pos->P, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'P';
		}
	}
	for (piece = pos->N, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'N';
		}
	}
	for (piece = pos->B, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'B';
		}
	}
	for (piece = pos->R, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'R';
		}
	}
	for (piece = pos->Q, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'Q';
		}
	}
	for (piece = pos->K, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'K';
		}
	}

	for (piece = pos->p, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'p';
		}
	}
	for (piece = pos->n, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'n';
		}
	}
	for (piece = pos->b, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'b';
		}
	}
	for (piece = pos->r, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'r';
		}
	}
	for (piece = pos->q, location = 0; piece; piece >>= 1, location++) {
		if (piece & 1) {
			board[63-location] = 'q';
		}
	}
	for (piece = pos->k, location = 0; piece; piece >>= 1, location++) {
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
