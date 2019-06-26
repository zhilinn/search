#include <stdio.h>
#include <windows.h>

void format(char** str, char* fmt, ...) {
	va_list al;
	va_start(al, fmt);
	size_t size = (size_t)vsnprintf(NULL, 0, fmt, al) + 1;
	*str = malloc(size);
	vsnprintf(*str, size, fmt, al);
	va_end(al);
}

char to_upper(char c) {
	return 'a' <= c && c <= 'z'
		? c & ~0x20
		: c;
}

char to_lower(char c) {
	return 'A' <= c && c <= 'Z'
		? c | 0x20
		: c;
}
