#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	TOKEN_EOF,

	TOKEN_IDENTIFIER,

	TOKEN_INT_LITERAL,
	TOKEN_FLOAT_LITERAL,
	TOKEN_STRING_LITERAL,

	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_MOD,

	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACE,
	TOKEN_RBRACE,

	TOKEN_SEMICOLON
} TokenType;

typedef struct {
	TokenType type;
	const char* start;
	size_t length;
} Token;

typedef struct {
	const char* source;
	size_t position;
} Lexer;

void lexer_init(Lexer* lexer, const char* source) {
	lexer->source = source;
	lexer->position = 0;
}

Token* lexer_next(Lexer* lexer) {
	Token* token = malloc(sizeof(Token));
	const char* source = lexer->source;

	int curr_max = 10;
	int c;
	while ((c = source[lexer->position]) != '\0') {
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
			if (c == '+') {
				token->type = TOKEN_PLUS;
			} else if (c == '-') {
				token->type = TOKEN_MINUS;
			} else if (c == '*') {
				token->type = TOKEN_STAR;
			} else if (c == '/') {
				token->type = TOKEN_SLASH;
			} else if (c == '%') {
				token->type = TOKEN_MOD;
			}
			token->length = 1;
			token->start = &source[lexer->position++];
			return token;
			// emit operator
		} else if (c == '{' || c == '}' || c == '(' || c == ')') {
			if (c == '{') {
				token->type = TOKEN_LBRACE;
			} else if (c == '}') {
				token->type = TOKEN_RBRACE;
			} else if (c == '(') {
				token->type = TOKEN_LPAREN;
			} else if (c == ')') {
				token->type = TOKEN_RPAREN;
			}
			token->length = 1;
			token->start = &source[lexer->position++];
			return token;
		} else if (isalpha(c) || c == '_') {
			token->type = TOKEN_IDENTIFIER;
			size_t start = lexer->position;
			token->start = &lexer->source[start];
			while ((c = source[lexer->position]) != '\0' &&
			       (isalnum(c) || c == '_')) {
				lexer->position++;
			}
			token->length = lexer->position - start;
			return token;

		} else if (isdigit(c)) {
			token->type = TOKEN_INT_LITERAL;
			size_t start = lexer->position;
			token->start = &lexer->source[start];
			while (isdigit(lexer->source[lexer->position])) {
				lexer->position++;
			}
			if (lexer->source[lexer->position] == '.') {
				token->type = TOKEN_FLOAT_LITERAL;
				while (isdigit(lexer->source[lexer->position])) {
					lexer->position++;
				}
			}
			token->length = lexer->position - start;
			return token;
		}
		lexer->position++;
	}

	token->type = TOKEN_EOF;
	token->length = 0;
	token->start = &lexer->source[lexer->position];
	return token;
}
