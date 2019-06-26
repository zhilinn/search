#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "options.h"

void parse_options(OPTIONS* options, int argc, char** argv) {
	options->ignore_case = false;
	options->print_positions = false;
	options->recursive = false;
	options->show_stats = false;
	options->verbose = false;

	int argn = 0;
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 1; argv[i][j]; j++) {
				switch (argv[i][j]) {
				case 'i': options->ignore_case = true; break;
				case 'p': options->print_positions = true; break;
				case 'r': options->recursive = true; break;
				case 's': options->show_stats = true; break;
				case 'v': options->verbose = true; break;
				}
			}
		}
		else {
			switch (argn++) {
			case 0: options->directory = argv[i]; break;
			case 1: options->text = argv[i]; break;
			}
		}
	}

	if (argn != 2) {
		fprintf(stderr,
			"Usage: %s [options] <directory> <text>\n"
			"  -i: Ignore case\n"
			"  -p: Print positions\n"
			"  -r: Recursive\n"
			"  -s: Show statistics\n"
			"  -v: Verbose\n"
			"\n",
			argv[0]);

		exit(EXIT_FAILURE);
	}
}
