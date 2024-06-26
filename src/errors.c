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

/**
 * Checks if the file can be opened successfully.
 *
 * @param m         The file mode.
 * @param prog_name The name of the program.
 * @param path      The path of the file.
 * @return          1 if the file is not valid, 0 otherwise.
 */
int check_filetype(mode_t m, const char *prog_name, const char *path)
{
	char *message = NULL;

	if (S_ISDIR(m))
		message = "is a directory";
	else if (!S_ISREG(m))
		message = "is not an ordinary file";

	if (message)
		printf("%s: Warning: '%s' %s\n", prog_name, path, message);

	return !(message == NULL);
}

/**
 * Prints an error message when opening the file.
 *
 * @param prog_name The name of the program.
 * @param path      The path of the file.
 * @param msg       The error message.
 * @return          1.
 */
int nm_error(const char *prog_name, const char *path, char *msg)
{
	printf("%s: %s: %s\n", prog_name, path, msg);
	return (1);
}

/**
 * Prints an error message when a syscall fails.
 *
 * @param prog_name The name of the program.
 * @param path      The path of the file.
 * @return          1.
 */
int syscall_error(const char *prog_name, const char *path)
{
	if (errno == ENOENT)
		printf("%s: '%s': No such file\n", prog_name, path);
	else
		printf("%s: Warning: '%s' is not an ordinary file\n", prog_name, path);
	return (1);
}