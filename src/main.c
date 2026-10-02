#include "lexer.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int i = 0, c = 0;
	FILE* file = fopen("test.unn", "r");
	Lexer* lexer = malloc(sizeof(Lexer));

	if (!file) {
		perror("test.unn");
		return 1;
	}
	int count = 100;
	char* source = malloc(count * sizeof(char));
	while (source && (c = fgetc(file)) != EOF) {
		source[i++] = (char)c;
		if (i == count - 1) {
			count *= 2;
			source = realloc(source, count * sizeof(char));
		}
	}
	source[i++] = '\0';
	fclose(file);

	lexer_init(lexer, source);

	Token* token;

	i = 0;
	while ((token = lexer_next(lexer)) != NULL) {
		printf("i:%d Token: %s Content: %.*s\n", i, token_type_name(token->type),
		       (int)token->length, token->start);
		free(token);
		i++;
		if (i == 100) break;
	}

	Parser* parser = malloc(sizeof(Parser));
	parser_init(parser, lexer);

	free(lexer);
	free(parser);

	return 0;
}
