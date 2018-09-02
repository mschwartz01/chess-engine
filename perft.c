#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

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

void run_perft_tests(char *test_pos) {
	char *p = test_pos;
	char fen[100];
	int i;
	for (i = 0; *p != ';'; i++, p++) {
		fen[i] = *p;
	}
	fen[i-1] = '\0';
	struct position pos = parse_fen(fen);
	p+=2;
	int correct_values[10];
	char num[20];
	int depth = 1;
	while (*p != '\n') {
		for (; *p != ' '; p++) {
			;
		}
		p++;
		for (i = 0; *p != ';' && *p != '\n'; i++, p++) {
			num[i] = *p;
		}
		num[i-1] = '\0';
		correct_values[depth-1] = atoi(num);
		depth++;
	}
	int result;
	for (int j = 0; j < depth - 1; j++) {
		result = 0;
		perft(j + 1, &pos, &result);
		if (result != correct_values[j]) {
			printf("ErrorErrorErrorErrorErrorErrorErrorError\nErrorErrorErrorErrorErrorErrorErrorError\nErrorErrorErrorErrorErrorErrorErrorError\n");
		} else {
			printf("%d == %d\n", result, correct_values[j]);
		}
	}
}

void run_perft_tests_from_file(FILE *file) {
	char line[250];
	int i = 0;
	char c;
	int test_num = 1;
	int j = 1;
	while (c != EOF) {
		i = 0;
		while ((c = fgetc(file)) != '\n') {
			line[i++] = c;
		}
		line[i] = '\n';
		line[i+1] = '\0';
		printf("\nFEN %d\n", j++);
		run_perft_tests(line);
	}
}
