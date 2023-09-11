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

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}

void	philo_sleep(int time_ms)
{
	long long	time;

	time = get_time_ms();
	while (get_time_ms() - time < time_ms)
		usleep(100);
}

/*
this function display what event philospher of id X did a what time
1. catch the time of start of the simulation and lock the code with a mutex 
(to impede all philospohersto write to STDOUT at the same time)
2. based on the type of message invoked, display the correct message with philo
 id and time_diff
3. if the philosopher DIED -> increased death count
4. unlock the mutex code so waiting philosopher can expess themselves
*/
void	message(t_philo *philo, int type)
{
	long long	start_time;

	start_time = get_simul_start(philo->table);
	pthread_mutex_lock(&philo->table->write_lock);
	if (!simulation_conditions(philo->table))
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	output_message(philo, type, start_time);
	pthread_mutex_unlock(&philo->table->write_lock);
	return ;
}

void	reach_end_condition(t_table *table)
{
	pthread_mutex_lock(&table->death_lock);
	table->is_over = 1;
	pthread_mutex_unlock(&table->death_lock);
}

long long	get_simul_start(t_table *table)
{
	long long	start_time;

	pthread_mutex_lock(&table->time_lock);
	start_time = table->start_time;
	pthread_mutex_unlock(&table->time_lock);
	return (start_time);
}
