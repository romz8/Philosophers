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

/*
Take the philo instance as an input, return nothing and  
run eat + sleep + think consecutives routines
1. use forks mutex in left-right order to avoid data race while locking 
the forks
2. mutex the philosopher object to prevent eating / dying at same time as well
as avoiding time updating vs time catching in the routine check.
3. update philosopher next death_time as per time_die in the table object
4. unlock philo mutex
5. put down forks (release the mutex) in the same order to avoid deadlock
6. put philo to sleep as per sleep-time in table object and with ms adjsutment
*/
void	philo_lifecycle(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	message(philo, FORK);
	pthread_mutex_lock(philo->right_fork);
	message(philo, FORK);
	pthread_mutex_lock(&philo->philo_lock);
	message(philo, EATING);
	philo->death_time = get_time_ms() + philo->table->time_die;
	philo->meal_count++;
	pthread_mutex_unlock(&philo->philo_lock);
	philo_sleep(philo->table->time_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	message(philo, SLEEPING);
	philo_sleep(philo->table->time_sleep);
	message(philo, THINKING);
}

/*
take a void pointer as input (as per pthread_create) that is converted
back to t_philo pnters. return a void pointer (as per pthread_create) 
and run philo_routine as long as no death.
1. convert void * to t_philo * 
2. create randomness to avoid deadlock (all philo reach their left 
fork at the same time and wait each other) - so if even 
philo_id, it will start the routine sligthly later
3. init the first time to die for a philo as he haven't eaten yet
4. while the simulation condition are correct, the philosopher executes his
eat / sleep / think rountines
*/
void	*philo_routine(void *input)
{
	t_philo		*philo;
	long long	init_time;

	philo = (t_philo *) input;
	if (philo->id % 2 == 0)
		philo_sleep(philo->table->time_eat / 10);
	init_time = get_simul_start(philo->table);
	pthread_mutex_lock(&philo->philo_lock);
	philo->death_time = init_time + philo->table->time_die;
	pthread_mutex_unlock(&philo->philo_lock);
	life_simulation(philo);
	return (NULL);
}

/* 
special case of one philosopher (only one fork and bound to die)
*/
void	*philone_routine(void *input)
{
	t_philo		*philo;
	long long	init_time;

	philo = (t_philo *) input;
	init_time = get_simul_start(philo->table);
	pthread_mutex_lock(&philo->philo_lock);
	philo->death_time = init_time + philo->table->time_die;
	pthread_mutex_unlock(&philo->philo_lock);
	pthread_mutex_lock(philo->left_fork);
	message(philo, FORK);
	pthread_mutex_lock(&philo->philo_lock);
	philo_sleep(philo->table->time_die);
	pthread_mutex_unlock(&philo->philo_lock);
	pthread_mutex_unlock(philo->left_fork);
	message(philo, DIED);
	return (NULL);
}

void	output_message(t_philo *philo, int type, long long start_time)
{
	if (type == DIED)
	{
		printf(RED "%lld %i died\n" RESET, get_time_ms() - start_time, \
			philo->id);
		reach_end_condition(philo->table);
	}
	else if (type == FINISHED)
		printf(PINK "%lld %i FINISHED\n" RESET, get_time_ms() - start_time, \
			philo->id);
	else if (type == FORK)
		printf("%lld %i has taken a fork\n", get_time_ms() - start_time, \
			philo->id);
	else if (type == EATING)
		printf(GREEN "%lld %i is eating\n" RESET, get_time_ms() - start_time, \
			philo->id);
	else if (type == SLEEPING)
		printf(BLUE "%lld %i is sleeping\n" RESET, get_time_ms() - start_time, \
			philo->id);
	else if (type == THINKING)
		printf(YELLOW "%lld %i is thinking\n" RESET, get_time_ms() - \
		start_time, philo->id);
}

/*
the part of the philo_rountine that run in a loop (separated here because
42 norms implies 1 function = 25 lines)
1. while the simulation conditons (see simulation.c) do :
2. philo life_cycle
3. if the meal count is now on the bounded target from user input :
write to stdout, indicate philo finished eating and break the loop
*/
void	life_simulation(t_philo *philo)
{
	while (simulation_conditions(philo->table))
	{
		philo_lifecycle(philo);
		if (philo->meal_count == philo->table->meal_max \
		&& philo->table->is_bounded == 1)
		{
			message(philo, FINISHED);
			pthread_mutex_lock(&philo->philo_lock);
			philo->is_finished = 1;
			pthread_mutex_unlock(&philo->philo_lock);
			break ;
		}
	}
}
