#include "argparse.h"
#include "libft.h"

// What if I put the same name multiple times here ?
static t_argo options[] = {
	{'v', "verbose", "verbose", "Enable verbose mode", NULL, 0},
	{'i', "input", "input", "Specify input file", "str", ONE_ARG},
	{'o', "output", "output", "Specify output file", "str", 1},
	{0}
};

static t_argp argp = {
	.options = options,
	.args_doc = "INPUT_FILE OUTPUT_FILE",
	.doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
};

int main(int argc, char const *argv[])
{
	parse_args(&argp, argc, argv);
	return 0;
}