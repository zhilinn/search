#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <windows.h>
#include "options.h"
#include "util.h"

void search_in_file(OPTIONS* options, char* path) {
	FILE* file = fopen(path, "r");
	size_t pos_line = 1;
	size_t pos_char = 1;
	size_t text_cursor = 0;
	char text_char;
	int file_char;

	if (file == NULL) {
		if (options->verbose) {
			fprintf(stderr, "ERROR %s (%s)\n", path, strerror(errno));
		}
		return;
	}

	while ((file_char = getc(file)) != EOF) {
		text_char = options->text[text_cursor];

		if (options->ignore_case) {
			file_char = to_upper(file_char);
			text_char = to_upper(text_char);
		}

		if (text_char == '\0') {
			if (options->print_positions) {
				printf("%s (%Iu:%Iu)\n", path, pos_line, pos_char - text_cursor);
			}
			else {
				printf("%s\n", path);
				fclose(file);
				return;
			}
		}

		if (file_char == '\n') {
			pos_line++;
			pos_char = 1;
		}
		else {
			pos_char++;
		}

		text_cursor = text_char == file_char
			? text_cursor + 1
			: 0;
	}

	fclose(file);
}

void search_in_dir(OPTIONS* options, char* path) {
	WIN32_FIND_DATA fd;
	HANDLE h;
	char* full_path;
	char* pattern;

	format(&pattern, "%s\\*", path);
	h = FindFirstFile(pattern, &fd);
	free(pattern);

	if (h == INVALID_HANDLE_VALUE) {
		if (options->verbose) {
			fprintf(stderr, "ERROR %s\n", path);
		}
		return;
	}

	do {
		format(&full_path, "%s\\%s", path, fd.cFileName);
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (options->recursive && strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")) {
				search_in_dir(options, full_path);
			}
		}
		else {
			search_in_file(options, full_path);
		}
		free(full_path);
	}
	while (FindNextFile(h, &fd));

	FindClose(h);
}
