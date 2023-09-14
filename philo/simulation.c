/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:33:00 by rjobert           #+#    #+#             */
/*   Updated: 2023/09/11 11:33:03 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
we "activate" the thread in each philo object and start its philo routine
1. we pass the object itself as an argument
2. we iniate the start time in the table object using a mutex to be sure.
you can see in the philo_routine taht no trhead starts while the start_time is
set up.
we join all the threads to wait for their execution later on 
in the simulation_monitor)
*/
int	start_simulation(t_philo *philo, t_table *table)
{
	int	i;

	i = 0;
	table->start_time = get_time_ms();
	if (table->total == 1)
	{
		if (pthread_create(&philo[i].thread, NULL, philone_routine, &philo[i]))
			return (1);
	}
	else
	{
		while (i < table->total)
		{
			if (pthread_create(&philo[i].thread, NULL, philo_routine, \
			&philo[i]))
				return (1);
			i++;
		}
	}
	return (0);
}

/*
check that the simulation should continue based on conditions (philos are alive)
or reached maximum number of meals - return 0 if should stop, otherwise 1
1. traverse the array of philo objects
2. lock the object and extract the expected death_time and if philo finished
3. if current time overpass death_time and he/she is not eating or is
not finished -> kill the philosopher (or declare dead, as you wish) 
and stop the simulation (return 0)
4. if the total of people who finished eating is the number of philos, stop
the simulation 
*/
int	simulation_continue(t_table *table, int total_finished, int i)
{
	long long	death_time;
	int			is_finished;

	while (i < table->total)
	{
		pthread_mutex_lock(&table->philos[i].philo_lock);
		death_time = table->philos[i].death_time;
		is_finished = table->philos[i].is_finished;
		pthread_mutex_unlock(&table->philos[i].philo_lock);
		if (get_time_ms() >= death_time && death_time \
		&& !table->philos[i].is_eating && !is_finished)
		{
			message(&table->philos[i], DIED);
			return (0);
		}
		total_finished += is_finished;
		if (total_finished == table->total)
		{
			reach_end_condition(table);
			return (0);
		}
		i++;
	}
	return (1);
}

/*
The routine associated to the simulation monitor: always return NULL.
1. take void pointer as input, convert it back to t_table type
2. constantly run a check if the simulation should stop or not - usleep
to avoid too much pressure on CPU - and exit as soon as simulation 
shoud stop
*/
void	*simulation_routine(void *input)
{
	t_table	*table;

	table = (t_table *) input;
	while (1)
	{
		if (!simulation_continue(table, 0, 0))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}

/*
monitoring if the simuluation should stop and close all the thread
accordingly if so
1. Launch the monitor and wait for its execution with pthread_join()
2. Once done -> join all the thread (as the simulation stop that
tantamounts to closing them all)
*/
int	simulation_monitor(t_table *table)
{
	int		i;
	t_philo	*philos;

	philos = table->philos;
	i = 0;
	if (pthread_create(&table->simul_thread, NULL, \
	simulation_routine, (void *) table))
		return (1);
	pthread_join(table->simul_thread, NULL);
	while (i < table->total)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	return (0);
}

/*
This is used in the philo threads at the local level to check if dinner is over
- take the table object as input and return 1 if simulation condition indicates
we should continue, else return 0
- objective : used in the simulation thead to check on if philosophers or 
have eaten enough 
- get the flag(is_over) in the simulation table object with "flash access" mutex 
and then store : to local variable - act like a quick transitor 
with fast mutex_protection
*/
int	simulation_conditions(t_table *table)
{
	int	over;

	pthread_mutex_lock(&table->death_lock);
	over = table->is_over;
	pthread_mutex_unlock(&table->death_lock);
	if (over == 1)
		return (0);
	return (1);
}
