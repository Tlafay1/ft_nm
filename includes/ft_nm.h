/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 05:38:19 by timothee          #+#    #+#             */
/*   Updated: 2023/02/06 14:32:24 by tlafay           ###   ########.fr       */
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

typedef struct s_file
{
	int		size;
	char	*buffer;
}	t_file;

void	parse_64bits(char *buffer);
void	parse_32bits(char *buffer);
char	print_type64(Elf64_Sym sym, Elf64_Shdr *shdr);
char	print_type32(Elf32_Sym sym, Elf32_Shdr *shdr);

#endif