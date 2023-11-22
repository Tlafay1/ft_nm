/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   64_bits.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:20:42 by tlafay            #+#    #+#             */
/*   Updated: 2023/11/02 10:38:29 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
	Default display function, to print the output
*/

void print_sym64(void *content)
{
	t_output *output;

	output = (t_output *)content;
	if (output->st_shndx != SHN_UNDEF)
		// *.s represents the maximum size of the string.
		printf("%016lx %c %.*s\n", output->value, output->type,
			   (int)(g_file.end - (void *)output->name), output->name);
	else
		printf("%16c %c %s\n", ' ', output->type,
			   output->name);
}

int parse_64bits()
{
	Elf64_Shdr *sections;
	Elf64_Ehdr *header;
	t_list *head;

	head = NULL;
	header = (Elf64_Ehdr *)g_file.buffer;

	// Need to cast in (void *) to make sure it increments byte after byte
	if (out_of_bounds((void *)header + sizeof(Elf64_Ehdr)))
		return 2;

	sections = (Elf64_Shdr *)((void *)g_file.buffer + header->e_shoff);

	for (int i = 0; i < header->e_shnum; i++)
	{
		// Checking if we will go out of the file's limits in this loop iteration
		if (out_of_bounds((void *)sections + sizeof(Elf64_Shdr)))
			break;

		// Contains a symbol table, that's where we get all the infos we want
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			// Points to the first byte of the section (see sh_offset)
			Elf64_Sym *symtab = (Elf64_Sym *)(g_file.buffer + sections[i].sh_offset);

			if (out_of_bounds((void *)symtab + sizeof(Elf64_Sym)))
				break;

			// This shouldn't be relevant for symtabs
			// because it should always be 24, but just in case
			if (sections[i].sh_entsize == 0)
				continue;

			// Getting the number of symbols we need to iterate over
			int symbol_num = sections[i].sh_size / sections[i].sh_entsize;

			// No need to check the out of bounds here, we only use this variable
			// in add_section(), and a check is already implemented here.
			char *symbol_names = (char *)(g_file.buffer + sections[sections[i].sh_link].sh_offset);
			for (int j = 0; j < symbol_num; j++)
			{
				if (out_of_bounds((void *)symbol_names + symtab[j].st_name))
					break;

				add_section(&head, symtab[j].st_value,
							get_type64(symtab[j], sections),
							symbol_names + symtab[j].st_name, symtab[j].st_shndx);
			}
		}
	}

	if (!head)
		return 1;

	ft_lstiter(head, print_sym64);

	return 0;
}

char get_type64(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char c;

	if (g_options.undefined_only)
	{
		if (sym.st_shndx != SHN_UNDEF)
			return 0;
	}

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
	else if (!out_of_bounds((void *)shdr + sym.st_shndx * sizeof(Elf64_Shdr)))
	{
		if (shdr[sym.st_shndx].sh_type == SHT_NOBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
			c = 'B';
		else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == 50)
			c = 'R';
		else if ((shdr[sym.st_shndx].sh_type == SHT_PROGBITS || shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY || shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY || shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY) && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
			c = 'D';
		else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
			c = 'D';
		else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR && ELF64_ST_TYPE(sym.st_info) == STT_FUNC)
			c = 'T';
		else
			c = '?';
	}
	else
		c = '?';
	if (c && ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}