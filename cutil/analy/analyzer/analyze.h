#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#define MAX_TOKEN_LEN 64
#define MAX_TOKENS    100

enum type_tag
{
    IDENTIFIER,
    QUALIFIER,
    TYPE
};

struct token
{
    char type;
    char string[MAX_TOKEN_LEN];
};

enum type_tag classify_string();
void gettoken();
void read_to_first_identifer();
void deal_with_arrays();
void deal_with_function_args();
void deal_with_pointers();
void deal_with_declarator();

#endif
