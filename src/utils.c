/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 16:20:43 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/13 14:44:53 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	nm_strcmp(t_list *n1, t_list *n2)
{
	char		*s1;
	char		*s2;
	t_output	*tmp;

	tmp = n1->content;
	s1 = tmp->name;
	tmp = n2->content;
	s2 = tmp->name;
	while (*s1 && !ft_isalnum(*s1))
		s1++;
	while (*s2 && !ft_isalnum(*s2))
		s2++;
	while (*s1 && *s2 && (ft_tolower(*s1) == ft_tolower(*s2)))
	{
		s1++;
		s2++;
		while (*s1 && !ft_isalnum(*s1))
			s1++;
		while (*s2 && !ft_isalnum(*s2))
			s2++;
	}
	return ((unsigned char)ft_tolower(*s1) - (unsigned char)ft_tolower(*s2));
}

int nm_strcmp_reverse(t_list *n1, t_list *n2)
{
	return -nm_strcmp(n1, n2);
}

int	add_section(t_list **head, long unsigned int value, char type, char *name, uint16_t st_shndx)
{
	t_list		*node;
	t_output	*output;

	output = (t_output *)malloc(sizeof(t_output));
	if (!output)
		return -1;
	output->value = value;
	output->type = type;
	output->name = name;
	output->st_shndx = st_shndx;
	if (!output->type || output->type == 'a' || output->type == 'u')
		return 1;
	node = ft_lstnew((void *)output);
	if (!node)
		return -1;
	if (g_options.no_sort)
		ft_lstadd_back(head, node);
	else if (g_options.reverse_sort)
		ft_lstadd_sorted(head, node, nm_strcmp_reverse);
	else
		ft_lstadd_sorted(head, node, nm_strcmp);
	return 0;
}

int out_of_bounds(void *ptr)
{
	return ptr > (void *)g_file.end;
}
