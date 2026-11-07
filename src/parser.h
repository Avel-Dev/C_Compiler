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
	const Token* id;
	const Token* type;
	ASTNode** parameters;
	int parameter_count;
	ASTNode** body;
	int body_count;
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

void print_token(const Token* token);

// these can use parser
void parser_init(Parser* parser, Lexer* lexer);
Token* parser_expect(Parser* parser, TokenType type);
Token* parser_peek(Parser* parser);
void parser_delete_front(Parser* parser);
Token* parser_advance(Parser* parser);

// these have to use the methods
FunctionNode* parse_function(Parser* parser);
ASTNode* parse_declaration(Parser* parser);
ASTNode* parse_module(Parser* parser);
ASTNode* parse_initializer(Parser* parser);
ASTNode* parse_expression(Parser* parser, int min_precedence);
ASTNode* parse_primary(Parser* parser);
