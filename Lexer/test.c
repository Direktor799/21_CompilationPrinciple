#include <stdio.h>
/* this is
a comment */
// this is another comment
int main()
{
    int bin = 0b10;
    unsigned int oct = 0123;
    long long dec = 123;
    short hex = 0x12;
    unsigned long long longint = 123456789ULL;
    float f = 3.14f;
    double d = 5e1;
    char c = 'abcd';
    printf("\q", c);
    char s[] = "This is a string";
    if (c == 'c')
        printf("wubba lubba dub 
        dub\n");
    else
        goto here;
here:
    for (dec = 0; dec < 10; dec++)
        continue;
    switch (c)
    {
        case '1':
            break;

        default:
            break;
    }
    return 0;
}
/*123