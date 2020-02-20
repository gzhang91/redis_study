#include <stdio.h>
#include "sds.h"

int main() {
    printf("hello world\n");

    sds str = sdsnewlen("gzhang123", 500);
    printf("sds string: %s, len=%zu\n", str, sdslen(str));

	unsigned char flag = str[-1] & 7;
	if (flag == SDS_TYPE_8) {
		printf("===&str=%p, sizeof(struct sdshdr8)=%zu\n", str, sizeof(struct sdshdr8));
		struct sdshdr8* hdr_str = SDS_HDR(8, str);
		printf("===&hdr=%p\n", (void *)hdr_str);

		printf("len: %d, buf: %s, alloc: %d, flag: %d", 
			hdr_str->len, 
			hdr_str->buf,
			hdr_str->alloc,
			hdr_str->flags & 7);
	} else if (flag == SDS_TYPE_16) {
		printf("===&str=%p, sizeof(struct sdshdr16)=%zu\n", str, sizeof(struct sdshdr16));
		struct sdshdr16* hdr_str = SDS_HDR(16, str);
		printf("===&hdr=%p\n", (void *)hdr_str);

		printf("len: %d, buf: %s, alloc: %d, flag: %d", 
			hdr_str->len, 
			hdr_str->buf,
			hdr_str->alloc,
			hdr_str->flags & 7);
	}

    return 0;
}
