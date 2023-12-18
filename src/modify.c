#include <stdio.h>

int main(void)
{
    FILE* f = fopen("protected", "w");
    printf("file is opened: %d\n", f);
    fprintf(f, "It is modified now\n");
    if (f)
        fclose(f);
    return 0;
}