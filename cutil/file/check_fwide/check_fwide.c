#include <stdio.h>
#include <wchar.h>

int check_fwide();

int main(int argc, char* argv[])
{
    check_fwide();

    return 0;
}

int check_fwide()
{
    FILE *fp = NULL;
    int fw = 0;
    char buf[1024] = {0};

    fp = fopen("Makefile", "r");

    fw = fwide(fp, 0);
    printf("current fwide is            :\033[33;1m %d \033[0m\n", fw);

    fread(buf, 1, sizeof(buf), fp);

    fw = fwide(fp, 0);
    printf("After reading, fwide is     :\033[33;1m %d \033[0m\n", fw);

    fw = fwide(fp, 1);
    printf("After setting 1, fwide is   :\033[33;1m %d \033[0m\n", fw);

    return 0;
}
