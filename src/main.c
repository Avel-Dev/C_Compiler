#include "lexer.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* token_type_name(TokenType type) {
	switch (type) {
	case TOKEN_EOF:
		return "EOF";
	case TOKEN_IDENTIFIER:
		return "IDENTIFIER";
	case TOKEN_INT_LITERAL:
		return "INT_LITERAL";
	case TOKEN_FLOAT_LITERAL:
		return "FLOAT_LITERAL";
	case TOKEN_STRING_LITERAL:
		return "STRING_LITERAL";
	case TOKEN_PLUS:
		return "PLUS";
	case TOKEN_MINUS:
		return "MINUS";
	case TOKEN_STAR:
		return "STAR";
	case TOKEN_SLASH:
		return "SLASH";
	case TOKEN_MOD:
		return "MOD";
	case TOKEN_LPAREN:
		return "LPAREN";
	case TOKEN_RPAREN:
		return "RPAREN";
	case TOKEN_LBRACE:
		return "LBRACE";
	case TOKEN_RBRACE:
		return "RBRACE";
	case TOKEN_SEMICOLON:
		return "SEMICOLON";
	default:
		return "UNKNOWN";
	}
}

int main() {
	int i = 0, c = 0;
	FILE* file = fopen("test.unn", "r");
	Lexer* lexer = malloc(sizeof(Lexer));

	if (!file) {
		perror("test.unn");
		return 1;
	}

	char source[100];
	while (i < sizeof(source) - 1 && (c = fgetc(file)) != EOF) {
		source[i++] = (char)c;
	}
	source[i++] = '\0';
	fclose(file);

	lexer_init(lexer, source);

	Token* token;
	while ((token = lexer_next(lexer)) != NULL && token->type != TOKEN_EOF) {
		printf("Token: %s\n", token_type_name(token->type));
	}

	free(lexer);

	return 0;
}
