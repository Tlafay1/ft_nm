/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 16:20:43 by tlafay            #+#    #+#             */
/*   Updated: 2023/02/28 17:59:33 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	nm_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (*s1 && !ft_isalpha(*s1))
	{
		i++;
		s1++;
	}
	while (*s2 && !ft_isalpha(*s2))
		s2++;
	while (*s1 != '\0' && (ft_tolower(*s1) == ft_tolower(*s2)) && i + 1 < ft_strlen(s1))
	{
		s1++;
		s2++;
		i++;
	}
	return ((unsigned char)ft_tolower(*s1) - (unsigned char)ft_tolower(*s2));
}

void	add_section(t_list **head, long unsigned int value, char type, char *name)
{
	t_list		*node;
	t_list		*tmp;
	t_output	*output;
	t_output	*tmp_output;

	output = (t_output *)malloc(sizeof(t_output));
	output->value = value;
	output->type = type;
	output->name = name;
	node = ft_lstnew((void *)output);
	if (!*head)
	{
		*head = node;
		return ;
	}
	tmp = *(head);
	tmp_output = (t_output *)tmp->content;
	if (nm_strcmp(output->name, tmp_output->name) > 0)
	{
		ft_lstadd_front(head, node);
		return ;
	}
	while (tmp)
	{
		tmp_output = (t_output *)tmp->content;
		if (nm_strcmp(output->name, tmp_output->name) > 0)
		{
			// printf("%s > %s\n", output->name, tmp_output->name);
			if (tmp->next)
				node->next = tmp->next;
			tmp->next = node;
			return ;

		}
		tmp = tmp->next;
	}
}