#include <stdio.h>
#include "MyBasics.h"

int main(void)
{
    int a = -2;
    strToInt("99999999999999999999999999999999", &a);
    printf("%d", a);
    return 0;
}
