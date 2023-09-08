/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_life.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 16:43:13 by rjobert           #+#    #+#             */
/*   Updated: 2023/08/30 16:43:17 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void   philo_think(t_philo *philo)
{
	message(philo, THINKING);
}

/*
Take the philo instance as an input, return nothing and  
run eat + sleep consecutives routines
1. use lock forsk mutex in left-right order to avoid data race
2. mutex the philosopher object to prevent eating / dying at same time
3. update philosopher xext death_time as per time_die in the table object
4. unlock philo mutex
5. put down forks (release the mutex) in the same order to avoid deadlock
6. put philo to sleep as per sleep-time in table object and with ms adjsutment
*/
void	philo_eat_sleep(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	message(philo, FORK);
	pthread_mutex_lock(philo->right_fork);
	message(philo, FORK);
	pthread_mutex_lock(&philo->philo_lock);
	message(philo, EATING);
	philo->is_eating = 1;
	philo->death_time = get_time_ms() + philo->table->time_die;
	philo->meal_count++;
	philo_sleep(philo->table->time_eat);
	philo->is_eating = 0;
	pthread_mutex_unlock(&philo->philo_lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	message(philo, SLEEPING);
	philo_sleep(philo->table->time_sleep);
}

/*
take a void pointer as input (as per pthread_create) that is converted
back to t_philo pnters. return a void pointer (as per pthread_create) and
run philo_routine as long as no death.
1. convert void * to t_philo * 
2. create randomness to avoid deadlock (all philo reach their left fork at 
the same time and wait each other) - so if even philo_id -> start sligthly later
3. init the first time to die for a philo as he haven't eaten yet
4. while the simulation condition are correct, the philosopher executes his
eat / sleep / think rountines
*/
void	*philo_routine(void *input)
{
	t_philo *philo;
	long long	init_time;
	
	philo = (t_philo *) input;
	if (philo->id % 2 == 0)
		philo_sleep(philo->table->time_eat / 10);
	init_time = get_simul_start(philo->table);
	while (get_time_ms() < init_time)
		philo_sleep(50);
	pthread_mutex_lock(&philo->philo_lock);
	philo->death_time = init_time + philo->table->time_die;
	pthread_mutex_unlock(&philo->philo_lock);
	while (simulation_conditions(philo->table))
	{
		philo_eat_sleep(philo);
		message(philo, THINKING);
		if (philo->meal_count == philo->table->meal_max && philo->table->is_bounded == 1)
		{
			message(philo, FINISHED);
			pthread_mutex_lock(&philo->philo_lock);
			philo->is_finished = 1;
			pthread_mutex_unlock(&philo->philo_lock);
			break;
		}
	}
	return (NULL);
}

/*
- take the table object as input and return 1 if simulation condition indicates
the end, else return 1
- objective : used in the simulation thead to check on if philosophers or have eaten enough
- get the flag(is_over) in the simulation table object with "flash access" mutex and then
store : to local variable - act like a quick transitor with fast mutex_protection
*/
int	simulation_conditions(t_table *table)
{
	int over;
	
	pthread_mutex_lock(&table->death_lock);
	over = table->is_over;
	pthread_mutex_unlock(&table->death_lock);
	if (over == 1)
		return (0);
	return (1);
}

void	*philone_routine(void *input)
{
	t_philo *philo;
	long long	init_time;
	
	philo = (t_philo *) input;
	init_time = get_simul_start(philo->table);
	pthread_mutex_lock(&philo->philo_lock);
	philo->death_time = init_time + philo->table->time_die;
	pthread_mutex_unlock(&philo->philo_lock);
	while (simulation_conditions(philo->table))
	{
		pthread_mutex_lock(philo->left_fork);
		message(philo, FORK);
		pthread_mutex_lock(&philo->philo_lock);
		philo_sleep(philo->table->time_die);
		pthread_mutex_unlock(&philo->philo_lock);
		pthread_mutex_unlock(philo->left_fork);
		message(philo, DIED);
	}
	return (NULL);
}

/*
void	end_simulation(t_table *table)
{
	int	i;
	t_philo philo;
	int	running;
	
	
	pthread_mutex_lock(&table->mutex_table);
	running = table->is_running;
	pthread_mutex_unlock(&table->mutex_table);
	if (running == 0)
		return;
	else
	{	
		pthread_mutex_lock(&table->mutex_table);
		table->is_running = 0;
		pthread_mutex_lock(&table->mutex_table);
	}
	i = 0;
	while (i < table->total)
	{
		philo = table->philos[i];
		pthread_join(philo.thread, NULL);
		i++;
	}
	i = 0;
	while (i < table->total)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->mutex_table);
	free(table->forks);
	free(table->philos);
	exit (0);
}


*/