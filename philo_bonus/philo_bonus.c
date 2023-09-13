/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 14:50:15 by rjobert           #+#    #+#             */
/*   Updated: 2023/08/30 14:50:18 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* 
1. We declare a table and philo data structure then check arguments are 
correct
2. we fill the table data from arguments, special case if user entered a 
max nber of meal
3. we init the mutex of the table object(that will be use when a thread modify
 a table
data like deat_count and start time)
4. we init the forks data struct (an array of N forks, all mutexes)
5. we init philosopers (see below)
6. destroy the Mutexes and free Data struct
*/
int	main(int argc, char **argv)
{
	t_table	table;

	if (check_arguments(argc, argv, 0, 0))
		return (1);
	if (init_table(&table, argv))
		ft_exit(&table, EXIT_FAILURE);
	if (init_philosophers(&table))
		ft_exit(&table, EXIT_FAILURE);
	start_simulation(&table);
	simulation_monitor(&table);
	sem_wait(table.sem_stop);
	clear_programme(&table);
	exit(EXIT_SUCCESS);
}

int	init_table(t_table *table, char **argv)
{
	table->total = ft_atoi(argv[1]);
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
	table->philo_pids = malloc(sizeof(pid_t) * table->total);
	if (!table->philo_pids)
		ft_exit(table, EXIT_FAILURE);
	table->sem_forks = safe_sem_init(SEM_FORKS, table->total);
	table->sem_write = safe_sem_init(SEM_WRITE, 1);
	table->sem_stop = safe_sem_init(SEM_STOP, 0);
	table->sem_names = process_sem(table);
	if (!table->sem_names)
		ft_exit(table, EXIT_FAILURE);
	return (0);
}

char	**process_sem(t_table *table)
{
	char	**names;
	int		i;

	names = malloc(sizeof(char *) * table->total);
	if (!names)
		return (NULL);
	i = 0;
	while (i < table->total)
	{
		names[i] = custom_sem_philo(i + 1);
		i++;
	}
	return (names);
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
int	init_philosophers(t_table *table)
{
	int		i;
	t_philo	*philo;

	philo = malloc(sizeof(t_philo) * table->total);
	if (!philo)
		return (1);
	i = -1;
	while (++i < table->total)
	{
		philo[i].id = i + 1;
		philo[i].meal_count = 0;
		philo[i].is_eating = 0;
		philo[i].is_dead = 0;
		philo[i].table = table;
		philo[i].lock = safe_sem_init(table->sem_names[i], 1);
	}
	table->philos = philo;
	return (0);
}
