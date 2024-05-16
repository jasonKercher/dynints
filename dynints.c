#include <stdlib.h>
#include <string.h>

struct Dynamic_Ints {
	int* data;
	int  len;
	int  cap;
};

struct Dynamic_Ints di_make(int len, int cap) {
	struct Dynamic_Ints di = {
	    .len = len,
	    .cap = cap > len ? cap : len * 2,
	};
	if (cap > 0) { di.data = calloc(di.cap, sizeof(int)); }
	return di;
}

void di_delete(struct Dynamic_Ints di) {
	if (di.data != NULL) {
		free(di.data);
	}
}

void _set_cap(struct Dynamic_Ints* di, int cap) {
	di->cap = cap;
	int* new_data = realloc(di->data, di->cap * sizeof(int));
	if (new_data != NULL) {
		di->data = new_data;
	}
}

void di_shrink_to_fit(struct Dynamic_Ints* di) {
	if (di->data == NULL) {
		return;
	}
	di->cap = di->len;
	_set_cap(di, di->cap);
}

void di_reserve(struct Dynamic_Ints* di, int new_cap) {
	if (di->cap > new_cap) {
		return;
	}
	if (di->data == NULL) {
		*di = di_make(0, new_cap);
		return;
	}
	_set_cap(di, new_cap);
}

void di_resize(struct Dynamic_Ints* di, int new_len) {
	if (new_len > di->cap) {
		int new_cap = di->cap * 2 > new_len ? di->cap * 2 : new_len * 2;
		di_reserve(di, new_cap);
	}
	di->len = new_len;
}

void di_clear(struct Dynamic_Ints* di) {
	di_resize(di, 0);
}

void di_append(struct Dynamic_Ints* di, int elem) {
	if (di->data == NULL) {
		*di = di_make(1, 16);
	} else {
		di_resize(di, di->len + 1);
	}
	di->data[di->len - 1] = elem;
}

void di_append_elems(struct Dynamic_Ints* di, const int* data, int n) {
	int old_len = di->len;
	if (di->data == NULL) {
		*di = di_make(n, 16);
	} else {
		di_resize(di, di->len + n);
	}
	memmove(&di->data[old_len], data, n * sizeof(int));
}

void di_remove(struct Dynamic_Ints* di, int idx) {
	if (idx > di->len - 1) {
		return;
	}
	di->len -= 1;
	if (idx == di->len) {
		return;
	}

	int copy_size = (di->cap - idx - 1) * sizeof(int);
	memmove(&di->data[idx], &di->data[idx + 1], copy_size);
}
