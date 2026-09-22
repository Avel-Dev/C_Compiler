#include "lexer.c"

#include <stdio.h>

int main() {
	int size = 10;
	FILE* file = fopen("test.unn", "r");
	TokenID** IDs = lexer(&size, file);

	for (int i = 0; i < 100 && IDs[i]; i++) {
		printf("%d %s ", i, IDs[i]->name);
	}
	return 0;
}
