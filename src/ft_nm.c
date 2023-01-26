/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timothee <timothee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:28 by tlafay            #+#    #+#             */
/*   Updated: 2023/01/25 09:07:10 by timothee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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

int	file_format_not_recognized(char **argv, char *path)
{
	printf("%s: '%s': file format not recognized\n", argv[0], path);
	return (1);
}

void	write_header(char *header, t_file file)
{
	for (int i = 0; i < file.size; i++, header++)
		*header = (int)file.buffer[i];
}

int	main(int argc, char **argv)
{
	char		*path;
	t_file		file;
	Elf64_Ehdr *header;

	if (argc == 2)
		path = argv[1];
	else
		path = "a.out";
	if (read_file(path, &file))
	{
		printf("%s: '%s': No such file\n", argv[0], path);
		return (1);
	}

	header = (Elf64_Ehdr *)file.buffer;
	if (ft_memcmp(header->e_ident, ELFMAG, SELFMAG) || header->e_ident[EI_CLASS] != ELFCLASS64)
		return file_format_not_recognized(argv, path);
	munmap((void *)file.buffer, file.size);

	return (0);
}
