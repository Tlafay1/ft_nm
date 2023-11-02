#include "argparse.h"
#include "libft.h"

static t_argo options[] = {
	{'v', "verbose", "Enable verbose mode", NULL, 0},
	{'i', "input", "Specify input file", "str", 1},
	{'o', "output", "Specify output file", "str", 1},
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