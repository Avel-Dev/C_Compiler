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
	if(root->type == AST_FUNCTION){
		print_token(root->data.function->id);
		print_token(root->data.function->type);
		printf("Parameters: ");
		for(int i = 0; i <  root->data.function->parameter_count; i++) {
			print_node(root->data.function->parameters[i]);
		}
		printf("Body: ");
		for(int i = 0; i <  root->data.function->body_count; i++) {
			print_node(root->data.function->body[i]);
		}
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

Token* parser_expect(Parser* parser, TokenType type) {
	const Token* token = parser_peek(parser);
	if (token && token->type == type) {
		return parser_advance(parser);
	}
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

ASTNode* parse_primary(Parser* parser) {
	Token* token = parser_peek(parser);

	if (!token) {
		return NULL;
	}

	ASTNode* node = malloc(sizeof(ASTNode));

	if (token->type == TOKEN_IDENTIFIER) {
		node->type = AST_IDENTIFIER;
		node->data.identifier = malloc(sizeof(IdentifierNode));
		node->data.identifier->identifier = token;

		parser_advance(parser);
		return node;
	}

	if (isLiteral(token->type)) {
		node->type = AST_LITERAL;
		node->data.literal = malloc(sizeof(LiteralNode));
		node->data.literal->literal = token;

		parser_advance(parser);
		return node;
	}
	if (isUnaryOperator(token->type)) {
		node->type = AST_UNARY_OPERATOR;
		node->data.unary = malloc(sizeof(UnaryOperatorNode));
		node->data.unary->operator = token;
		parser_advance(parser);
		int precedence = get_precedence(token->type);
		node->data.unary->operand = parse_expression(parser, precedence);

		token = parser_peek(parser);

		if (!node->data.unary->operand) {
			free(node->data.unary);
			free(node);
			return NULL;
		}

		parser_advance(parser);
		return node;
	}
	if (token->type == TOKEN_LPAREN) {
		free(node);

		parser_advance(parser);

		node = parse_expression(parser, 0);

		token = parser_peek(parser);

		if (!token || token->type != TOKEN_RPAREN) {
			// Throw parser error: expected ')'
			return NULL;
		}

		parser_advance(parser);
		return node;
	}

	free(node);

	// Throw parser error: expected expression
	return NULL;
}

ASTNode* parse_expression(Parser* parser, int min_precedence) {
	ASTNode* left = parse_primary(parser);

	if (!left) {
		return NULL;
	}

	while (1) {
		Token* token = parser_peek(parser);
		if (!token) break;

		if (token->type == TOKEN_SEMICOLON || token->type == TOKEN_RPAREN) {
			break;
		}

		if (!isBinaryOperator(token->type)) {
			break;
		}

		int precedence = get_precedence(token->type);

		if (precedence < min_precedence) {
			break;
		}

		Token* operator_token = token;
		parser_advance(parser);

		ASTNode* right = parse_expression(parser, precedence + 1);

		if (!right) {
			// Throw parser error: expected expression after operator
			return NULL;
		}

		BinaryOperatorNode* binary = malloc(sizeof(BinaryOperatorNode));

		binary->operator = operator_token;
		binary->left = left;
		binary->right = right;

		ASTNode* parent = malloc(sizeof(ASTNode));

		parent->type = AST_BINARY_OPERATOR;
		parent->data.binary = binary;

		left = parent;
	}

	return left;
}

ASTNode** parse_function_parameters(Parser* parser, int*  count) {
	ASTNode** parameters = NULL;
	int capacity = 0;
	int length = 0;
	*count = 0;

	Token* token = NULL;
	while(1) {
		token = parser_peek(parser);
		if(!token) {
			// error
			return NULL;
		}
		if(token->type == TOKEN_RPAREN) {
			*count = length;
			return parameters;
		}		
		if(!isDataType(token->type)) {
			//datatype required error
			return NULL;	
		}
		Token* type = parser_advance(parser);
		Token* id = token;

		if(!(id = parser_expect(parser, TOKEN_IDENTIFIER))){
			//identifier required error
			return NULL;
		}

		ASTNode* parameter = malloc(sizeof(ASTNode));
		parameter->type = AST_DECLARATION;
		parameter->data.declaration = malloc(sizeof(DeclarationNode));

		if(!parameter->data.declaration) {
			free(parameter);
			return NULL;
		} 
		parameter->data.declaration->identifier = id;
		parameter->data.declaration->type = type;

		token = parser_peek(parser);
		if(token->type == TOKEN_ASSIGN) {
			parser_delete_front(parser);	
			parameter->data.declaration->initializer = parse_expression(parser, 0);
			token = parser_peek(parser);
			if(token->type == TOKEN_SEMICOLON) {
				// can't end on semicolon in argument list
				return NULL;
			}
		} 
		if(length == capacity) {
			if(capacity == 0) capacity = 1;
			else capacity = capacity*2;

			parameters = realloc(parameters, capacity*sizeof(ASTNode*));
		}

		parameters[length++] = parameter;

		if(token->type == TOKEN_COMMA) {	
			parser_delete_front(parser);	
		}	
	}

}

ASTNode** parse_function_body(Parser* parser, int*  count) {
	ASTNode** statements = NULL;
	*count = 0;
	Token* token = parser_peek(parser);
	if(token->type == TOKEN_RBRACE){
		return statements;
	}
}

FunctionNode* parse_function(Parser* parser) {

	FunctionNode* node = malloc(sizeof(FunctionNode));
	int count= 0;
	ASTNode** parameters = parse_function_parameters(parser, &count); 
	node->parameters = parameters;
	node->parameter_count = count;
	Token* token = parser_peek(parser);
	if(token->type == TOKEN_RPAREN){
		parser_delete_front(parser);
		token = parser_peek(parser);
		if(token->type == TOKEN_LBRACE) {
			parser_delete_front(parser);
		}
	}
	ASTNode** statements = parse_function_body(parser, &count);
	node->body = statements;
	node->body_count = count;
	
	return node;
}

ASTNode* parse_declaration(Parser* parser) {
	Token* type = parser_peek(parser);
	Token* id = NULL;

	if (!isDataType(type->type)) {
		// invalid start parser error
		return NULL;
	}

	type = parser_advance(parser);
	if (!(id = parser_expect(parser, TOKEN_IDENTIFIER))) {
		// identifier expected error

		return NULL;
	}

	Token* token = parser_peek(parser);
	if (token->type == TOKEN_ASSIGN) {
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_DECLARATION;
		node->data.declaration = malloc(sizeof(DeclarationNode));
		node->data.declaration->type = type;
		node->data.declaration->identifier = id;

		parser_delete_front(parser);

		ASTNode* initializer = parse_expression(parser, 0);
		if (!initializer) {
			free(node->data.declaration);
			free(node);
			// initializer required error
			return NULL;
		}
		node->data.declaration->initializer = initializer;
		return node;
	} else if (token->type == TOKEN_LPAREN) {
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_FUNCTION;

		parser_delete_front(parser);

		node->data.function = parse_function(parser);
		node->data.function->id = id;
		node->data.function->type = type;
		return node;
	} else if (token->type == TOKEN_SEMICOLON) {
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_DECLARATION;
		node->data.declaration = malloc(sizeof(DeclarationNode));
		node->data.declaration->type = type;
		node->data.declaration->identifier = id;
		return node;
	}

	return NULL;
}

void parser_init(Parser* parser, Lexer* lexer) {
	parser->lexer = lexer;
	parser->next = lexer_next(lexer);

	parser_advance(parser);
}

void parser_delete_front(Parser* parser) {
	free(parser->token);
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
	Token* token = NULL;
	while (1) {
		token = parser_peek(parser);
		if (!token) break;

		if (token->type == TOKEN_SEMICOLON || token->type == TOKEN_RPAREN || token->type == TOKEN_RBRACE) {
			parser_delete_front(parser);
			continue;
		}

		ASTNode* var = parse_declaration(parser);

		if (var) print_node(var);
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
