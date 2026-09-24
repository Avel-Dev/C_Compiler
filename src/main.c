#include "lexer.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* token_type_name(TokenType type) {
	switch (type) {
	case TOKEN_EOF:
		return "TOKEN_EOF";
	case TOKEN_IDENTIFIER:
		return "TOKEN_IDENTIFIER";

	case TOKEN_INT_LITERAL:
		return "TOKEN_INT_LITERAL";
	case TOKEN_FLOAT_LITERAL:
		return "TOKEN_FLOAT_LITERAL";
	case TOKEN_STRING_LITERAL:
		return "TOKEN_STRING_LITERAL";

	// Keywords
	case TOKEN_INT:
		return "TOKEN_INT";
	case TOKEN_FLOAT:
		return "TOKEN_FLOAT";
	case TOKEN_CHAR:
		return "TOKEN_CHAR";
	case TOKEN_VOID:
		return "TOKEN_VOID";
	case TOKEN_IF:
		return "TOKEN_IF";
	case TOKEN_ELSE:
		return "TOKEN_ELSE";
	case TOKEN_WHILE:
		return "TOKEN_WHILE";
	case TOKEN_FOR:
		return "TOKEN_FOR";
	case TOKEN_RETURN:
		return "TOKEN_RETURN";

	// Arithmetic operators
	case TOKEN_PLUS:
		return "TOKEN_PLUS";
	case TOKEN_MINUS:
		return "TOKEN_MINUS";
	case TOKEN_STAR:
		return "TOKEN_STAR";
	case TOKEN_SLASH:
		return "TOKEN_SLASH";
	case TOKEN_MOD:
		return "TOKEN_MOD";
	case TOKEN_INCREMENT:
		return "TOKEN_INCREMENT";
	case TOKEN_DECREMENT:
		return "TOKEN_DECREMENT";

	// Relational operators
	case TOKEN_EQUALS:
		return "TOKEN_EQUALS";
	case TOKEN_NOT_EQUALS:
		return "TOKEN_NOT_EQUALS";
	case TOKEN_LESS_THAN:
		return "TOKEN_LESS_THAN";
	case TOKEN_MORE_THAN:
		return "TOKEN_MORE_THAN";
	case TOKEN_LESS_EQUALS:
		return "TOKEN_LESS_EQUALS";
	case TOKEN_MORE_EQUALS:
		return "TOKEN_MORE_EQUALS";

	// Assignment
	case TOKEN_ASSIGN:
		return "TOKEN_ASSIGN";
	case TOKEN_ADD_ASSIGN:
		return "TOKEN_ADD_ASSIGN";
	case TOKEN_SUB_ASSIGN:
		return "TOKEN_SUB_ASSIGN";
	case TOKEN_SLASH_ASSIGN:
		return "TOKEN_SLASH_ASSIGN";
	case TOKEN_MOD_ASSIGN:
		return "TOKEN_MOD_ASSIGN";
	case TOKEN_MUL_ASSIGN:
		return "TOKEN_MUL_ASSIGN";

	// Logical
	case TOKEN_AND:
		return "TOKEN_AND";
	case TOKEN_OR:
		return "TOKEN_OR";
	case TOKEN_NOT:
		return "TOKEN_NOT";

	case TOKEN_LPAREN:
		return "TOKEN_LPAREN";
	case TOKEN_RPAREN:
		return "TOKEN_RPAREN";
	case TOKEN_LBRACE:
		return "TOKEN_LBRACE";
	case TOKEN_RBRACE:
		return "TOKEN_RBRACE";

	case TOKEN_SEMICOLON:
		return "TOKEN_SEMICOLON";
	case TOKEN_ERROR:
		return "TOKEN_ERROR";
	case TOKEN_COMMENT:
		return "TOKEN_COMMENT";

	default:
		return "TOKEN_UNKNOWN";
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
	while ((token = lexer_next(lexer)) != NULL) {
		printf("Token: %s Content: %.*s\n", token_type_name(token->type),
		       (int)token->length, token->start);
		free(token);
	}

	free(lexer);

	return 0;
}
