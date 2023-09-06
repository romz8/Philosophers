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

/*
Take the philo instance as an input, return nothing and  
run eat + sleep consecutives routines
1. if the simualtion condition are not to continue, leave and do nothing
2. use the pickup_forks() function to avoid data race and deadlock
3. update philosopher xext death_time as per time_die in the table object
4. put down forks (release the mutex)
5. put philo to sleep as per sleep-time in table object and with ms adjsutment
*/
void	philo_eat_sleep(t_philo *philo)
{
	if (!simulation_conditions(philo->table))
		return ;
	pickup_forks(philo);
	message(philo, EATING);
	pthread_mutex_lock(&philo->philo_lock);
	philo->death_time = get_time_ms() + philo->table->time_die;
	pthread_mutex_unlock(&philo->philo_lock);
	philo_sleep(philo->table->time_eat, philo->table);
	philo->meal_count++;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	message(philo, SLEEPING);
	philo_sleep(philo->table->time_sleep, philo->table);
}

/*
take a void pointer as input (as per pthread_create) that is converted
back to t_philo pnters. return a void pointer (as per pthread_create) and
run philo_routine as long as no death.
1. convert void * to t_philo * 
2. do not start as long as the time as been init is null : use sleep (in the 
main thread, once all threads are created then time is setup so we can  
start the simulation when all philosophers are seated)
3. init the first time to die for a philo as he haven't eaten yet
4. while the simulation condition are correct, the philosopher executes his
eat / sleep / think rountines
*/
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
		usleep(0);
	}
	pthread_mutex_lock(&philo->philo_lock);
	philo->death_time = init_time + philo->table->time_die;
	pthread_mutex_unlock(&philo->philo_lock);
	while (simulation_conditions(philo->table) == 1)  //!death_risk(philo) //simulation_conditions(philo->table)
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
	int over;
	
	pthread_mutex_lock(&table->death_lock);
	over = table->is_over;
	pthread_mutex_unlock(&table->death_lock);
	if (over == 1)
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