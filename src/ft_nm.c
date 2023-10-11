/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:28 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/11 12:48:56 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"


/*
	Read the file specified in argument.
	Returns a structure containing the size
	of the file and the pointer to the buffer. 
*/

int read_file(char *path, t_file *file)
{
	int			fd;
	struct stat	buf;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -1;
	if (fstat(fd, &buf) == -1)
		return -1;
	file->size = buf.st_size;
	file->buffer = (char *)mmap(0, file->size, PROT_READ, MAP_PRIVATE, fd, 0);
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

void	parse_elf(t_file file, char **argv, char *path)
{
	if (is_32bits((Elf32_Ehdr *)file.buffer))
		parse_32bits(file.buffer);
	else if (is_64bits((Elf64_Ehdr *)file.buffer))
		parse_64bits(file);
	else
		file_format_not_recognized(argv, path);
}

int	main(int argc, char **argv)
{
	char		*path;
	t_file		file;

	if (argc == 2)
		path = argv[1];
	else
		path = "a.out";
	if (read_file(path, &file))
	{
		printf("%s: '%s': No such file\n", argv[0], path);
		return (1);
	}

	if (is_elf((Elf32_Ehdr *)file.buffer))
		parse_elf(file, argv, path);
	else
		return file_format_not_recognized(argv, path);

	munmap((void *)file.buffer, file.size);
	return (0);
}
