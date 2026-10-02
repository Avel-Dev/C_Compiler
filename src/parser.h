#pragma once

#include "lexer.h"

typedef struct {
	Lexer* lexer;
	Token* token;
	Token* next;
} Parser;

typedef enum { AST_MODULE, AST_FUNCTION, AST_OPERATOR, AST_LITERAL, AST_IDENTIFIER } ASTNodeType;

typedef struct ASTNode ASTNode;

typedef struct {
	ASTNode** nodes;
	int count;
} ModuleNode;

typedef struct {
	ASTNode** nodes;
	int count;
} FunctionNode;

typedef struct {
	const Token* operator;
	ASTNode* left;
	ASTNode* right;
} BinaryOperatorNode;

typedef struct {
	const Token* literal;
} LiteralNode;

typedef struct {
	const Token* identifier;
} IdentifierNode;

union Data {
	ModuleNode* module;
	FunctionNode* function;
	BinaryOperatorNode* binary;
	LiteralNode* literal;
	IdentifierNode* identifier;
};

struct ASTNode {
	ASTNodeType type;
	union Data data;
};

void parser_init(Parser* parser, Lexer* lexer);
void parser_advance(Parser* parser);
