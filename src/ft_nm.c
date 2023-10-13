/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:28 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/13 17:25:30 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"


/*
	Need to define the variable once
*/

t_file	g_file;


/*
	Read the file specified in argument.
	Returns a structure containing the size
	of the file and the pointer to the buffer. 
*/

int read_file(char *path)
{
	int			fd;
	struct stat	buf;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -1;
	if (fstat(fd, &buf) == -1)
		return -1;
	g_file.size = buf.st_size;
	g_file.buffer = mmap(0, g_file.size, PROT_READ, MAP_PRIVATE, fd, 0);
	g_file.end = g_file.buffer + g_file.size;
	return 0;
}


/*
	Error message thrown when the file is not ELF
*/

int	nm_error(char *prog_name, char *path, char *message)
{
	printf("%s: %s: %s\n", prog_name, path, message);
	return (1);
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

int	ft_nm(char *path, char *prog_name, int print)
{
	if (read_file(path))
		return nm_error(prog_name, path, "No such file");

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
		ret += ft_nm("a.out", argv[0], 0);

	for (int i = 1; argv[i]; i++)
		// We basically check if there's more than 1 file in there
		ret += ft_nm(argv[i], argv[0], i || !argv[i + 1]);
	return (ret);
}
