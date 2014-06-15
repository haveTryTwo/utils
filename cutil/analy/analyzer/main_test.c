#include <stdio.h>

#include "analyze.h"

int test_analyze();

int main()
{
    test_analyze();

    return 0;
}

int test_analyze()
{
    read_to_first_identifer();
    deal_with_declarator();
    printf("\n");

    return 0;
}
