
#pragma once

#include <stddef.h>

typedef enum {
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
	TOKEN_ADD,
	TOKEN_SUB,
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
	// Arithmetic Assignment
	TOKEN_ADD_ASSIGN,
	TOKEN_SUB_ASSIGN,
	TOKEN_SLASH_ASSIGN,
	TOKEN_MOD_ASSIGN,
	TOKEN_MUL_ASSIGN,
	// Bitwise Assignment
	TOKEN_AND_ASSIGN,
	TOKEN_OR_ASSIGN,
	TOKEN_XOR_ASSIGN,
	TOKEN_LSHIFT_ASSIGN,
	TOKEN_RSHIFT_ASSIGN,

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
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,

	TOKEN_COMMA,
	TOKEN_DOT,
	TOKEN_COLON,
	TOKEN_QUESTION,

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

const char* token_type_name(TokenType type);

void lexer_init(Lexer* lexer, const char* source);

TokenType keyword_type(const char* start, size_t length);

Token* createKeyword(size_t* position, const char* source);
Token* createNumericLiteral(size_t* position, const char* source);
Token* createCHARLiteral(size_t* position, const char* source);
Token* createStringLiteral(size_t* position, const char* source);
Token* createComment(size_t* position, const char* source);
Token* createOperatorOrPunctuation(size_t* position, const char* source);

Token* lexer_next(Lexer* lexer);

int isLiteral(TokenType type);

int isBinaryOperator(TokenType type);

void print_token(const Token* token);
