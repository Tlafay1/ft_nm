/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:28 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/30 18:05:56 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

/*
	Need to define the variable once
*/

t_file	g_file;


/*
	Read the file specified in argument.
	Assigns the global structure to the
	corresponding values.
	Returns -1 if a syscall failed, 1 if
	the path is a dir and 2 if it's not a
	regular file. 
*/

int read_file(char *path)
{
	int			fd;
	struct stat	buf;

	fd = open(path, O_RDONLY);
	
	if (fd < 0)
		return -1;
	
	if (fstat(fd, &buf) == -1)
		return -1;
	
	if (S_ISDIR(buf.st_mode))
		return 1;
	
	if (!S_ISREG(buf.st_mode))
		return 2;

	g_file.size = buf.st_size;
	g_file.buffer = mmap(0, g_file.size, PROT_READ, MAP_PRIVATE, fd, 0);
	g_file.end = g_file.buffer + g_file.size;
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

int	is_64bits(Elf64_Ehdr *header)
{
	return (header->e_ident[EI_CLASS] == ELFCLASS64);
}

int	is_32bits(Elf32_Ehdr *header)
{
	return (header->e_ident[EI_CLASS] == ELFCLASS32);
}

int	parse_elf(char *prog_name, char *path)
{
	int	ret = 0;
	if (!out_of_bounds(g_file.buffer + sizeof(Elf64_Ehdr))
		&& is_32bits((Elf32_Ehdr *)g_file.buffer))
		ret = parse_32bits();
	else if (!out_of_bounds(g_file.buffer + sizeof(Elf64_Ehdr))
		&& is_64bits((Elf64_Ehdr *)g_file.buffer))
		ret = parse_64bits();
	else
		return nm_error(prog_name, path, "file format not recognized");

	if (ret)
		return nm_error(prog_name, path, "no symbols");

	return (0);
}

int	ft_nm(char *prog_name, char *path, int print)
{
	int status = read_file(path);
	if (status < 0)
		return (syscall_error(prog_name, path));

	if (status == 1)
		return nm_error(prog_name, "Warning", ft_strjoin(path, " is a directory"));

	if (status == 2)
		return nm_error(prog_name, "Warning", ft_strjoin(path, " is not an ordinary file"));

	if (out_of_bounds(g_file.buffer + EI_NIDENT)
		|| !is_elf((Elf32_Ehdr *)g_file.buffer))
		return nm_error(prog_name, path, "file format not recognized");
	
	if (print)
		printf("\n%s:\n", path);

	int ret = parse_elf(prog_name, path);

	munmap(g_file.buffer, g_file.size);
	return (ret);
}

int	main(int argc, char **argv)
{
	int		ret;

	ret = 0;
	if (argc == 1)
		ret += ft_nm(argv[0], "a.out", 0);

	for (int i = 1; argv[i]; i++)
		// We basically check if there's more than 1 file in there
		ret += ft_nm(argv[0], argv[i], i - 1 || argv[i + 1]);
	
	return (ret);
}
