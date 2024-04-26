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
	int section_count = 0;

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
				section_count++;
				// print_sym_debug_type64(symtab[j], sections, symbol_names + symtab[j].st_name);
			}
		}
	}

	if (!section_count)
		return 1;

	ft_lstiter(head, print_sym64);
	ft_lstclear(&head, free);

	return 0;
}

__attribute__((warning("This function is for debugging only"))) void print_sym_debug_type64(Elf64_Sym sym, Elf64_Shdr *shdr, char *name)
{
	// if (!out_of_bounds((char *)(g_file.buffer + shdr[sym.st_shndx].sh_offset + sym.st_name)))
	printf("Symbol name: %s\n", name);
	printf("Symbol value: %lx\n", sym.st_value);
	printf("Symbol size: %lx\n", sym.st_size);
	printf("Symbol type: %d\n", ELF64_ST_TYPE(sym.st_info));
	printf("Symbol bind: %d\n", ELF64_ST_BIND(sym.st_info));
	printf("Symbol visibility: %d\n", ELF64_ST_VISIBILITY(sym.st_other));
	printf("Symbol section index: %d\n", sym.st_shndx);

	if (ELF64_ST_TYPE(sym.st_info) == STT_NOTYPE)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_NOTYPE\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_OBJECT\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_FUNC)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_FUNC\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_SECTION)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_SECTION\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_FILE)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_FILE\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_COMMON)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_COMMON\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_TLS)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_TLS\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_GNU_IFUNC)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_GNU_IFUNC\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_LOOS)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_LOOS\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_HIOS)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_HIOS\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_LOPROC)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_LOPROC\n");
	else if (ELF64_ST_TYPE(sym.st_info) == STT_HIPROC)
		printf("ELF64_ST_TYPE(sym.st_info) == STT_HIPROC\n");
	else
		printf("ELF64_ST_TYPE(sym.st_info) == %d\n", ELF64_ST_TYPE(sym.st_info));

	if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL)
		printf("ELF64_ST_BIND(sym.st_info) == STB_LOCAL\n");
	else if (ELF64_ST_BIND(sym.st_info) == STB_GLOBAL)
		printf("ELF64_ST_BIND(sym.st_info) == STB_GLOBAL\n");
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
		printf("ELF64_ST_BIND(sym.st_info) == STB_WEAK\n");
	else if (ELF64_ST_BIND(sym.st_info) == STB_NUM)
		printf("ELF64_ST_BIND(sym.st_info) == STB_NUM\n");
	else if (ELF64_ST_BIND(sym.st_info) == STB_LOOS)
		printf("ELF64_ST_BIND(sym.st_info) == STB_LOOS\n");
	else if (ELF64_ST_BIND(sym.st_info) == STB_HIOS)
		printf("ELF64_ST_BIND(sym.st_info) == STB_HIOS\n");
	else if (ELF64_ST_BIND(sym.st_info) == STB_LOPROC)
		printf("ELF64_ST_BIND(sym.st_info) == STB_LOPROC\n");
	else if (ELF64_ST_BIND(sym.st_info) == STB_HIPROC)
		printf("ELF64_ST_BIND(sym.st_info) == STB_HIPROC\n");
	else
		printf("ELF64_ST_BIND(sym.st_info) == %d\n", ELF64_ST_BIND(sym.st_info));

	if (ELF64_ST_VISIBILITY(sym.st_other) == STV_DEFAULT)
		printf("ELF64_ST_VISIBILITY(sym.st_other) == STV_DEFAULT\n");
	else if (ELF64_ST_VISIBILITY(sym.st_other) == STV_INTERNAL)
		printf("ELF64_ST_VISIBILITY(sym.st_other) == STV_INTERNAL\n");
	else if (ELF64_ST_VISIBILITY(sym.st_other) == STV_HIDDEN)
		printf("ELF64_ST_VISIBILITY(sym.st_other) == STV_HIDDEN\n");
	else if (ELF64_ST_VISIBILITY(sym.st_other) == STV_PROTECTED)
		printf("ELF64_ST_VISIBILITY(sym.st_other) == STV_PROTECTED\n");
	else
		printf("ELF64_ST_VISIBILITY(sym.st_other) == %d\n", ELF64_ST_VISIBILITY(sym.st_other));

	if (sym.st_shndx == SHN_UNDEF)
		printf("sym.st_shndx == SHN_UNDEF\n");
	else if (sym.st_shndx == SHN_LORESERVE)
		printf("sym.st_shndx == SHN_LORESERVE\n");
	else if (sym.st_shndx == SHN_LOPROC)
		printf("sym.st_shndx == SHN_LOPROC\n");
	else if (sym.st_shndx == SHN_BEFORE)
		printf("sym.st_shndx == SHN_BEFORE\n");
	else if (sym.st_shndx == SHN_AFTER)
		printf("sym.st_shndx == SHN_AFTER\n");
	else if (sym.st_shndx == SHN_HIPROC)
		printf("sym.st_shndx == SHN_HIPROC\n");
	else if (sym.st_shndx == SHN_ABS)
		printf("sym.st_shndx == SHN_ABS\n");
	else if (sym.st_shndx == SHN_COMMON)
		printf("sym.st_shndx == SHN_COMMON\n");
	else if (sym.st_shndx == SHN_HIRESERVE)
		printf("sym.st_shndx == SHN_HIRESERVE\n");
	else
		printf("sym.st_shndx == %d\n", sym.st_shndx);

	if (!out_of_bounds((void *)shdr + sym.st_shndx * sizeof(Elf64_Shdr)))
	{
		if (shdr[sym.st_shndx].sh_type == SHT_NULL)
			printf("shdr[sym.st_shndx].sh_type == SHT_NULL\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS)
			printf("shdr[sym_shndx].sh_type == SHT_PROGBITS\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_SYMTAB)
			printf("shdr[sym_shndx].sh_type == SHT_SYMTAB\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_STRTAB)
			printf("shdr[sym_shndx].sh_type == SHT_STRTAB\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_RELA)
			printf("shdr[sym_shndx].sh_type == SHT_RELA\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_HASH)
			printf("shdr[sym_shndx].sh_type == SHT_HASH\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
			printf("shdr[sym_shndx].sh_type == SHT_DYNAMIC\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_NOTE)
			printf("shdr[sym_shndx].sh_type == SHT_NOTE\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS)
			printf("shdr[sym_shndx].sh_type == SHT_NOBITS\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_REL)
			printf("shdr[sym_shndx].sh_type == SHT_REL\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_SHLIB)
			printf("shdr[sym_shndx].sh_type == SHT_SHLIB\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_DYNSYM)
			printf("shdr[sym_shndx].sh_type == SHT_DYNSYM\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY)
			printf("shdr[sym_shndx].sh_type == SHT_INIT_ARRAY\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY)
			printf("shdr[sym_shndx].sh_type == SHT_FINI_ARRAY\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_PREINIT_ARRAY)
			printf("shdr[sym_shndx].sh_type == SHT_PREINIT_ARRAY\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_GROUP)
			printf("shdr[sym_shndx].sh_type == SHT_GROUP\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_SYMTAB_SHNDX)
			printf("shdr[sym_shndx].sh_type == SHT_SYMTAB_SHNDX\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_LOOS)
			printf("shdr[sym_shndx].sh_type == SHT_LOOS\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_HIOS)
			printf("shdr[sym_shndx].sh_type == SHT_HIOS\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_LOPROC)
			printf("shdr[sym_shndx].sh_type == SHT_LOPROC\n");
		else if (shdr[sym.st_shndx].sh_type == SHT_HIPROC)
			printf("shdr[sym_shndx].sh_type == SHT_HIPROC\n");
		else
			printf("shdr[sym_shndx].sh_type == %d\n", shdr[sym.st_shndx].sh_type);

		if (shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR)
			printf("shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_ALLOC)
			printf("shdr[sym.st_shndx].sh_flags & SHF_ALLOC\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_WRITE)
			printf("shdr[sym.st_shndx].sh_flags & SHF_WRITE\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_MASKPROC)
			printf("shdr[sym.st_shndx].sh_flags & SHF_MASKPROC\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_MERGE)
			printf("shdr[sym.st_shndx].sh_flags & SHF_MERGE\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_STRINGS)
			printf("shdr[sym.st_shndx].sh_flags & SHF_STRINGS\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_INFO_LINK)
			printf("shdr[sym.st_shndx].sh_flags & SHF_INFO_LINK\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_LINK_ORDER)
			printf("shdr[sym.st_shndx].sh_flags & SHF_LINK_ORDER\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_OS_NONCONFORMING)
			printf("shdr[sym.st_shndx].sh_flags & SHF_OS_NONCONFORMING\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_GROUP)
			printf("shdr[sym.st_shndx].sh_flags & SHF_GROUP\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_TLS)
			printf("shdr[sym.st_shndx].sh_flags & SHF_TLS\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_COMPRESSED)
			printf("shdr[sym.st_shndx].sh_flags & SHF_COMPRESSED\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_MASKOS)
			printf("shdr[sym.st_shndx].sh_flags & SHF_MASKOS\n");
		if (shdr[sym.st_shndx].sh_flags & SHF_MASKPROC)
			printf("shdr[sym.st_shndx].sh_flags & SHF_MASKPROC\n");
	}
	printf("\n");
}

char get_type64(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char c;

	if (g_options.undefined_only && sym.st_shndx != SHN_UNDEF)
		return 0;

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
		else if ((shdr[sym.st_shndx].sh_type == SHT_PROGBITS || shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY || shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY || shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY) && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
			c = 'D';
		else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
			c = 'D';
		else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR)
			c = 'T';
		else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags & SHF_ALLOC)
			c = 'R';
		else
			c = '?';
	}
	else
		c = '?';
	if (c && ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c = ft_tolower(c);
	return c;
}