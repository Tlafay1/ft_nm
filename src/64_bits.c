/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   64_bits.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:20:42 by tlafay            #+#    #+#             */
/*   Updated: 2023/02/06 15:16:32 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	parse_64bits(char *buffer)
{
	Elf64_Shdr	*shdr;
	Elf64_Ehdr	*header;
	Elf64_Shdr	*symtab;
	Elf64_Shdr	*strtab;

	header = (Elf64_Ehdr *)buffer;
	shdr = (Elf64_Shdr *)((char *)buffer + header->e_shoff);
	char *section_names = (char *)(buffer + shdr[header->e_shstrndx].sh_offset);

	for (int i = 0; i < header->e_shnum; i++)
	{
		if (shdr[i].sh_size)
		{
			if (ft_strncmp(&section_names[shdr[i].sh_name], ".symtab", 7) == 0)
				symtab = (Elf64_Shdr *) &shdr[i];
			if (ft_strncmp(&section_names[shdr[i].sh_name], ".strtab", 7) == 0)
				strtab = (Elf64_Shdr *) &shdr[i];
		}
	}

	Elf64_Sym *sym = (Elf64_Sym *) (buffer + symtab->sh_offset);
	char *str = (char *) (buffer + strtab->sh_offset);

	for (size_t i = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++)
	{
		if (ft_strlen(str + sym[i].st_name) > 0)
		{
			printf("%016lx %s\n", sym[i].st_value, str + sym[i].st_name);
			printf("%c\n", print_type64(*sym, symtab));
		}
	}
}

char	print_type64(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char	c;

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
	{
		c = 'W';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
	{
		c = 'V';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
		&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
		&& shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
		&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
		&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else
		c = '?';
	if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}