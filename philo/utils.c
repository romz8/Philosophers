/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 15:10:37 by rjobert           #+#    #+#             */
/*   Updated: 2023/08/30 15:10:42 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}

void	philo_sleep(int time_ms)
{
	long long time;

	time = get_time_ms();
	while (get_time_ms() - time < time_ms)
		usleep(100);
}

void	clean_forks(pthread_mutex_t *forks, int n)
{
	int i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
}

/*
this function display what event philospher of id X did a what time
1. catch the time of start of the simulation and lock the code with a mutex (to impede all philospohers
to write to STDOUT at the same time)
2. based on the type of essage invoked, display the correct message with philo id and time_diff
3. if the philosopher DIED -> increased death count
4. unlock the mutex code so waiting philosopher can expess themselves
*/
void	message(t_philo *philo, int type)
{
	long long start_time;

	pthread_mutex_lock(&philo->table->time_lock);
	start_time = philo->table->start_time;
	pthread_mutex_unlock(&philo->table->time_lock);
	pthread_mutex_lock(&philo->table->write_lock);
	
	if (!simulation_conditions(philo->table))
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return;
	}
	if (type == DIED)
	{
		printf(RED "%lld %i died\n" RESET, get_time_ms() - start_time, philo->id);
		pthread_mutex_lock(&philo->table->death_lock);
		philo->table->is_over =1;
		pthread_mutex_unlock(&philo->table->death_lock);
	}
	else if (type == FORK)
		printf("%lld %i has taken a fork\n", get_time_ms() - start_time, philo->id);
	else if (type == EATING)
		printf(GREEN "%lld %i is eating\n" RESET, get_time_ms() - start_time, philo->id);
	else if (type == SLEEPING)
		printf(BLUE "%lld %i is sleeping\n" RESET, get_time_ms() - start_time, philo->id);
	else if (type == THINKING)
		printf(YELLOW "%lld %i is thinking\n" RESET, get_time_ms() - start_time, philo->id);
	pthread_mutex_unlock(&philo->table->write_lock);
	return ;
}

void	reach_end_condition(t_table *table)
{
	pthread_mutex_lock(&table->death_lock);
	table->is_over =1;
	pthread_mutex_unlock(&table->death_lock);
}
