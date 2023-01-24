/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:28 by tlafay            #+#    #+#             */
/*   Updated: 2023/01/24 18:10:05 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "elf.h"

#include <stdio.h>
#include <string.h>

typedef struct s_file
{
	int		size;
	char	*buffer;
}	t_file;

int read_file(char *path, t_file *file)
{
	int			fd;
	struct stat	buf;
	char		*ret;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -1;
	if (fstat(fd, &buf) == -1)
		return -1;
	file->size = buf.st_size;
	file->buffer = (char *)mmap(0, file->size, PROT_READ, MAP_PRIVATE, fd, 0);
	return 0;
}

void	write_header(char *header, t_file file)
{
	for (int i = 0; i < file.size; i++, header++)
		*header = (int)file.buffer[i];
}

int	main(int argc, char **argv, char **envp)
{
	char	*path;
	t_file	file;
	Elf64_Ehdr *header;

	if (argc == 2)
		path = argv[1];
	else
		path = "a.out";
	if (read_file(path, &file))
	{
		write(2, "Error\n", 6);
		return (1);
	}

	header = (Elf64_Ehdr *)file.buffer;
	if (memcmp(header->e_ident, ELFMAG, SELFMAG) == 0)
		printf("Valid\n");
	else
		printf("Invalid\n");
	return (0);
}
