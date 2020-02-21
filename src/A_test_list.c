#include <stdio.h>
#include <string.h>
#include "adlist.h"

typedef struct Book {
	char name[32];
	int page_size;
	char writer[32];
} Book;

void* DupBook(void *ptr) {
	struct Book *ori_bk = (struct Book*)ptr;
	if (!ori_bk) {
		printf("unknown type!\n");
		return NULL;
	}

	struct Book *res_bk = (struct Book*)malloc(sizeof(*ori_bk));
	if (!res_bk) {
		printf("malloc error!\n");
		return NULL;
	}
	
	strncpy(res_bk->name, ori_bk->name, sizeof(ori_bk->name));
	res_bk->page_size = ori_bk->page_size;
	strncpy(res_bk->writer, ori_bk->name, sizeof(ori_bk->writer));

	return res_bk;
}

void FreeBook(void *ptr) {
	struct Book *ori_bk = (struct Book*)ptr;
	if (!ori_bk) {
		printf("unknown type!\n");
		return;
	}

	free(ori_bk);
}

int CompareBook(void *ptr, void *key) {
	struct Book *fst_bk = (struct Book*)ptr;
	if (!fst_bk) {
		printf("unknown type!\n");
		return 0;
	}

	struct Book *snd_bk = (struct Book*)key;
	if (!snd_bk) {
		printf("unknown type!\n");
		return 0;
	}

	// strncmp是有缺陷的，这里只是测试程序
	return fst_bk->page_size == snd_bk->page_size &&
		(strncmp(fst_bk->name, snd_bk->name, strlen(fst_bk->name)) == 0) &&
		(strncmp(fst_bk->writer, snd_bk->writer, strlen(fst_bk->writer)) == 0);
}

struct Book* NewBook(const char* name, const char* writer, int page) {
	struct Book* bk_tmp = (struct Book*)malloc(sizeof(struct Book));
	if (!bk_tmp) {
		printf("malloc error!\n");
		return NULL;
	}

	strncpy(bk_tmp->name, name, strlen(name));
	strncpy(bk_tmp->writer, writer, strlen(writer));
	bk_tmp->page_size = page;

	return bk_tmp;
}

void PrintBook(struct Book* book) {
	printf("Book info: name(%s), writer(%s), page(%d)\n", book->name, book->writer, book->page_size);
}

void ForeachList(struct list* lst) {
	listIter *iter = listGetIterator(lst, AL_START_HEAD);
	listNode *node = NULL;
	while (node = listNext(iter)) {
		PrintBook((struct Book *)node->value);
	}
	listReleaseIterator(iter);
}

int main() {
	printf("list test ...\n");

	struct list* lst = listCreate();
	listSetDupMethod(lst, *DupBook);
	listSetFreeMethod(lst, *FreeBook);
	listSetMatchMethod(lst, *CompareBook);

	struct Book* bk1 = NewBook("C++", "bjm", 456);
	listAddNodeHead(lst, bk1);

	struct Book* bk2 = NewBook("C", "Riche", 234);
	listAddNodeHead(lst, bk2);

	struct Book* bk3 = NewBook("Java", "Sun", 1000);
	listAddNodeHead(lst, bk3);

	printf("insert head: \n");
	ForeachList(lst);

	struct Book* bk4 = NewBook("Python", "snake", 800);
	listAddNodeTail(lst, bk4);

	printf("insert tail: \n");
	ForeachList(lst);

	listRelease(lst);
	printf("clear list ...\n");
	
	return 0;
}


