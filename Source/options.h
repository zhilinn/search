#pragma once
#include <stdbool.h>

typedef struct OPTIONS OPTIONS;

struct OPTIONS {
	bool verbose;
	bool recursive;
	bool print_positions;
	bool ignore_case;
	char* directory;
	char* text;
};

void parse_options(OPTIONS* options, int argc, char** argv);
