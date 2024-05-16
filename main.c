#include <stdio.h>
#include "dynints.c"

/* replicate fmt.println from main.odin */
void _print_dynints(struct Dynamic_Ints di, const char* msg) {
	printf("%-21sDynamic_Ints{data = %p, len = %d, cap = %d} [", msg, di.data, di.len, di.cap);
	for (int i = 0; i < di.len; i += 1) {
		if (i != 0) {
			printf(", ");
		}
		printf("%d", di.data[i]);
	}
	printf("]\n");
}

int main(int argc, char** argv) {
	struct Dynamic_Ints di = di_make(0, 16);
	/* will cause data to move when it grows */
	struct Dynamic_Ints unused = di_make(0, 16);
	_print_dynints(di, "After Make:");

	for (int i = 0; i < 14; i += 1) {
		di_append(&di, i);
	}
	_print_dynints(di, "After appends:");

	di_remove(&di, 6);
	di_remove(&di, 5);
	di_remove(&di, 3);
	_print_dynints(di, "After removes:");

	int more_elems[] = {7,6,5,4,3,2,1,0};
	di_append_elems(&di, more_elems, sizeof(more_elems) / sizeof(int));
	_print_dynints(di, "After append_elems:");

	di_shrink_to_fit(&di);
	_print_dynints(di, "After shrink_to_fit:");

	di_clear(&di);
	_print_dynints(di, "After clear:");

	di_delete(di);
	_print_dynints(di, "After delete:");
}
