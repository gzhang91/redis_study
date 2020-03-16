#include <stdio.h>
#include "server.h"
#include "A_skiplist.h"
#include "sds.h"

int main() {
	Azskiplist *zsl = AzslCreate();

	sds str1 = sdsnewlen("gzhang", 6);
	AzslInsert(zsl, 1.0, str1);
	
	
	AzslFree(zsl);
	return 0;
}
