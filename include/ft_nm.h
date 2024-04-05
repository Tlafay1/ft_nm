/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 05:38:19 by timothee          #+#    #+#             */
/*   Updated: 2023/10/31 13:49:25 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <elf.h>
# include <stdio.h>

# include "libft.h"
# include "argparse.h"

typedef struct s_nm_options
{
	int debug_syms;
	int	extern_only;
	int	no_sort;
	int	reverse_sort;
	int	undefined_only;
}	t_nm_options;

typedef struct s_file
{
	void	*buffer;
	void	*end;
	int		size;
}	t_file;

typedef struct s_output
{
	long unsigned int	value;
	unsigned char		type;
	uint16_t			st_shndx;
	char				*name;
}	t_output;

// Useful in many places
extern t_file		g_file;
extern t_nm_options	g_options;

int		parse_64bits();
int		parse_32bits();
char	get_type64(Elf64_Sym sym, Elf64_Shdr *shdr);
char	get_type32(Elf32_Sym sym, Elf32_Shdr *shdr);
int		add_section(t_list **head, long unsigned int value,
			char type, char *name, uint16_t st_shndx);
int 	out_of_bounds(void *ptr);
int		nm_error(const char *prog_name, const char *path, char *msg);
int		syscall_error(const char *prog_name, const char *path);
int		check_filetype(mode_t m, const char *prog_name, const char *path);
#endif