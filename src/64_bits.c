/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   64_bits.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:20:42 by tlafay            #+#    #+#             */
/*   Updated: 2023/02/06 16:15:18 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	parse_64bits(char *buffer)
{
	Elf64_Shdr	*sections;
	Elf64_Ehdr	*header;

	header = (Elf64_Ehdr *)buffer;
	sections = (Elf64_Shdr *)((char *)buffer + header->e_shoff);
	// char *section_names = (char *)(buffer + sections[header->e_shstrndx].sh_offset);

	for (int i = 0; i < header->e_shnum; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			Elf64_Sym *symtab = (Elf64_Sym *)(buffer + sections[i].sh_offset);
			int symbol_num = sections[i].sh_size / sections[i].sh_entsize;
			char *symbol_names = (char *)(buffer + sections[sections[i].sh_link].sh_offset);
			for (int j = 0; j < symbol_num; j++)
			{
				if (symtab[j].st_value)
					printf("%016lx %c %s\n", symtab[j].st_value,
						print_type64(symtab[j], sections), symbol_names + symtab[j].st_name);
				else
					printf("                 %c %s\n", print_type64(symtab[j], sections),
						symbol_names + symtab[j].st_name);
			}
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