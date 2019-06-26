#include <stdbool.h>
#include <stdlib.h>
#include "options.h"
#include "search.h"

void main(int argc, char** argv) {
	OPTIONS options;
	parse_options(&options, argc, argv);
	search(&options);
	exit(EXIT_SUCCESS);
}
