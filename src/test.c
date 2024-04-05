#include "argparse.h"

/**
 * This array of structures holds the informations about the args expected.
 * The last element of the array must be {0}.
 **/
static t_argo options[] = {
	{'v', "verbose", "verbose", "Enable verbose mode", NO_ARG},
	{'t', "test", "test", "Display a test", NO_ARG},
	{'o', "output", "output", "Specify output file", ONE_ARG},
	{'i', "input", "input", "Specify input file", INF_ARG},
	{0}};

/**
 * This structure holds the informations about the parser.
 * The members are:
 * - options: the array of structures that holds the informations about the args expected.
 * - args_doc: a string that describes the non-option command-line arguments to use in the help menu.
 * - doc: a string that describes the program to the user in the help menu.
 **/

static t_argp argp = {
	.options = options,
	.args_doc = "INPUT_FILE OUTPUT_FILE",
	.doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
};

int main(int argc, char const *argv[])
{
	t_list *head = NULL;
	// Parse the arguments and store them in a linked list.
	t_argr *argr;

	parse_args(&argp, argv, &head);
	// Loop through the options (e.g ./program -abc --option).
	while ((argr = get_next_option(head)))
	{
		// Be careful, the option can be NULL.
		if (argr->option)
		{
			printf("sflag: %c, lflag: %s, name: %s\n", argr->option->sflag,
				   argr->option->lflag, argr->option->name);
		}
		// Be careful, the values can be NULL.
		if (argr->values)
			for (int i = 0; argr->values[i]; i++)
				printf("%s\n", argr->values[i]);
	}

	// Loop through the arguments (e.g ./program arg1 arg2 arg3).
	while ((argr = get_next_arg(head)))
	{
		// Be careful, the option can be NULL.
		if (argr->option)
		{
			printf("sflag: %c, lflag: %s, name: %s\n", argr->option->sflag,
				   argr->option->lflag, argr->option->name);
		}
		// Be careful, the values can be NULL.
		if (argr->values)
			for (int i = 0; argr->values[i]; i++)
				printf("%s\n", argr->values[i]);
	}

	// IMPORTANT: don't forget to free the linked list !
	free_args(head);

	return 0;
}