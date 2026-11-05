#pragma once

#include "lexer.h"

typedef enum {
	AST_MODULE,
	AST_FUNCTION,
	AST_UNARY_OPERATOR,
	AST_LITERAL,
	AST_IDENTIFIER,
	AST_DECLARATION,
	AST_INITIALIZER,
	AST_BINARY_OPERATOR,
	AST_EXPRESSION,
	AST_ERROR
} ASTNodeType;

char* AST_Node_name(ASTNodeType type);
typedef struct ASTNode ASTNode;

typedef struct {
	ASTNode** nodes;
	int count;
	int size;
} ModuleNode;

typedef struct {
	ASTNode** nodes;
	int count;
	// datatype
} FunctionNode;

typedef struct {
	const Token* operator;
	ASTNode* left;
	ASTNode* right;
} BinaryOperatorNode;

typedef struct {
	const Token* operator;
	ASTNode* operand;
} UnaryOperatorNode;

typedef struct {
	const Token* literal;
} LiteralNode;

typedef struct {
	const Token* identifier;
} IdentifierNode;

typedef struct {
	const Token* identifier;
	const Token* type;
	const ASTNode* initializer;
} DeclarationNode;

typedef struct {
	char* error_string;
} ErrorNode;

union Data {
	ModuleNode* module;
	FunctionNode* function;
	BinaryOperatorNode* binary;
	LiteralNode* literal;
	IdentifierNode* identifier;
	DeclarationNode* declaration;
	UnaryOperatorNode* unary;
	ErrorNode* error;
};

struct ASTNode {
	ASTNodeType type;
	union Data data;
};

typedef struct {
	Token* token;
	Lexer* lexer;
	Token* next;
} Parser;

ASTNode* parse_declaration(Parser* parser);
ASTNode* parse_module(Parser* parser);
ASTNode* parse_initializer(Parser* parser);

void print_token(const Token* token);
void parser_init(Parser* parser, Lexer* lexer);

Token* expect(Parser* parser, TokenType type);
Token* parser_peek(Parser* parser);
Token* parser_advance(Parser* parser);
