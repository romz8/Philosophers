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

/* 
1. We declare a table and philo data structure then check arguments are correct
2. we fill the table data from arguments, special case if user entered a 
max nber of meal
3. we init the mutex of the table object(that will be use when a thread modify a table
data like deat_count and start time)
4. we init the forks data struct (an array of N forks, all mutexes)
5. we init philosopers (see below)
6. destroy the Mutexes and free Data struct
*/
int main(int argc, char **argv)
{
	t_table table;
	pthread_mutex_t *forks;

	if (check_arguments(argc, argv, 0, 0))
		return (1);
	init_table(&table, argv);
	forks = NULL;
	init_forks(forks, &table);
	table.philos = init_philosophers(&table);
	thread_dinner(table.philos, &table);
	simulation_monitor(&table);
	return (0);
}

/*
we "activate" the thread in each philo object and start its philo routine
1. we pass the object itself as an argument
2. we iniate the start time in the table object using a mutex to be sure.
you can see in the philo_routine taht no trhead starts while the start_time is
set up.
3. we join all the threads to wait for their execution.
*/
void	thread_dinner(t_philo *philo, t_table *table)
{
	int i;

	i = 0;
	table->start_time = get_time_ms();
	while (i < table->total)
	{
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		i++;
	}
	pthread_mutex_lock(&table->time_lock);
	table->start_time = get_time_ms();
	pthread_mutex_unlock(&table->time_lock);
}

/*
check that the simulation should continue based on conditions (philos are alive)
or reached maximum number of meals - return 0 if should stop, otherwise 1
1. traverse the array of philo objects
2. lock the object and extract the expected death_time
3. if we overpass it -> kill the philosopher (or declare it dead, as you wish) 
*/
int	simulation_continue(t_table *table)
{
	int	i;
	long long death_time;
	
	i = 0;
// 	while (get_time_ms() < table->start_time || !table->start_time)
// 		usleep(100);
	while (i < table->total)
	{
		pthread_mutex_lock(&table->philos[i].philo_lock);
		death_time = table->philos[i].death_time;
		pthread_mutex_unlock(&table->philos[i].philo_lock);
		if (get_time_ms() >= death_time && death_time)
		{
			message(&table->philos[i], DIED);
			return (0);
		}
		i++;
	}
	return (1);
}

/*
The routine associated to the simulation monitor: always return NULL.
1. take void pointer as input, convert it back to t_philo type
2. constantly run a check if the simulation should stop or not - usleep
to avoid too much pressure on CPU - and exit as soon as simulation 
shoud stop
*/
void	*simulation_routine(void *input)
{
	t_table *table;

	table = (t_table *) input;
	while(1)
	{
		if (!simulation_continue(table))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}

/*
monitoring if the simuluation should stop and close all the thread
accordingly if so
*/
void	simulation_monitor(t_table *table)
{
	int		i;
	t_philo *philos;

	philos = table->philos;
	i = 0;
	pthread_create(&table->simul_thread, NULL, simulation_routine, (void *) table);
	pthread_join(table->simul_thread, NULL);
	while (i < table->total)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	return ;
}