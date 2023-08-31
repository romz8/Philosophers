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
	printf(YELLOW "%lld %i is thinking\n" RESET, timestamp_ms() - philo->start, philo->id);
}

void	philo_sleep(t_philo *philo)
{
	long	start_time;

	start_time = philo->start;
	printf(BLUE "%lld %i is sleeping\n" RESET, timestamp_ms() - start_time, philo->id);
	usleep(philo->table->time_sleep);
}

long	philo_eat(t_philo *philo)
{
	long long	meal_time;
	long long	start_time;
	int			id;

	id = philo->id;
	start_time = philo->start;
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	printf("%lld %i has taken a fork\n", timestamp_ms() - start_time, id); // careful must be oonly fork nad not nbered
	printf("%lld %i has taken a fork\n", timestamp_ms() - start_time, id);
	meal_time = timestamp_ms() - start_time;
	printf(GREEN "%lld %i is eating\n" RESET, meal_time, id);
	usleep(philo->table->time_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	philo->meal_count++;
	return (meal_time);
}

void	philo_death_risk(long long last_meal, t_philo *philo)
{
	int	id;
	long long start_time;

	id = philo->id;
	start_time = philo->start;
	if ((timestamp_ms() - start_time) - last_meal > philo->table->time_die )
	{
		printf(RED "%lld %i died\n" RESET, timestamp_ms() - start_time, id);
		pthread_mutex_lock(&philo->table->mutex_table);
		philo->table->death_count++;
		pthread_mutex_unlock(&philo->table->mutex_table);

	}
}

void	*philo_routine(void *input)
{
	long long meal_time;
	int			id;
	t_philo *philo;
	
	philo = (t_philo *) input;
	id = philo->id;
	while(1)
	{
		pthread_mutex_lock(&philo->table->mutex_table);
		if (philo->table->start_time != 0)
		{
			pthread_mutex_unlock(&philo->table->mutex_table);
			philo->start = philo->table->start_time;
			break;
		}
		usleep(50);
		pthread_mutex_unlock(&philo->table->mutex_table);
	} //or usleep(50); //as long as time set up don't do anything 
	while (1)
	{
		meal_time = philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
		philo_death_risk(meal_time, philo);
		pthread_mutex_lock(&philo->table->mutex_table);
		if (philo->table->death_count > 0 || (philo->is_bounded == 1 && !(philo->meal_count < philo->meal_max)))
		{
			pthread_mutex_unlock(&philo->table->mutex_table);
			break;
		}
		pthread_mutex_unlock(&philo->table->mutex_table); 
	}
	//printf("philo %i : total of meal is : %i\n", id, philo->meal_count);
	return (NULL);
}