/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:37:18 by fses              #+#    #+#             */
/*   Updated: 2025/09/08 16:55:34 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

void	*g_collecter(t_garbage **garb, void *adress, int l_flag)
{
	t_garbage	*root;
	t_garbage	*iter;
	t_garbage	*new_node;

	if (adress == NULL)
		return (NULL);
	new_node = malloc(sizeof(t_garbage));
	if (!new_node)
		return (NULL);
	new_node->adress = adress;
	new_node->line = l_flag;
	new_node->is_env = 0;
	new_node->next = NULL;
	root = (*garb);
	if (!root)
	{
		*garb = new_node;
		return (adress);
	}
	iter = root;
	while (iter->next)
		iter = iter->next;
	iter->next = new_node;
	return (adress);
}

void	*g_collecter_env(t_garbage **garb, void *adress)
{
	t_garbage	*root;
	t_garbage	*iter;
	t_garbage	*new_node;

	if (adress == NULL)
		return (NULL);
	new_node = malloc(sizeof(t_garbage));
	if (!new_node)
		return (NULL);
	new_node->adress = adress;
	new_node->line = 0;
	new_node->is_env = 1;
	new_node->next = NULL;
	root = (*garb);
	if (!root)
	{
		*garb = new_node;
		return (adress);
	}
	iter = root;
	while (iter->next)
		iter = iter->next;
	iter->next = new_node;
	return (adress);
}

void	g_free_l(t_garbage **garb)
{
	t_garbage	*root;
	t_garbage	*iter;
	void		*addr;

	if (!garb || !*garb)
		return ;
	root = (*garb);
	iter = root;
	while (iter)
	{
		if (iter->adress != NULL && !iter->is_env)
		{
			addr = iter->adress;
			iter->adress = NULL;
			free(addr);
		}
		iter = iter->next;
	}
}

void	g_free(t_garbage **garb)
{
	t_garbage	*root;
	t_garbage	*tmp;

	if (!garb || !*garb)
		return ;
	root = (*garb);
	while (root)
	{
		if (root->adress != NULL)
		{
			free(root->adress);
			root->adress = NULL;
		}
		tmp = root;
		root = root->next;
		free(tmp);
		tmp = NULL;
	}
	*garb = NULL;
}
