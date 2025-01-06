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

int nm_strcmp(t_list *n1, t_list *n2)
{
	t_output *tmp = n1->content;
	char *s1 = tmp->name;
	tmp = n2->content;
	char *s2 = tmp->name;
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

int add_section(t_list **head, long unsigned int value, char type, char *name, uint16_t st_shndx)
{
	if (!type || type == 'a' || type == 'u')
		return 1;
	t_output *output = malloc(sizeof(t_output));
	if (!output)
		return -1;
	output->value = value;
	output->type = type;
	output->name = name;
	output->st_shndx = st_shndx;
	t_list *node = ft_lstnew(output);
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

/**
 * @brief Check that the pointer is not out of bounds and won't cause a segfault.
 *
 * @param ptr The pointer to check.
 * @return A boolean indicating if the pointer is out of bounds.
 */
int out_of_bounds(const void *ptr)
{
	return ptr > g_file.end;
}
