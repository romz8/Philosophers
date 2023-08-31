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


#include "philo.h"

int main(int argc, char **argv)
{
	t_table table;
	pthread_mutex_t *forks;
	t_philo	*philo;

	if (check_arguments(argc, argv))
		return (1);
	table.total = ft_atoi(argv[1]);
	table.death_count = 0;
	table.time_die = ft_atoi(argv[2]);
	table.time_eat = ft_atoi(argv[3]);
	table.time_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		table.is_bounded = 1;
		table.meal_max= ft_atoi(argv[5]);
	}
	else
		table.is_bounded = 0;
	table.start_time = 0;
	forks = NULL;
	pthread_mutex_init(&table.mutex_table, NULL);
	init_forks(forks, &table);
	philo = init_philosophers(&table);
	thread_dinner(philo, &table);
	//printf("the death count is %i\n", table.death_count);
	//free(forks); - do a proper function carefull
	pthread_mutex_destroy(&table.mutex_table);
	return (0);
}

void	init_forks(pthread_mutex_t *forks, t_table *table)
{
	int i;
	
	forks = malloc(sizeof(pthread_mutex_t) * table->total);
	if (!forks)
	{	
		free(table);
		return ;
	}
	i = 0;
	while (i < table->total)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	table->forks = forks;
}

t_philo	*init_philosophers(t_table *table)
{
	int		i;
	t_philo	*philo;

	philo = malloc(sizeof(t_philo) * table->total);
	if (!philo)
	{
		clean_forks(table->forks, table->total);
		free(table);
		return (NULL);
	}
	i = -1;	
	while (++i < table->total)
	{
		philo[i].id = i + 1;
		philo[i].meal_count = 0; 
		philo[i].meal_max = table->meal_max; 
		philo[i].is_bounded = table->is_bounded; 
		philo[i].left_fork = &table->forks[i];
		philo[i].right_fork = &table->forks[(i + 1) % table->total];
		philo[i].table = table;
	}
	return (philo);
}

void	thread_dinner(t_philo *philo, t_table *table)
{
	int i;
	//pthread_t	*table_philo;
	//table_philo = malloc(sizeof(pthread_t) * table->total);
	i = 0;
	while (i < table->total)
	{
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		i++;
	}
	pthread_mutex_lock(&philo->table->mutex_table);
	table->start_time = timestamp_ms(); //to macke sure time doesn't start while everybody is seated
	pthread_mutex_unlock(&philo->table->mutex_table);
	i = 0;
	while (i < table->total)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}