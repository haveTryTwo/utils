#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "analyze.h"

static int top = -1;
static struct token stack[MAX_TOKENS];
static struct token this;

#define pop stack[top--]
#define push(s) stack[++top] = s

enum type_tag classify_string()
{
    char *s = this.string;
    if (strcmp(s, "const") == 0)
    {
        strcpy(s, "read-only");
        return QUALIFIER;
    }

    if (strcmp(s, "volatile") == 0) return QUALIFIER;
    
    if (strcmp(s, "void") == 0) return TYPE;
    if (strcmp(s, "char") == 0) return TYPE;
    if (strcmp(s, "signed") == 0) return TYPE;
    if (strcmp(s, "unsigned") == 0) return TYPE;
    if (strcmp(s, "short") == 0) return TYPE;
    if (strcmp(s, "int") == 0) return TYPE;
    if (strcmp(s, "long") == 0) return TYPE;
    if (strcmp(s, "float") == 0) return TYPE;
    if (strcmp(s, "double") == 0) return TYPE;
    if (strcmp(s, "struct") == 0) return TYPE;
    if (strcmp(s, "union") == 0) return TYPE;
    if (strcmp(s, "enum") == 0) return TYPE;

    return IDENTIFIER;
}

void gettoken()
{
    char *p = this.string;

    while ((*p = getchar()) == ' ');

    if (isalnum(*p))
    {
        while (isalnum(*++p = getchar()));
        ungetc(*p, stdin);
        *p = '\0';
        this.type = classify_string();
        return;
    }

    if (*p == '*')
    {
        strcpy(this.string, " pointer to ");
        this.type = '*';
        return;
    }

    this.string[1] = '\0';
    this.type = *p;
    return;
}

void read_to_first_identifer()
{
    gettoken();
    while (this.type != IDENTIFIER)
    {
        push(this);
        gettoken();
    }
    printf("%s is ", this.string);
    gettoken();
}

void deal_with_arrays()
{
    while (this.type == '[')
    {
        printf(" array ");
        gettoken();
        if(isdigit(this.string[0]))   // check whether this is number according the frist char
        {
            printf("0..%d", atoi(this.string) - 1);
            gettoken(); // get char : ]
        }
        gettoken();     // get char if is [
        printf("of ");
    }
}

void deal_with_function_args()
{
    while (this.type != ')')
    {
        gettoken(); // ingnore the arguments, and get the char )
    }

    gettoken();     // get the next token
    printf(" function returning ");
}

void deal_with_pointers()
{
    while (stack[top].type == '*')
    {
        printf("%s ", pop.string);
    }
}

void deal_with_declarator()
{
    switch (this.type)
    {
        case '[' :    // array 
            deal_with_arrays();
            break;
        case '(':       // function
            deal_with_function_args();
    }

    deal_with_pointers();   // pointer

    while (top >= 0)
    {
        if (stack[top].type == '(')
        { // this may be pointer function, then pop ( and ignore ), then get next token
            pop;
            gettoken();
            deal_with_declarator();
        }
        else
        {
            printf("%s ", pop.string);
        }
    }
}
