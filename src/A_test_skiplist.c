#include <stdio.h>
#include "server.h"

int main(int argc, char *argv[]) {
	zskiplist *zsl = zslCreate();
	zslFree(zsl);
	return 0;
}
