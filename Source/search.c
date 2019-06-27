#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <windows.h>
#include "options.h"
#include "util.h"

typedef struct STATS STATS;

struct STATS {
	size_t matches;
	size_t dirs;
	size_t files;
	size_t chars;
};

static void search_in_file(STATS* stats, OPTIONS* options, char* path) {
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

	stats->files++;

	while ((file_char = getc(file)) != EOF) {
		text_char = options->text[text_cursor];

		stats->chars++;

		if (text_char == '\0') {
			stats->matches++;

			if (options->print_positions) {
				printf("%s (%Iu:%Iu)\n", path, pos_line, pos_char - text_cursor);
			}
			else {
				printf("%s\n", path);
				break;
			}
		}

		if (file_char == '\n') {
			pos_line++;
			pos_char = 1;
		}
		else {
			pos_char++;
		}

		if (options->ignore_case) {
			file_char = to_upper(file_char);
			text_char = to_upper(text_char);
		}

		text_cursor = text_char == file_char
			? text_cursor + 1
			: 0;
	}

	fclose(file);
}

static void search_in_dir(STATS* stats, OPTIONS* options, char* path) {
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

	stats->dirs++;

	do {
		format(&full_path, "%s\\%s", path, fd.cFileName);
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (options->recursive && strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")) {
				search_in_dir(stats, options, full_path);
			}
		}
		else {
			search_in_file(stats, options, full_path);
		}
		free(full_path);
	}
	while (FindNextFile(h, &fd));

	FindClose(h);
}

void search(OPTIONS* options) {
	STATS stats = { 0, 0, 0, 0 };
	search_in_dir(&stats, options, options->directory);

	if (options->show_stats) {
		fprintf(stderr, "\n%Iu matches\n%Iu dirs\n%Iu files\n%Iu chars\n",
			stats.matches, stats.dirs, stats.files, stats.chars);
	}
}
