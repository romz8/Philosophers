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
	if (!simulation_conditions(philo->table))
		return ;
	message(philo, THINKING);
}

void	pickup_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		message(philo, FORK);
		pthread_mutex_lock(philo->left_fork);
		message(philo, FORK);
	}
	else 
	{
		pthread_mutex_lock(philo->left_fork);
		message(philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		message(philo, FORK);
	}
}

void	philo_eat_sleep(t_philo *philo)
{
	if (!simulation_conditions(philo->table))
		return ;
	pickup_forks(philo);
	//philo->is_eating = 1;
	message(philo, EATING);
	philo->death_time = get_time_ms() + philo->table->time_die;
	philo_sleep(philo->table->time_eat, philo->table);
	//philo->is_eating = 0;
	philo->meal_count++;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	message(philo, SLEEPING);
	philo_sleep(philo->table->time_sleep, philo->table);
}

void	*philo_routine(void *input)
{
	t_philo *philo;
	long long	init_time;
	
	philo = (t_philo *) input;
	while(1)
	{
		pthread_mutex_lock(&philo->table->time_lock);
		init_time = philo->table->start_time;
		pthread_mutex_unlock(&philo->table->time_lock);
		if (init_time != 0)
			break;
		usleep(5);
	}
	philo->death_time = get_time_ms() + philo->table->time_die;
	while (!death_risk(philo))
	{
		philo_eat_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

int death_risk(t_philo *philo)
{

	pthread_mutex_lock(&philo->philo_lock);
	if (get_time_ms() >= philo->death_time)
	{
		message(philo, DIED);
		pthread_mutex_unlock(&philo->philo_lock);
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&philo->philo_lock);
		return (0);
	}
}

int	simulation_conditions(t_table *table)
{
	
	int death_count_local;
	
	pthread_mutex_lock(&table->death_lock);
	death_count_local = table->death_count;
	pthread_mutex_unlock(&table->death_lock);
	if (death_count_local > 0)
		return (0);
	return (1);
}
/*
void	eating_permission(t_philo *philo)
{
	t_philo philo_left;
	t_philo philo_right;
	int		side_left;
	int		side_right;

	if (philo->id == 1)
	{
		side_left = philo->table->total;
		side_right = philo->id + 1;
	}
	else if (philo->id == philo->table->total)
	{	
		side_left = philo->id - 1;
		side_right = 1;
	}
	else
	{
		side_left = philo->id - 1;
		side_right = philo->id - 1;
	}
	philo_left = philo->table->philos[side_left];
	philo_right = philo->table->philos[side_right];
	while (!(philo_left.is_eating == 0 && philo_right.is_eating == 0))
		usleep(10);
	return;
} */



/*void	*death_risk_thread(t_philo *philo)
{

	while (simulation_conditions(philo))
	{
		pthread_mutex_lock(&philo->philo_lock);
		if (timestamp_ms() >= philo->death_time && philo->is_eating == 0)
		{
			message(philo, DIED);
			pthread_mutex_unlock(&philo->philo_lock);
			break;
		}
		else
			pthread_mutex_unlock(&philo->philo_lock);
	}
	return (1);
}
*/



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