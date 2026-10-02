#include "lexer.h"

#include <ctype.h>
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

Token* createKeyword(size_t* position, const char* source) {
	size_t start = *position;
	Token* token = malloc(sizeof(Token));
	token->start = &source[*position];
	unsigned char c;
	while ((c = source[*position]) != '\0' && (isalnum(c) || c == '_')) {
		(*position)++;
	}
	token->length = (*position) - start;
	token->type = keyword_type(token->start, token->length);
	return token;
}

Token* createNumericLiteral(size_t* position, const char* source) {
	Token* token = malloc(sizeof(Token));
	token->type = TOKEN_INT_LITERAL;
	size_t start = *position;
	token->start = &source[start];
	while (isdigit(source[*position])) {
		(*position)++;
	}
	if (source[*position] == '.') {
		token->type = TOKEN_FLOAT_LITERAL;
		(*position)++;
		while (isdigit(source[*position])) {
			(*position)++;
		}
	}
	token->length = (*position) - start;

	return token;
}

Token* createCHARLiteral(size_t* position, const char* source) {
	Token* token = malloc(sizeof(Token));
	token->type = TOKEN_CHAR_LITERAL;

	size_t pos = *position;
	size_t start = pos;
	token->start = &source[start];

	char second = source[pos + 1];
	if (second == '\'') {
		token->type = TOKEN_ERROR;
		pos++;
	} else if (second == '\\') {
		pos += 3;
		char fourth = source[pos];
		if (fourth != '\'') {
			token->type = TOKEN_ERROR;
			while (fourth != ' ' && fourth != '\n' && fourth != '\'' &&
			       fourth != '\0') {
				pos++;
				fourth = source[pos];
			}
		}
	} else {
		pos += 2;
		char third = source[pos];
		if (third != '\'') {
			token->type = TOKEN_ERROR;
			while (third != ' ' && third != '\n' && third != '\'' &&
			       third != '\0') {
				pos++;
				third = source[pos];
			}
		}
	}
	if (source[pos] != '\0') pos++;
	token->length = pos - start;
	*position = pos;
	return token;
}

Token* createStringLiteral(size_t* position, const char* source) {
	Token* token = malloc(sizeof(Token));
	token->type = TOKEN_STRING_LITERAL;
	size_t pos = *position;
	size_t start = pos++;
	token->start = &source[start];
	while (source[pos] != '\n' && source[pos] != '"') {
		if (source[pos] == '\\') pos++;
		pos++;
	}
	if (source[pos] == '\n') {
		token->type = TOKEN_ERROR;
	}

	if (source[pos] != '\0') pos++;
	token->length = pos - start;
	*position = pos;
	return token;
}

Token* createComment(size_t* position, const char* source) {
	size_t pos = *position;
	size_t start = pos;
	Token* token = malloc(sizeof(Token));
	token->type = TOKEN_COMMENT;
	token->start = &source[pos];
	char c = source[pos];
	char sec = source[pos + 1];
	if (sec == '*') {
		while (!(c == '*' && sec == '/')) {
			if (sec == '\0') break;
			pos++;
			c = source[pos];
			sec = source[pos + 1];
		}
		if (sec == '\0') {
			token->type = TOKEN_ERROR;
			pos++;
		} else if (sec == '/') {
			pos += 2;
		}
	} else if (sec == '/') {
		while (c != '\n' && c != '\0') {
			pos++;
			c = source[pos];
		}
		if (c == '\n') pos++;
	}

	token->length = pos - start;
	*position = pos;
	return token;
}

