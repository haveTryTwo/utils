#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static int check_no_buf();
static int check_line_buf();
static int check_full_buf();

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf("Plase input IO buffer type:\n" 
               " N : no buffer\n"
               " L : line buffer\n"
               " F : full buffer\n");
        exit(0);
    }
    
    if (strcmp(argv[1], "N") == 0)
    {
        check_no_buf();
    }
    else if (strcmp(argv[1], "L") == 0)
    {
        check_line_buf();
    }
    else if (strcmp(argv[1], "F") == 0)
    {
        check_full_buf();
    }

    return 0;
}

static int check_no_buf()
{
    int i = 0;
    
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("IO NO Buffer\n");
    for (i = 0; i < 3; i++)
    {
        printf("hello world");
        sleep(1);
    }

    return 0;
}

static int check_line_buf()
{
    int i = 0;

    setvbuf(stdout, NULL, _IOLBF, 0);
    printf("IO Line Buffer\n");
    for (i = 0; i < 3; i++)
    {
        printf("hello world\n");
        sleep(1);
    }

    return 0;
}

static int check_full_buf()
{
    int i = 0;

    setvbuf(stdout, NULL, _IOFBF, 0);
    printf("IO Full Buffer\n");
    for (i = 0; i < 3; i++)
    {
        printf("hello world\n");
        sleep(1);
    }

    return 0;
}
