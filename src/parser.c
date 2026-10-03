#include "parser.h"

#include "lexer.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

Token* expect(Parser* parser, TokenType type) {
	Token* token = parser->token;
	if (token && token->type == type) {
		parser_advance(parser);
		return token;
	}
	return NULL;
}

ASTNode* parse_initializer(Parser* parser) {
	if (!parser->token) {
		return NULL;
	}
	ASTNode* curr = NULL;

	if (isLiteral(parser->token->type)) {
		curr = malloc(sizeof(ASTNode));
		curr->data.literal = malloc(sizeof(LiteralNode));
		curr->data.literal->literal = parser->token;
		curr->type = AST_LITERAL;

		parser_advance(parser);
	}

	if (parser->token && isBinaryOperator(parser->token->type)) {
		ASTNode* nnode = malloc(sizeof(ASTNode));

		if (curr)
			nnode->type = AST_BINARY_OPERATOR;
		else
			nnode->type = AST_UNARY_OPERATOR;

		BinaryOperatorNode* binary = malloc(sizeof(BinaryOperatorNode));
		binary->left = curr;

		binary->op = parser->token;

		parser_advance(parser);
		binary->right = parse_initializer(parser);

		nnode->data.binary = binary;
		curr = nnode;
	}

	return curr;
}

ASTNode* parse_declaration(Parser* parser) {
	Token *type = NULL, *id = NULL, *semicolon = NULL, *literal;

	if ((type = expect(parser, TOKEN_INT))) {
		if ((id = expect(parser, TOKEN_IDENTIFIER))) {
			ASTNode* ast = malloc(sizeof(ASTNode));
			ast->type = AST_DECLARATION;

			DeclarationNode* declaration = malloc(sizeof(DeclarationNode));
			declaration->identifier = id;
			declaration->type = type;
			declaration->initializer = NULL;

			if ((literal = expect(parser, TOKEN_ASSIGN))) {
				free(literal);
				declaration->initializer = parse_initializer(parser);
				ast->data.declaration = declaration;
			}
			if (parser->token && parser->token->type == TOKEN_SEMICOLON) {
				free(parser->token);
				parser_advance(parser);
			}

			ast->data.declaration = declaration;
			return ast;
		}
	}

	return NULL;
}

void parser_init(Parser* parser, Lexer* lexer) {
	parser->lexer = lexer;
	parser->next = lexer_next(lexer);
	parser_advance(parser);
}

void parser_advance(Parser* parser) {
	parser->token = parser->next;
	parser->next = lexer_next(parser->lexer);
}

void print_node(const ASTNode* root) {
	if (root->type == AST_LITERAL) {
		print_token(root->data.literal->literal);
	}
	if (root->type == AST_BINARY_OPERATOR) {
		print_token(root->data.binary->op);
		print_node(root->data.binary->left);
		print_node(root->data.binary->right);
	}
	if (root->type == AST_DECLARATION) {
		print_token(root->data.declaration->identifier);
		print_token(root->data.declaration->type);
		if (root->data.declaration->initializer)
			print_node(root->data.declaration->initializer);
	}
}

ASTNode* parse_module(Parser* parser) {
	ASTNode* astnode = malloc(sizeof(ASTNode));
	ModuleNode* module = malloc(sizeof(ModuleNode));
	int count = 0;
	int size = 1;
	module->nodes = malloc(size * sizeof(ASTNode*));

	while (parser->token) {
		ASTNode* var = parse_declaration(parser);
		if (!var) return NULL;
		const Token* id = var->data.declaration->identifier;
		const Token* type = var->data.declaration->type;
		const ASTNode* initializer = var->data.declaration->initializer;
		print_node(var);

		if (count == size) {
			size *= 2;
			module->nodes = realloc(module->nodes, size * sizeof(ASTNode*));
		}
		module->nodes[count++] = var;
	}
	module->count = count;
	module->size = size;
	astnode->data.module = module;
	astnode->type = AST_MODULE;

	return astnode;
}
