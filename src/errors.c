/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 18:01:40 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/31 11:39:29 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"


/*
	Checks if the file can be opened sucessfully
*/

int	check_filetype(mode_t m, char *prog_name, char *path)
{
	char	*message = NULL;

	if (S_ISDIR(m))
		message = "is a directory";
	
	else if (!S_ISREG(m))
		message = "is not an ordinary file";
	
	if (message)
		printf("%s: Warning: '%s' %s\n", prog_name, path, message);
	
	return !(message == NULL);
}


/*
	Error message when opening the file
*/

int	nm_error(char *prog_name, char *path, char *msg)
{
	printf("%s: %s: %s\n", prog_name, path, msg);
	return (1);
}


/*
	Error returned when a syscall failed
*/

int	syscall_error(char *prog_name, char *path)
{
	if (errno == ENOENT)
		printf("%s: '%s': No such file\n", prog_name, path);
	else
		printf("%s: Warning: '%s' is not an ordinary file\n", prog_name, path);
	return (1);
}

