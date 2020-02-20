#include <stdio.h>
#include "sds.h"

int main() {
    printf("hello world\n");

    sds str = sdsnewlen("gzhang123", 32);
    printf("sds string: %s\n", str);

    return 0;
}
