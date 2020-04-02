#include <stdio.h>
#include "ziplist.h"

int main() {
	unsigned char *ziplist = ziplistNew();

	// 从最后一个节点开始插入节点
	ziplist = ziplistPush(ziplist, (unsigned char *)"C++", 3, ZIPLIST_TAIL);
	ziplist = ziplistPush(ziplist, (unsigned char *)"11", 2, ZIPLIST_TAIL);
	ziplist = ziplistPush(ziplist, (unsigned char *)"C", 1, ZIPLIST_TAIL);
	ziplist = ziplistPush(ziplist, (unsigned char *)"99", 2, ZIPLIST_TAIL);

	unsigned char *curr = ziplist + ziplistBlobLen(ziplist) - 1;
	while ((curr = ziplistPrev(ziplist, curr)) != NULL) {
		unsigned char *entry = NULL;
		unsigned int entry_len = 0;
		long long entry_int_data = 0;
		if (ziplistGet(curr, &entry, &entry_len, &entry_int_data) == 0) {
			continue;
		}

		if (entry_len > 0) {
			unsigned char c = entry[entry_len];
			entry[entry_len] = 0;
			printf("string(%s) ", entry);
			entry[entry_len] = c;
			continue;
		}
		
		printf("number(%lld) ", entry_int_data);
	}

	printf("\n");
	
	return 0;
}