Token* createOperatorOrPunctuation(size_t* position, const char* source) {
	size_t pos = *position;
	char c = source[pos];
	char sec = source[pos + 1];
	int token_length = 1;
	TokenType tokentype = TOKEN_ERROR;
	if (c == '=' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_EQUALS;
	} else if (c == '!' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_NOT_EQUALS;
	} else if (c == '>' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_MORE_EQUALS;
	} else if (c == '<' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_LESS_EQUALS;
	} else if (c == '+' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_ADD_ASSIGN;
	} else if (c == '-' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_SUB_ASSIGN;
	} else if (c == '/' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_SLASH_ASSIGN;
	} else if (c == '%' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_MOD_ASSIGN;
	} else if (c == '*' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_MUL_ASSIGN;
	} else if (c == '&' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_AND_ASSIGN;
	} else if (c == '|' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_OR_ASSIGN;
	} else if (c == '^' && sec == '=') {
		token_length = 2;
		tokentype = TOKEN_XOR_ASSIGN;
	} else if (c == '>' && sec == '>') {
		char third = source[pos + 2];
		if (third == '=') {
			token_length = 3;
			tokentype = TOKEN_RSHIFT_ASSIGN;
		} else {
			token_length = 2;
			tokentype = TOKEN_BIT_RSHIFT;
		}
	} else if (c == '<' && sec == '<') {
		char third = source[pos + 2];
		if (third == '=') {
			token_length = 3;
			tokentype = TOKEN_LSHIFT_ASSIGN;
		} else {
			token_length = 2;
			tokentype = TOKEN_BIT_LSHIFT;
		}
	} else if (c == '&' && sec == '&') {
		token_length = 2;
		tokentype = TOKEN_AND;
	} else if (c == '|' && sec == '|') {
		token_length = 2;
		tokentype = TOKEN_OR;
	} else if (c == '+' && sec == '+') {
		token_length = 2;
		tokentype = TOKEN_INCREMENT;
	} else if (c == '-' && sec == '-') {
		token_length = 2;
		tokentype = TOKEN_DECREMENT;
	} else if ((c == '/' && sec == '/') || (c == '/' && sec == '*')) {
		token_length = 2;
		tokentype = TOKEN_COMMENT;
		return createComment(position, source);
	} else if (c == '!') {
		tokentype = TOKEN_NOT;
	} else if (c == '>') {
		tokentype = TOKEN_MORE_THAN;
	} else if (c == '<') {
		tokentype = TOKEN_LESS_THAN;
	} else if (c == '+') {
		tokentype = TOKEN_ADD;
	} else if (c == '-') {
		tokentype = TOKEN_SUB;
	} else if (c == '*') {
		tokentype = TOKEN_STAR;
	} else if (c == '/') {
		tokentype = TOKEN_SLASH;
	} else if (c == '%') {
		tokentype = TOKEN_MOD;
	} else if (c == '&') {
		tokentype = TOKEN_BIT_AND;
	} else if (c == '|') {
		tokentype = TOKEN_BIT_OR;
	} else if (c == '~') {
		tokentype = TOKEN_BIT_NOT;
	} else if (c == '^') {
		tokentype = TOKEN_BIT_XOR;
	} else if (c == '{') {
		tokentype = TOKEN_LBRACE;
	} else if (c == '}') {
		tokentype = TOKEN_RBRACE;
	} else if (c == '(') {
		tokentype = TOKEN_LPAREN;
	} else if (c == ')') {
		tokentype = TOKEN_RPAREN;
	} else if (c == ';') {
		tokentype = TOKEN_SEMICOLON;
	} else if (c == '=') {
		tokentype = TOKEN_ASSIGN;
	} else if (c == '[') {
		tokentype = TOKEN_LBRACKET;
	} else if (c == ']') {
		tokentype = TOKEN_RBRACKET;
	} else if (c == ',') {
		tokentype = TOKEN_COMMA;
	} else if (c == '.') {
		tokentype = TOKEN_DOT;
	} else if (c == ':') {
		tokentype = TOKEN_COLON;
	} else if (c == '?') {
		tokentype = TOKEN_QUESTION;
	} else if (c == ';') {
		tokentype = TOKEN_SEMICOLON;
	}

	Token* token = malloc(sizeof(Token));
	token->start = &source[pos];
	token->type = tokentype;
	token->length = 1;

	token->length = token_length;
	pos += token_length;
	*position = pos;
	return token;
}

Token* lexer_next(Lexer* lexer) {
	if (lexer->source[lexer->position] == '\0') return NULL;
	const char* source = lexer->source;
	size_t* position = &lexer->position;
	unsigned char c;
	while ((c = source[lexer->position]) != '\0') {
		if (isspace(c)) {
			lexer->position++;
			continue;
		}
		if (isalpha(c) || c == '_') {
			return createKeyword(position, source);
		} else if (isdigit(c)) {
			return createNumericLiteral(position, source);
		} else if (c == '\'') {
			return createCHARLiteral(position, source);
		} else if (c == '"') {
			return createStringLiteral(position, source);
		} else {
			return createOperatorOrPunctuation(position, source);
		}
	}

	return NULL;
}
