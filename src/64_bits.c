/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   64_bits.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:20:42 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/11 12:53:38 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"


/*
	Default display function, to print the output
*/

void	print_sym64(void *content)
{
	t_output	*output;

	output = (t_output *)content;
	if (output->value)
		printf("%016lx %c %s\n", output->value,
			output->type, output->name);
	else
		printf("                 %c %s\n", output->type,
			output->name);
}



void	parse_64bits(t_file	file)
{
	Elf64_Shdr	*sections;
	Elf64_Ehdr	*header;
	t_list		*head;

	head = NULL;
	header = (Elf64_Ehdr *)file.buffer;
	sections = (Elf64_Shdr *)((char *)file.buffer + header->e_shoff);

	for (int i = 0; i < header->e_shnum; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			Elf64_Sym *symtab = (Elf64_Sym *)(file.buffer + sections[i].sh_offset);
			int symbol_num = sections[i].sh_size / sections[i].sh_entsize;
			char *symbol_names = (char *)(file.buffer + sections[sections[i].sh_link].sh_offset);
			for (int j = 0; j < symbol_num; j++)
			{
				add_section(&head, symtab[j].st_value,
					get_type64(symtab[j], sections), symbol_names + symtab[j].st_name);
			}
		}
	}
	ft_lstiter(head, print_sym64);
}

char	get_type64(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char	c;

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_TYPE(sym.st_info) == STT_SECTION)
		c = 0;
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
	else if ((shdr[sym.st_shndx].sh_type == SHT_PROGBITS
			|| shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY
			|| shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY
			|| shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY)
		&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
		&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else
	{
		c = '?';
		printf("%d %d %ld\n", sym.st_shndx, shdr[sym.st_shndx].sh_type, shdr[sym.st_shndx].sh_flags);
	}
	if (c && ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}