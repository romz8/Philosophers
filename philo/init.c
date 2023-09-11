/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 18:48:49 by rjobert           #+#    #+#             */
/*   Updated: 2023/09/07 18:48:52 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
MALLOC - THIS NEEDS TO BE FREED - CHECKED ?
1. we allocate N memory space of size N as nbr forks = nbr phillo
2. malloc protected
3. we init all the mutexes in the mallocated mutex array
4. we make the forks pointer in table DS point toward our array
*/
int	init_forks(pthread_mutex_t *forks, t_table *table)
{
	int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->total);
	if (!forks)
	{
		clean_mutex(table);
		free_memory(table);
		return (1);
	}
	i = 0;
	while (i < table->total)
	{
		if (pthread_mutex_init(&forks[i], NULL))
			return (1);
		i++;
	}
	table->forks = forks;
	return (0);
}

/*
we init our threads based on our nber of philosophers
1. receive table DS and from there take the nber of philo input
to allocated an array of philo struct - malloc protected and if 
issue free the table object.
2. fill the struct in the array for all philo : it's id, init nber of
meals, if it has a cap, and fill in the forks and table data.
3. the forks are filled as follwo : forks array goes from 0 to N -1 and 
philo id go from 1 to N : philo 1 has fork[0] o his left and fork[1] on 
his right, philo 2 has fork[1] and fork[2], ..., philo n has fork[n - 1] and
fork [0]. so we fill the philo of id [i + 1] with left_mutex pointer has fork[i]
and the left fork[i + 1] and make it modulo nber of philo so that philo [n] left
fork is [n % n] = 0.
4. we return the philo array
*/
t_philo	*init_philosophers(t_table *table)
{
	int		i;
	t_philo	*philo;

	philo = malloc(sizeof(t_philo) * table->total);
	if (!philo)
	{
		clean_mutex(table);
		free_memory(table);
		return (NULL);
	}
	i = -1;
	while (++i < table->total)
	{
		philo[i].id = i + 1;
		philo[i].meal_count = 0;
		philo[i].is_finished = 0;
		philo[i].left_fork = &table->forks[i];
		philo[i].right_fork = &table->forks[(i + 1) % table->total];
		philo[i].table = table;
		if (pthread_mutex_init(&philo[i].philo_lock, NULL))
			return (NULL);
	}
	return (philo);
}

int	init_table(t_table *table, char **argv)
{
	table->total = ft_atoi(argv[1]);
	table->death_count = 0;
	table->time_die = ft_atoi(argv[2]);
	table->time_eat = ft_atoi(argv[3]);
	table->time_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		table->is_bounded = 1;
		table->meal_max = ft_atoi(argv[5]);
	}
	else
		table->is_bounded = 0;
	table->start_time = 0;
	table->is_over = 0;
	if (pthread_mutex_init(&table->time_lock, NULL))
		return (1);
	if (pthread_mutex_init(&table->death_lock, NULL))
		return (1);
	if (pthread_mutex_init(&table->write_lock, NULL))
		return (1);
	return (0);
}
