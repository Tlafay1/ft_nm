/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argparse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:28:05 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/31 18:29:02 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGPARSE_H
# define ARGPARSE_H

# include <stdio.h>

typedef struct s_arg_option
{
	char	sflag;
	char	*lflag;
	char	*help;
	char	*type;
	int		valnum;
}	t_argo;

typedef struct s_arg_return
{
	char	*name;
	char	**values;
	char	key;
	char	*type;
}	t_argr;

typedef struct s_argp_parse
{
	t_argo	*options;
	char	*args_doc;
	char	*doc;
}	t_argp;

typedef struct s_arg_list
{
	t_argr			*arg;
	struct s_argl	*next;
}	t_argl;

t_argl	*parse_args(const t_argp *argp, int argc, char **argv);
void	help_args(const t_argp *argp);

#endif