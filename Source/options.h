#pragma once
#include <stdbool.h>

typedef struct OPTIONS OPTIONS;

struct OPTIONS {
	bool ignore_case;
	bool print_positions;
	bool recursive;
	bool show_stats;
	bool verbose;
	char* directory;
	char* text;
};

void parse_options(OPTIONS* options, int argc, char** argv);
