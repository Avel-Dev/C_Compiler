#pragma once

#include "lexer.h"

typedef enum {
	AST_SCOPE,
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
	ASTNode** body;
	int body_count;
} ScopeBody;

typedef struct {	
	const Token* id;
	const Token* type;
	ASTNode** parameters;
	int parameter_count;
	ScopeBody scope;
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
	const ASTNode* initializer;
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
	ScopeBody* scope;
	ErrorNode* error;
};

struct ASTNode {
	ASTNodeType type;
	union Data data;
};

typedef struct {
	Lexer* lexer;
	Token** buffer;
	int next;
	int buffer_size; // look ahead buffer
} Parser;

void print_token(const Token* token);

// these can use parser
void parser_init(Parser* parser, Lexer* lexer);
Token* parser_expect(Parser* parser, TokenType type);
Token* parser_peek(Parser* parser, int next);
void parser_delete_front(Parser* parser);
Token* parser_advance(Parser* parser);
Token** parser_advance_multiple(Parser* parser, int advance);

// these have to use the methods
FunctionNode* parse_function(Parser* parser);
ASTNode* parse_declaration(Parser* parser);
ASTNode* parse_module(Parser* parser);
ASTNode* parse_initializer(Parser* parser);
ASTNode* parse_expression(Parser* parser, int min_precedence);
ASTNode* parse_primary(Parser* parser);
