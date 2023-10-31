/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:27:31 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/31 18:30:18 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparse.h"

t_argp	*_g_argp = NULL;
char	**_g_argv = NULL;

static int _exit_on_error(char *message)
{
	help_args(_g_argp);
	return (1);
}

static int _parse_short_option(char **args, t_argl *head)
{	
	char *arg = args[0] + 1;
	args++;

	return (0);
}

static int _parse_long_option(char **args, t_argl *head)
{
	char *arg = args[0] + 2;
	args++;
	return (0);
}

static int _parse_option(char **args, t_argl *head)
{
	char *arg = args[0] + 1;

	if (*arg == '-')
		_parse_long_option(args, head);
	else
		_parse_short_option(args, head);
	return (0);
}

void	help_args(const t_argp *argp)
{
	t_argo	*options = argp->options;
	if (argp->args_doc)
		printf("Usage: %s %s\n", _g_argv[0], argp->args_doc);
	
	if (argp->doc)
		printf("%s\n", argp->doc);
	
	for (; options; options++)
		printf("\t-%c, --%-17s%s\n", options->sflag, options->lflag, options->help);
}

t_argl	*parse_args(const t_argp *argp, int argc, char **argv)
{
	char	*arg;
	t_argl	*head;

	_g_argp = argp;
	_g_argv = argv;
	for (int i = 1; i < argc && argv[i]; i++)
	{
		arg = argv[i];
		if (*arg == '-' && arg + 1)
			if (_parse_option(argv, head))
				return (NULL);
		// else
		// 	ft_lstadd_back(arg);
	}
	return (head);
}