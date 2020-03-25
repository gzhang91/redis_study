#include <stdio.h>
#include "server.h"
#include "A_skiplist.h"
#include "sds.h"

int main() {
	Azskiplist *zsl = AzslCreate();

	// 语言难度系数排行
	sds str1 = sdsnewlen("C++", 3);
	AzslInsert(zsl, 100.0, str1);

	sds str2 = sdsnewlen("C", 1);
	AzslInsert(zsl, 88.0, str2);

	sds str3 = sdsnewlen("Perl", 3);
	AzslInsert(zsl, 93.0, str3);

	sds str4 = sdsnewlen("Delphi", 6);
	AzslInsert(zsl, 54.0, str4);

	sds str5 = sdsnewlen("Python", 6);
	AzslInsert(zsl, 67.0, str5);

	sds str6 = sdsnewlen("Ruby", 4);
	AzslInsert(zsl, 32.0, str6);

	sds str7 = sdsnewlen("Java", 4);
	AzslInsert(zsl, 55.0, str7);

	Azrangespec range = {50, 100, 1, 1};
	long rank = AzslFirstInRange(zsl, &range, )
	
	AzslFree(zsl);
	return 0;
}
