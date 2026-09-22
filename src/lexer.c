#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	TOKEN_EOF,

	TOKEN_IDENTIFIER,

	TOKEN_INT_LITERAL,
	TOKEN_FLOAT_LITERAL,
	TOKEN_STRING_LITERAL,

	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,

	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACE,
	TOKEN_RBRACE,

	TOKEN_SEMICOLON
} TokenType;

typedef struct {
	TokenType type;
	const char* start;
	size_t length;
} Token;

typedef enum { IDENTIFIER } TypeID;

typedef enum { INT, FLOAT, STRING, BOOL } TypeLiteral;

typedef struct {
	TypeID ID;
	TypeLiteral literal;
	char* name;
	int size;
} TokenID;

void finishID(TokenID*** IDs, int* end, int* capacity, char** curr_id_name, int* size,
	    int curr_id_max_len) {
	TokenID* new_ID = malloc(sizeof(TokenID));
	new_ID->name = *curr_id_name;
	new_ID->size = *size;
	new_ID->ID = IDENTIFIER;

	if ((*end + 1) == *capacity) {
		*capacity *= 2;
		*IDs = realloc(*IDs, *capacity * sizeof(TokenID*));
	}
	(*IDs)[++(*end)] = new_ID;

	*curr_id_name = malloc(curr_id_max_len * sizeof(char));
	*size = -1;
}

TokenID** lexer(int* count, FILE* file) {
	int capacity = *count;
	if (file == NULL) return NULL;

	TokenID** IDs = malloc(capacity * sizeof(TokenID*));
	int c;
	int curr_id_max_len = 10;
	char* curr_id_name = malloc(curr_id_max_len * sizeof(char));
	int curr_id_end = -1;
	int id_end = -1;

	while ((c = fgetc(file)) != EOF) {
		if (c == ' ') {
			if (curr_id_end >= 0)
				finishID(&IDs, &id_end, &capacity, &curr_id_name,
				         &curr_id_end, curr_id_max_len);
		} else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
			if (curr_id_end >= 0)
				finishID(&IDs, &id_end, &capacity, &curr_id_name,
				         &curr_id_end, curr_id_max_len);
			// emit operator
		} else if (c == '{' || c == '}' || c == '(' || c == ')') {
			if (curr_id_end >= 0)
				finishID(&IDs, &id_end, &capacity, &curr_id_name,
				         &curr_id_end, curr_id_max_len);
			// emit bracket
		} else if (isalpha(c) || c == '_') {
			if (curr_id_end + 1 == curr_id_max_len) {
				curr_id_max_len *= 2;
				curr_id_name =
				  realloc(curr_id_name, curr_id_max_len * sizeof(char));
			}
			curr_id_name[++curr_id_end] = c;
		} else if (isdigit(c)) {
			if (curr_id_end >= 0) {
				finishID(&IDs, &id_end, &capacity, &curr_id_name,
				         &curr_id_end, curr_id_max_len);
				while (isalnum(c)) {
					c = fgetc(file);
				}
				// create digit primitive
				continue;
			}
			if (curr_id_end + 1 == curr_id_max_len) {
				curr_id_max_len *= 2;
				curr_id_name =
				  realloc(curr_id_name, curr_id_max_len * sizeof(char));
			}
			curr_id_name[++curr_id_end] = c;

			// if yes create number
			// if no add to current
		}
	}

	fclose(file);
	*count = id_end;
	return IDs;
}
