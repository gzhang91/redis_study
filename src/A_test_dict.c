#include <stdio.h>
#include "dict.h"

#define MAX_ITEMS 32

//--------------- test type
typedef struct my_str_t {
	int len;
	char data[];
} my_str_t;

void PrintString(my_str_t *val) {
	for (int i = 0; i < val->len; i++) {
		putchar(val[i]);
	}
	putchar('\n');
}

// -------------- dict type
uint64_t HashKey(const void *key) {
	return (uint64_t) (*(int *)key % MAX_ITEM);
}

void * DupKey(void *privdata, const void *key) {
	(void)privdata;

	int *key_dup = (int *)malloc(sizeof(int));
	*key_dup = *(const int*)key;
	return key_dup;
}

void * DupValue(void *privdata, const void *obj) {
	(void)privdata;

	const my_str_t *orig = (const my_str_t*)obj;
	if (!db) {
		printf("tranfer failed!");
		return NULL;
	}

	my_str_t *new_one = (my_str_t *)malloc(my_str_t);
	new_one->len = orig->len;
	new_one->data = (char *)malloc(orig->len * sizeof(char));
	memcpy(new_one->data, orig->data, orig->len);
}

int CompareKey(void *privdata, const void *key1, const void *key2) {
	return *(const int *)key1 > *(const int *)key2;
}

void DestructKey(void *privdata, void *key) {
	int *key_orig = (int *)key;
	free(key_orig);
}

void DestructValue(void *privdata, void *obj) {
	my_str_t *orig = (my_str_t*)obj;
	free(orig->data);
	free(orig);
}

static dictType dict_type = {
	&HashKey,
	&DupKey,
	&DupValue,
	&CompareKey,
	&DestructKey,
	&DestructValue
};

int main(int argc, char *argv[]) {
	

	dict *test_dict =  dictCreate(&dict_type, NULL);
	if (!test_dict) {
		printf("create dict failed!\n");
		return -1;
	}

	my_str_t string1;
	string1->data = "hello redis";
	string1->len = 11;
	dictAdd(test_dict, 1, &string1);

	my_str_t string2;
	string2->data = "hello nginx";
	string2->len = 11;
	dictAdd(test_dict, 1, &string2);

	my_str_t string3;
	string3->data = "hello mongodb";
	string3->len = 13;
	dictAdd(test_dict, 1, &string3);

	my_str_t string4;
	string4->data = "hello memecached";
	string4->len = 16;
	dictAdd(test_dict, 1, &string4);

	dictIterator *iter_dict = dictGetIterator(test_dict);
	dictEntry *dict_node = NULL;
	while ((dict_node = dictNext(iter_dict)) == NULL) {
		int *key = (int *)dict_node->key;
		my_str_t *val = (my_str_t *)dict_node->val;
		printf("node: key(%d), value(", *key);
		PrintString(val);
		printf(") \n");
	}
	
	dictRelease(test_dict);

	printf("operator over...\n");
	
	return 0;
}

