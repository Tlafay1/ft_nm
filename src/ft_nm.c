/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timothee <timothee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:28 by tlafay            #+#    #+#             */
/*   Updated: 2023/02/03 03:40:09 by timothee         ###   ########.fr       */
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

int	is_64bits(Elf64_Ehdr *header)
{
	return (header->e_ident[EI_CLASS] == ELFCLASS64);
}

int	is_32bits(Elf32_Ehdr *header)
{
	return (header->e_ident[EI_CLASS] == ELFCLASS32);
}

int	main(int argc, char **argv)
{
	char		*path;
	t_file		file;
	Elf64_Ehdr	*header;
	Elf64_Shdr	*symtab;
	Elf64_Shdr	*strtab;

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
	if (ft_memcmp(header->e_ident, ELFMAG, SELFMAG))
		return file_format_not_recognized(argv, path);

	printf("%d\n", is_32bits((Elf32_Ehdr *)file.buffer));

	Elf64_Shdr *sections = (Elf64_Shdr *)((char *)file.buffer + header->e_shoff);
	char *section_names = (char *)(file.buffer + sections[header->e_shstrndx].sh_offset);

	for (int i = 0; i < header->e_shnum; i++)
	{
		if (sections[i].sh_size)
		{
			if (ft_strncmp(&section_names[sections[i].sh_name], ".symtab", 7) == 0)
				symtab = (Elf64_Shdr *) &sections[i];
			if (ft_strncmp(&section_names[sections[i].sh_name], ".strtab", 7) == 0)
				strtab = (Elf64_Shdr *) &sections[i];
		}
	}

	Elf64_Sym *sym = (Elf64_Sym *) (file.buffer + symtab->sh_offset);
	char *str = (char *) (file.buffer + strtab->sh_offset);

	for (size_t i = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++)
	{
		printf("%016lx %s\n", sym[i].st_value, str + sym[i].st_name);
	}

	munmap((void *)file.buffer, file.size);
	return (0);
}
