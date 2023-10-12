/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:28 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/12 17:30:06 by tlafay           ###   ########.fr       */
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

int	file_format_not_recognized(char **argv, char *path)
{
	printf("%s: '%s': file format not recognized\n", argv[0], path);
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

void	parse_elf(char **argv, char *path)
{
	if (!out_of_bounds(g_file.buffer + sizeof(Elf64_Ehdr))
		&& is_32bits((Elf32_Ehdr *)g_file.buffer))
		parse_32bits();
	else if (!out_of_bounds(g_file.buffer + sizeof(Elf64_Ehdr))
		&& is_64bits((Elf64_Ehdr *)g_file.buffer))
		parse_64bits();
	else
		file_format_not_recognized(argv, path);
}

int	main(int argc, char **argv)
{
	char	*path;

	if (argc == 2)
		path = argv[1];
	else
		path = "a.out";
	if (read_file(path))
	{
		printf("%s: '%s': No such file\n", argv[0], path);
		return (1);
	}

	if (!out_of_bounds(g_file.buffer + EI_NIDENT)
		&& is_elf((Elf32_Ehdr *)g_file.buffer))
		parse_elf(argv, path);
	else
		return file_format_not_recognized(argv, path);

	munmap(g_file.buffer, g_file.size);
	return (0);
}
