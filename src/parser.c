#include "parser.h"

void parser_init(Parser* parser, Lexer* lexer) {
	parser->lexer = lexer;
	parser->token = lexer_next(parser->lexer);
	parser->next = lexer_next(parser->lexer);
}

void parser_advance(Parser* parser) {
	parser->token = parser->next;
	parser->next = lexer_next(parser->lexer);
}
