/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:27:31 by tlafay            #+#    #+#             */
/*   Updated: 2023/11/03 12:05:00 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argparse.h"

t_argp	*_g_argp = NULL;
char	**_g_argv = NULL;

static int _unrecognized_option(char sflag, char *lflag)
{
	if (sflag)
		printf("%s: invalid option -- '%c'\n", _g_argv[0], sflag);
	else
		printf("%s: unrecognized option '--%s'\n", _g_argv[0], lflag);

	printf("Try '%s --help' for more information\n", _g_argv[0]);
	
	return (1);
}

static t_argo	*_search_option(char sflag, char *lflag)
{
	for (t_argo *options = _g_argp->options; options->sflag || options->lflag; options++)
	{
		if ((sflag && sflag == options->sflag)
			|| (lflag && !ft_strncmp(lflag, options->lflag, ft_strlen(lflag))))
			return (options);
	}
	return (NULL);
}

static int _wrong_arguments_number(t_argo *option, int flagtype)
{
	char	*message;

	if (option->argnum == ONE_ARG)
		message = "an";
	else
		message = "at least one";

	if (flagtype == SFLAG)
		printf("%s: option requires %s argument -- '%c'\n", _g_argv[0], message, option->sflag);

	else
		printf("%s: option '%s' requires %s argument\n", _g_argv[0], option->lflag, message);

	printf("Try '%s --help' for more information\n", _g_argv[0]);

	return (1);
}

static int	_get_option_arguments(char **args, t_argo *option,
	char **values, int flagtype)
{
	// printf("%d\n", option->argnum);
	switch (option->argnum)
	{
	case NO_ARG:
		values = NULL;
		return (0);
	
	case ONE_ARG:
		if (!*args)
			return (_wrong_arguments_number(option, flagtype));
		values = (char **) malloc(2 * sizeof(char *));
		values[0] = *args;
		values[1] = NULL;
		return (0);

	default:
		return (1);
	}
	// for (; *args; args++)
}

static int _parse_short_option(char **args, t_list **head)
{
	t_argo	*option;
	t_argr	*ret;

	char *arg = args[0] + 1;
	args++;

	ret = (t_argr *) malloc(sizeof(t_argr));
	for (; *arg; arg++)
	{
		option = _search_option(*arg, NULL);

		if (!option)
			return (_unrecognized_option(*arg, NULL));
		if (_get_option_arguments(args, option, ret->values, SFLAG))
			return (1);
	}

	return (0);
}

static int _parse_long_option(char **args, t_list **head)
{
	(void)head;
	(void)args;
	// char *arg = _g_argv[0] + 2;
	return (0);
}

static int _parse_option(char **args, t_list **head)
{
	if (*(args[0] + 1) == '-')
		return _parse_long_option(args, head);
	return _parse_short_option(args, head);
}

void	help_args(t_argp *argp, const char *prog_name)
{
	t_argo	*options = argp->options;

	if (argp->args_doc)
		printf("Usage: %s %s\n", prog_name, argp->args_doc);

	if (argp->doc)
		printf("%s\n", argp->doc);

	for (; options->sflag || options->lflag; options++)
		printf("  -%c, --%-17s%s\n", options->sflag, options->lflag, options->help);

	printf("  -%c, --%-17s%s\n", 'h', "help", "display this help and exit");
}

t_list	*parse_args(t_argp *argp, int argc, char const *argv[])
{
	t_list	*head;

	_g_argp = argp;
	_g_argv = (char **) argv;
	head = NULL;
	for (int i = 1; i < argc && _g_argv[i]; i++)
	{
		if (_g_argv[i][0] == '-' && _g_argv[i][1])
			if (_parse_option(_g_argv + i, &head))
				return (NULL);
		// else
		// 	ft_lstadd_back(arg);
	}
	return (head);
}
