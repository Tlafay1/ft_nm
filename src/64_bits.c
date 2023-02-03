/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   64_bits.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:20:42 by tlafay            #+#    #+#             */
/*   Updated: 2023/02/03 17:33:14 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	parse_64bits(char *buffer)
{
	Elf64_Shdr	*sections;
	Elf64_Ehdr	*header;
	Elf64_Shdr	*symtab;
	Elf64_Shdr	*strtab;

	header = (Elf64_Ehdr *)buffer;
	sections = (Elf64_Shdr *)((char *)buffer + header->e_shoff);
	char *section_names = (char *)(buffer + sections[header->e_shstrndx].sh_offset);

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

	Elf64_Sym *sym = (Elf64_Sym *) (buffer + symtab->sh_offset);
	char *str = (char *) (buffer + strtab->sh_offset);

	for (size_t i = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++)
	{
		printf("%016lx %s\n", sym[i].st_value, str + sym[i].st_name);
	}
}