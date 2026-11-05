#include "parser.h"

#include "lexer.h"

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_node(const ASTNode* root) {
	printf("AST Node:  %s\n", AST_Node_name(root->type));
	if (root->type == AST_LITERAL) {
		print_token(root->data.literal->literal);
	}
	if (root->type == AST_BINARY_OPERATOR) {
		print_token(root->data.binary->operator);
		printf("Left ");
		print_node(root->data.binary->left);

		printf("Reft ");
		print_node(root->data.binary->right);
	}
	if (root->type == AST_UNARY_OPERATOR) {
		print_token(root->data.unary->operator);
		print_node(root->data.unary->operand);
	}
	if (root->type == AST_DECLARATION) {
		print_token(root->data.declaration->identifier);
		print_token(root->data.declaration->type);
		if (root->data.declaration->initializer)
			print_node(root->data.declaration->initializer);
	}
	if (root->type == AST_IDENTIFIER) {
		print_token(root->data.identifier->identifier);
	}
}

int get_precedence(TokenType type) {
	switch (type) {
	case TOKEN_STAR:
	case TOKEN_SLASH:
	case TOKEN_MOD:
		return 6;

	case TOKEN_ADD:
	case TOKEN_SUB:
		return 5;

	case TOKEN_LESS_THAN:
	case TOKEN_MORE_THAN:
	case TOKEN_LESS_EQUALS:
	case TOKEN_MORE_EQUALS:
		return 4;

	case TOKEN_EQUALS:
	case TOKEN_NOT_EQUALS:
		return 3;

	case TOKEN_AND:
		return 2;

	case TOKEN_OR:
		return 1;

	default:
		return -1;
	}
}

char* AST_Node_name(ASTNodeType type) {
	switch (type) {
	case AST_MODULE:
		return "AST_MODULE";
	case AST_FUNCTION:
		return "AST_FUNCTION";
	case AST_UNARY_OPERATOR:
		return "AST_UNARY_OPERATOR";
	case AST_LITERAL:
		return "AST_LITERAL";
	case AST_IDENTIFIER:
		return "AST_IDENTIFIER";
	case AST_DECLARATION:
		return "AST_DECLARATION";
	case AST_INITIALIZER:
		return "AST_INITIALIZER";
	case AST_BINARY_OPERATOR:
		return "AST_BINARY_OPERATOR";
	case AST_EXPRESSION:
		return "AST_EXPRESSION";
	case AST_ERROR:
		return "AST_ERROR";
	}

	return "NOT VALID TYPE";
}

Token* expect(Parser* parser, TokenType type) {
	const Token* token = parser_peek(parser);
	if (token && token->type == type) {
		return parser_advance(parser);
	}
	return NULL;
}

ASTNode* parse_expression(Parser* parser, int min_precedence) {
	Token* token = parser_peek(parser);
	ASTNode* left = NULL;

	while (token && token->type != TOKEN_SEMICOLON) {
		print_token(token);

		if (token->type == TOKEN_IDENTIFIER) {
			left = malloc(sizeof(ASTNode));
			left->type = AST_IDENTIFIER;
			left->data.identifier = malloc(sizeof(IdentifierNode));
			left->data.identifier->identifier = token;
			parser_advance(parser);
		} else if (isLiteral(token->type)) {
			left = malloc(sizeof(ASTNode));
			left->type = AST_LITERAL;
			left->data.literal = malloc(sizeof(LiteralNode));
			left->data.literal->literal = token;
			parser_advance(parser);
		} else if (isBinaryOperator(token->type)) {
			if (!left) {
				// throw parser error
			}
			int precedence = get_precedence(token->type);

			if (precedence < min_precedence) {
				return left;
			}
			BinaryOperatorNode* binary_operator =
			  malloc(sizeof(BinaryOperatorNode));
			binary_operator->operator = token;
			binary_operator->left = left;
			parser_advance(parser);
			binary_operator->right = parse_expression(parser, precedence + 1);
			left = malloc(sizeof(ASTNode));
			left->type = AST_BINARY_OPERATOR;
			left->data.binary = binary_operator;
		} else if (token->type == TOKEN_LPAREN) {
			if (left) {
				// throw parser error
			}
			free(token);
			parser_advance(parser);
		} else if (token->type == TOKEN_RPAREN) {
			free(token);
			parser_advance(parser);
			return left;
		}
		token = parser_peek(parser);
	}
	if (token && token->type == TOKEN_SEMICOLON) {
		free(token);
		parser_advance(parser);
	}
	return left;
}

ASTNode* parse_quality(Parser* parser) {
	return NULL;
}

ASTNode* parse_term(Parser* parser) {
	return NULL;
}

ASTNode* parse_factor(Parser* parser) {
	return NULL;
}

ASTNode* parse_unary(Parser* parser) {
	return NULL;
}

char* string_concate(const char* null_terminated, const char* non_null_terminated, int len) {
	int literal_len = strlen(null_terminated);
	char* result = malloc(literal_len + len);
	memcpy(result, null_terminated, literal_len);
	memcpy(result + literal_len, non_null_terminated, len);
	result[literal_len + len] = '\0';
	return result;
}

/*
ASTNode* parse_primary(Parser* parser) {
	if (parser->token == NULL) return NULL;

	ASTNode* curr = malloc(sizeof(ASTNode));
	const Token* token = parser_peek(parser);
	if (isLiteral(token->type)) {
		curr->data.literal = malloc(sizeof(LiteralNode));
		curr->data.literal->literal = parser_advance(parser);
		curr->type = AST_LITERAL;

	} else if (token->type == TOKEN_IDENTIFIER) {
		curr->data.identifier = malloc(sizeof(IdentifierNode));
		curr->data.identifier->identifier = parser_advance(parser);
		curr->type = AST_IDENTIFIER;
	} else if (token->type == TOKEN_LPAREN) {
		free(curr);
		free(parser_advance(parser));
		curr = parse_expression(parser);
		curr->type = AST_EXPRESSION;
	} else {
		curr->type = AST_ERROR;
		curr->data.error = malloc(sizeof(ErrorNode));
		curr->data.error->error_string = string_concate(
		  "expected primary not found: instead found", token->start, token->length);
	}

	return curr;
}*/

ASTNode* parse_initializer(Parser* parser) {
	if (!parser->token) {
		return NULL;
	}

	//	return parse_primary(parser);
	return parse_expression(parser, INT_MIN);
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

			const Token* token = parser_peek(parser);
			if (token->type == TOKEN_ASSIGN) {
				free(parser_advance(parser));
				declaration->initializer = parse_initializer(parser);
			} else if (token->type == TOKEN_SEMICOLON) {
				free(parser_advance(parser));
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

Token* parser_advance(Parser* parser) {
	Token* temp = parser->token;
	parser->token = parser->next;
	parser->next = lexer_next(parser->lexer);
	return temp;
}

Token* parser_peek(Parser* parser) {
	return parser->token;
}

ASTNode* parse_module(Parser* parser) {
	ASTNode* astnode = malloc(sizeof(ASTNode));
	ModuleNode* module = malloc(sizeof(ModuleNode));
	int count = 0;
	int size = 1;
	module->nodes = malloc(size * sizeof(ASTNode*));
	int h = 0;
	while (h < 5 && parser->token) {
		h++;
		ASTNode* var = parse_expression(parser, INT_MIN);
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
