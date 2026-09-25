#include "lexer.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* token_type_name(TokenType type) {
	switch (type) {
	case TOKEN_IDENTIFIER:
		return "TOKEN_IDENTIFIER";

	// Literals
	case TOKEN_INT_LITERAL:
		return "TOKEN_INT_LITERAL";
	case TOKEN_FLOAT_LITERAL:
		return "TOKEN_FLOAT_LITERAL";
	case TOKEN_STRING_LITERAL:
		return "TOKEN_STRING_LITERAL";
	case TOKEN_CHAR_LITERAL:
		return "TOKEN_CHAR_LITERAL";

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
	case TOKEN_ADD:
		return "TOKEN_ADD";
	case TOKEN_SUB:
		return "TOKEN_SUB";
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

	// Arithmetic assignment
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

	// Bitwise assignment
	case TOKEN_AND_ASSIGN:
		return "TOKEN_AND_ASSIGN";
	case TOKEN_OR_ASSIGN:
		return "TOKEN_OR_ASSIGN";
	case TOKEN_XOR_ASSIGN:
		return "TOKEN_XOR_ASSIGN";
	case TOKEN_LSHIFT_ASSIGN:
		return "TOKEN_LSHIFT_ASSIGN";
	case TOKEN_RSHIFT_ASSIGN:
		return "TOKEN_RSHIFT_ASSIGN";

	// Logical
	case TOKEN_AND:
		return "TOKEN_AND";
	case TOKEN_OR:
		return "TOKEN_OR";
	case TOKEN_NOT:
		return "TOKEN_NOT";

	// Bitwise
	case TOKEN_BIT_AND:
		return "TOKEN_BIT_AND";
	case TOKEN_BIT_OR:
		return "TOKEN_BIT_OR";
	case TOKEN_BIT_NOT:
		return "TOKEN_BIT_NOT";
	case TOKEN_BIT_XOR:
		return "TOKEN_BIT_XOR";
	case TOKEN_BIT_LSHIFT:
		return "TOKEN_BIT_LSHIFT";
	case TOKEN_BIT_RSHIFT:
		return "TOKEN_BIT_RSHIFT";

	// Parentheses / braces
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

	case TOKEN_LBRACKET:
		return "TOKEN_LBRACKET";

	case TOKEN_RBRACKET:
		return "TOKEN_RBRACKET";

	case TOKEN_COMMA:
		return "TOKEN_COMMA";

	case TOKEN_DOT:
		return "TOKEN_DOT";

	case TOKEN_COLON:
		return "TOKEN_COLON";

	case TOKEN_QUESTION:
		return "TOKEN_QUESTION";
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

	i = 0;
	while ((token = lexer_next(lexer)) != NULL) {
		printf("i:%d Token: %s Content: %.*s\n", i, token_type_name(token->type),
		       (int)token->length, token->start);
		free(token);
		i++;
		if (i == 100) break;
	}

	free(lexer);

	return 0;
}
