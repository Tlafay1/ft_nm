/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:28 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/31 11:37:46 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>


t_file			g_file;
t_nm_options	g_options;

/*
	Read the file specified in argument.
	Assigns the global structure to the
	corresponding values.
	Returns -1 if a syscall failed and
	1 we are not opening a regular file.
*/

int read_file(const char *prog_name, const char *path)
{
	int fd;
	struct stat buf;

	fd = open(path, O_RDONLY);

	if (fd < 0 || fstat(fd, &buf) == -1)
		return syscall_error(prog_name, path);

	if (check_filetype(buf.st_mode, prog_name, path))
		return (1);

	g_file.size = buf.st_size;
	g_file.buffer = mmap(0, g_file.size, PROT_READ, MAP_PRIVATE, fd, 0);
	g_file.end = g_file.buffer + g_file.size;
	close(fd);
	return 0;
}

/*
	Check if the file is an elf executable.
	We use the magic number for that.
*/

int is_elf(Elf32_Ehdr *header)
{
	if (ft_memcmp(header->e_ident, ELFMAG, SELFMAG) == 0)
		return 1;
	return 0;
}

/*
	Check if the file is 64 bits.
*/

int is_64bits(Elf64_Ehdr *header)
{
	return (header->e_ident[EI_CLASS] == ELFCLASS64);
}

int is_32bits(Elf32_Ehdr *header)
{
	return (header->e_ident[EI_CLASS] == ELFCLASS32);
}

int parse_elf(const char *prog_name, const char *path)
{
	int ret = 0;

	if (!out_of_bounds(g_file.buffer + sizeof(Elf64_Ehdr)) && is_32bits((Elf32_Ehdr *)g_file.buffer))
		ret = parse_32bits();

	else if (!out_of_bounds(g_file.buffer + sizeof(Elf64_Ehdr)) && is_64bits((Elf64_Ehdr *)g_file.buffer))
		ret = parse_64bits();

	else
		return nm_error(prog_name, path, "file format not recognized");

	if (ret)
		return nm_error(prog_name, path, "no symbols");

	return (0);
}

int ft_nm(const char *prog_name, const char *path, int print)
{
	if (read_file(prog_name, path))
		return (1);

	if (out_of_bounds(g_file.buffer + EI_NIDENT) || !is_elf((Elf32_Ehdr *)g_file.buffer))
		return (nm_error(prog_name, path, "file format not recognized"));

	if (print)
		printf("\n%s:\n", path);

	int ret = parse_elf(prog_name, path);

	munmap(g_file.buffer, g_file.size);
	return (ret);
}

void	nm_parse_options(t_list *head)
{
	t_nm_options	options = {0, 0, 0, 0, 0};
	t_argr			*argr;

	while ((argr = get_next_option(head)))
	{
		if (argr->option->sflag == 'a')
			options.debug_syms = 1;
		else if (argr->option->sflag == 'g')
			options.extern_only = 1;
		else if (argr->option->sflag == 'p')
			options.no_sort = 1;
		else if (argr->option->sflag == 'r')
			options.reverse_sort = 1;
		else if (argr->option->sflag == 'u')
			options.undefined_only = 1;
	}

	g_options = options;
}

static t_argo	options[] = {
	{'a', "debug-syms", "debug-syms", "Display debugger-only symbols", NO_ARG},
	{'g', "extern-only", "extern-only", "Display only external symbols", NO_ARG},
	{'p', "no-sort", "no-sort", "Do not sort the symbols", NO_ARG},
	{'r', "reverse-sort", "reverse-sort", "Sort in reverse order", NO_ARG},
	{'u', "undefined-only", "undefined-only", "Display only undefined symbols", NO_ARG},
	{0}
};

static t_argp argp = {
	.options = options,
	.args_doc = "[option(s)] [file(s)]",
	.doc = " List symbols in [file(s)] (a.out by default).\n The options are:",
};

int main(int argc, const char **argv)
{
	int		ret;
	t_list *head;
	t_argr	*arg;
	int		count;

	ret = 0;
	if (!(head = parse_args(&argp, argc, argv)))
		return (1);
	nm_parse_options(head);
	count = args_count(head);
	if (!count)
		ret += ft_nm(argv[0], "a.out", 0);

	else
		while ((arg = get_next_arg(head)))
			ret += ft_nm(argv[0], arg->values[0], count > 1);

	free_args(head);
	return (ret);
}
