#include "ush.h"

bool mx_str_head(const char *where, const char *what) {
	for (int i = 0; where[i] && what[i]; i++)
		if (where[i] != what[i])
			return 0;
	return 1;
}
