#include <stdbool.h>
#include <stdlib.h>
#include "options.h"
#include "search.h"

void main(int argc, char** argv) {
	OPTIONS options;
	parse_options(&options, argc, argv);
	search_in_dir(&options, options.directory);
	exit(EXIT_SUCCESS);
}
