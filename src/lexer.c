#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	TOKEN_EOF,

	TOKEN_IDENTIFIER,

	// literals
	TOKEN_INT_LITERAL,
	TOKEN_FLOAT_LITERAL,
	TOKEN_STRING_LITERAL,
	TOKEN_CHAR_LITERAL,

	// Keywords
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_CHAR,
	TOKEN_VOID,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_WHILE,
	TOKEN_FOR,
	TOKEN_RETURN,

	// Arithmetic operators
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_MOD,
	TOKEN_INCREMENT,
	TOKEN_DECREMENT,

	// relational operators
	TOKEN_EQUALS,
	TOKEN_NOT_EQUALS,
	TOKEN_LESS_THAN,
	TOKEN_MORE_THAN,
	TOKEN_LESS_EQUALS,
	TOKEN_MORE_EQUALS,

	// Assignment
	TOKEN_ASSIGN,
	TOKEN_ADD_ASSIGN,
	TOKEN_SUB_ASSIGN,
	TOKEN_SLASH_ASSIGN,
	TOKEN_MOD_ASSIGN,
	TOKEN_MUL_ASSIGN,

	// Logical
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_NOT,

	// BITWISE
	TOKEN_BIT_AND,
	TOKEN_BIT_OR,
	TOKEN_BIT_NOT,
	TOKEN_BIT_XOR,
	TOKEN_BIT_LSHIFT,
	TOKEN_BIT_RSHIFT,

	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACE,
	TOKEN_RBRACE,

	TOKEN_SEMICOLON,
	TOKEN_ERROR,

	TOKEN_COMMENT
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

TokenType keyword_type(const char* start, size_t length) {
	if (length == 3 && strncmp(start, "int", 3) == 0) {
		return TOKEN_INT;
	} else if (length == 4 && strncmp(start, "float", 4) == 0) {
		return TOKEN_FLOAT;
	} else if (length == 4 && strncmp(start, "char", 4) == 0) {
		return TOKEN_CHAR;
	} else if (length == 4 && strncmp(start, "void", 4) == 0) {
		return TOKEN_VOID;
	} else if (length == 2 && strncmp(start, "if", 2) == 0) {
		return TOKEN_IF;
	} else if (length == 4 && strncmp(start, "else", 4) == 0) {
		return TOKEN_ELSE;
	} else if (length == 5 && strncmp(start, "while", 5) == 0) {
		return TOKEN_WHILE;
	} else if (length == 3 && strncmp(start, "for", 3) == 0) {
		return TOKEN_FOR;
	} else if (length == 6 && strncmp(start, "return", 6) == 0) {
		return TOKEN_RETURN;
	}

	return TOKEN_IDENTIFIER;
}

