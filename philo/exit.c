/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:40:34 by rjobert           #+#    #+#             */
/*   Updated: 2023/09/11 11:40:36 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_memory(t_table *table)
{
	if (!table)
		return ;
	if (table->forks)
		free(table->forks);
	if (table->philos)
		free(table->philos);
	return ;
}

void	clean_mutex(t_table *table)
{
	int	i;

	if (!table)
		return ;
	if (table->forks)
	{
		i = -1;
		while (++i < table->total)
			pthread_mutex_destroy(&table->forks[i]);
	}
	if (table->philos)
	{
		i = -1;
		while (++i < table->total)
			pthread_mutex_destroy(&table->philos[++i].philo_lock);
	}
	pthread_mutex_destroy(&table->time_lock);
	pthread_mutex_destroy(&table->death_lock);
	pthread_mutex_destroy(&table->write_lock);
	return ;
}

int	exit_free_all(t_table *table, int EXIT_CODE)
{
	clean_mutex(table);
	free_memory(table);
	return (EXIT_CODE);
}

int	exit_bad(t_table *table, int EXIT_CODE)
{
	free_memory(table);
	return (EXIT_CODE);
}
