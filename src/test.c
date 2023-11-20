#include "argparse.h"
#include "libft.h"

// What if I put the same name multiple times here ?
static t_argo options[] = {
	{'v', "verbose", "verbose", "Enable verbose mode", "str", NO_ARG},
	{'o', "output", "output", "Specify output file", "str", ONE_ARG},
	{'i', "input", "input", "Specify input file", "str", INF_ARG},
	{0}
};

static t_argp argp = {
	.options = options,
	.args_doc = "INPUT_FILE OUTPUT_FILE",
	.doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
};

int main(int argc, char const *argv[])
{
	t_list *head = parse_args(&argp, argc, argv);
	t_argr *argr = get_next_arg(head);

	if (argr->option)
	{
		printf("sflag: %c, lflag: %s, name: %s\n", argr->option->sflag,
			argr->option->lflag, argr->option->name);
	}
	for (int i = 0; argr->values[i]; i++)
		printf("%s\n", argr->values[i]);

	free_args(head);

	return 0;
}