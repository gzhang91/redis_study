#include <stdio.h>
#include "server.h"
#include "A_skiplist.h"
#include "sds.h"
#include <time.h>

int main() {
	srand(time(0));
	Azskiplist *zsl = AzslCreate();

	// 语言难度系数排行
	sds str1 = sdsnewlen("C++", 3);
	AzslInsert(zsl, 100.0, str1);
	AzslVisit(zsl);

	sds str2 = sdsnewlen("C", 1);
	AzslInsert(zsl, 88.0, str2);
	AzslVisit(zsl);

	sds str3 = sdsnewlen("Perl", 4);
	AzslInsert(zsl, 93.0, str3);
	AzslVisit(zsl);
	
	sds str4 = sdsnewlen("Delphi", 6);
	AzslInsert(zsl, 54.0, str4);
	AzslVisit(zsl);

	sds str5 = sdsnewlen("Python", 6);
	AzslInsert(zsl, 67.0, str5);
	AzslVisit(zsl);

	sds str6 = sdsnewlen("Ruby", 4);
	AzslInsert(zsl, 32.0, str6);
	AzslVisit(zsl);

	sds str7 = sdsnewlen("Java", 4);
	AzslInsert(zsl, 59.0, str7);
	AzslVisit(zsl);

	sds str8 = sdsnewlen("C1", 2);
	AzslInsert(zsl, 25.0, str8);
	AzslVisit(zsl);

	sds str9 = sdsnewlen("C2", 2);
	AzslInsert(zsl, 34.0, str9);
	AzslVisit(zsl);

	sds str10 = sdsnewlen("C3", 2);
	AzslInsert(zsl, 11.0, str10);
	AzslVisit(zsl);

	sds str11 = sdsnewlen("C4", 2);
	AzslInsert(zsl, 96.0, str11);
	AzslVisit(zsl);

	sds str12 = sdsnewlen("C5", 2);
	AzslInsert(zsl, 85.0, str12);
	AzslVisit(zsl);
	
	AzslFree(zsl);
	return 0;
}