Token* lexer_next(Lexer* lexer) {
	if (lexer->source[lexer->position] == '\0') return NULL;
	Token* token = malloc(sizeof(Token));
	const char* source = lexer->source;

	unsigned char c;
	while ((c = source[lexer->position]) != '\0') {
		if (isspace(c)) {
			lexer->position++;
			continue;
		}
		if (isalpha(c) || c == '_') {
			size_t start = lexer->position;
			token->start = &source[start];
			while ((c = source[lexer->position]) != '\0' &&
			       (isalnum(c) || c == '_')) {
				lexer->position++;
			}
			token->length = lexer->position - start;
			token->type = keyword_type(token->start, token->length);
			return token;

		} else if (isdigit(c)) {
			token->type = TOKEN_INT_LITERAL;
			size_t start = lexer->position;
			token->start = &source[start];
			while (isdigit(source[lexer->position])) {
				lexer->position++;
			}
			if (source[lexer->position] == '.') {
				token->type = TOKEN_FLOAT_LITERAL;
				lexer->position++;
				while (isdigit(source[lexer->position])) {
					lexer->position++;
				}
			}
			token->length = lexer->position - start;

			return token;
		} else if (c == '\'') {
			token->type = TOKEN_CHAR_LITERAL;
			size_t start = lexer->position;
			token->start = &source[start];

			char second = source[lexer->position + 1];
			if (second == '\'') {
				token->type = TOKEN_ERROR;
				lexer->position++;
			} else if (second == '\\') {
				lexer->position += 3;
				char fourth = source[lexer->position];
				if (fourth != '\'') {
					token->type = TOKEN_ERROR;
					while (fourth != ' ' && fourth != '\n' &&
					       fourth != '\'' && fourth != '\0') {
						lexer->position++;
						fourth = source[lexer->position];
					}
				}
			} else {
				lexer->position += 2;
				char third = source[lexer->position];
				if (third != '\'') {
					token->type = TOKEN_ERROR;
					while (third != ' ' && third != '\n' &&
					       third != '\'' && third != '\0') {
						lexer->position++;
						third = source[lexer->position];
					}
				}
			}
			if (source[lexer->position] != '\0') lexer->position++;
			token->length = lexer->position - start;

			return token;
		} else if (c == '"') {
			token->type = TOKEN_STRING_LITERAL;
			size_t start = lexer->position++;
			token->start = &source[start];
			while (source[lexer->position] != '\0' &&
			       source[lexer->position] != '"') {
				lexer->position++;
			}
			if (source[lexer->position] == '\0') {
				token->type = TOKEN_ERROR;
				token->length = lexer->position - start;
				return token;
			}
			lexer->position++;
			token->length = lexer->position - start;
			return token;
		} else {
			token->type = TOKEN_ERROR;
			char sec = source[lexer->position + 1];
			if (c == '=') {
				token->type = TOKEN_ASSIGN;
				if (sec == '=') {
					token->type = TOKEN_EQUALS;
					token->length = 2;
					token->start = &source[lexer->position];
					lexer->position += 2;
					return token;
				}
			} else if (c == '+') {
				token->type = TOKEN_PLUS;
				if (sec == '+') {
					token->type = TOKEN_INCREMENT;
					token->length = 2;
					token->start = &source[lexer->position];
					lexer->position += 2;
					return token;
				} else if (sec == '=') {
					token->type = TOKEN_ADD_ASSIGN;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}

			} else if (c == '-') {
				token->type = TOKEN_MINUS;
				char sec = source[lexer->position + 1];
				if (sec == '-') {
					token->type = TOKEN_DECREMENT;
					token->length = 2;
					token->start = &source[lexer->position];
					lexer->position += 2;
					return token;
				} else if (sec == '=') {
					token->type = TOKEN_SUB_ASSIGN;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}

			} else if (c == '*') {
				token->type = TOKEN_STAR;
				if (sec == '=') {
					token->type = TOKEN_MUL_ASSIGN;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}
			} else if (c == '/') {
				token->type = TOKEN_SLASH;
				if (sec == '=') {
					token->type = TOKEN_SLASH_ASSIGN;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				} else if (sec == '/') {
					token->type = TOKEN_COMMENT;
					token->start = &source[lexer->position];
					size_t start = lexer->position;
					while (c != '\n' && c != '\0') {
						c = source[lexer->position++];
					}
					if (c == '\n') lexer->position++;

					token->length = lexer->position - start;
					return token;
				} else if (sec == '*') {
					token->type = TOKEN_COMMENT;
					token->start = &source[lexer->position];
					size_t start = lexer->position;
					lexer->position += 2;
					c = source[lexer->position];
					sec = source[lexer->position + 1];

					while (!(c == '*' && sec == '/')) {
						if (sec == '\0') break;
						lexer->position++;
						c = source[lexer->position];
						sec = source[lexer->position + 1];
					}
					if (sec == '/') {
						lexer->position += 2;
					} else
						lexer->position++;
					token->length = lexer->position - start;
					return token;
				}
			} else if (c == '%') {
				token->type = TOKEN_MOD;
				if (sec == '=') {
					token->type = TOKEN_MOD_ASSIGN;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}
			} else if (c == '{') {
				token->type = TOKEN_LBRACE;
			} else if (c == '}') {
				token->type = TOKEN_RBRACE;
			} else if (c == '(') {
				token->type = TOKEN_LPAREN;
			} else if (c == ')') {
				token->type = TOKEN_RPAREN;
			} else if (c == ';') {
				token->type = TOKEN_SEMICOLON;
			} else if (c == '<') {
				token->type = TOKEN_LESS_THAN;
				if (sec == '=') {
					token->type = TOKEN_LESS_EQUALS;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				} else if (sec == '<') {
					token->type = TOKEN_BIT_LSHIFT;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}
			} else if (c == '>') {
				token->type = TOKEN_MORE_THAN;
				if (sec == '=') {
					token->type = TOKEN_MORE_EQUALS;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				} else if (sec == '>') {
					token->type = TOKEN_BIT_RSHIFT;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}
			} else if (c == '&') {
				token->type = TOKEN_BIT_AND;
				if (sec == '&') {
					token->type = TOKEN_AND;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}
			} else if (c == '|') {
				token->type = TOKEN_BIT_OR;
				if (sec == '|') {
					token->type = TOKEN_OR;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}
			} else if (c == '!') {
				token->type = TOKEN_NOT;
				if (sec == '=') {
					token->type = TOKEN_NOT_EQUALS;
					token->length = 2;
					token->start = &source[lexer->position];

					lexer->position += 2;
					return token;
				}

			} else if (c == '~') {
				token->type = TOKEN_BIT_NOT;
			} else if (c == '^') {
				token->type = TOKEN_BIT_XOR;
			}

			token->length = 1;
			token->start = &source[lexer->position++];
			return token;
		}
	}

	token->type = TOKEN_EOF;
	token->length = 0;
	token->start = &source[lexer->position];
	return token;
}
