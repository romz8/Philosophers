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

#include "philo_bonus.h"

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
void	philo_lifecycle(t_philo *philo)
{
	//printf("test LIFECYCLE process %i philo%i\n", getpid(), philo->id);
	sem_wait(philo->table->sem_forks);
	message(philo, FORK);
	sem_wait(philo->table->sem_forks);
	message(philo, FORK);
	message(philo, EATING);
	philo->is_eating = 1;
	philo->death_time = get_time_ms() + philo->table->time_die;
	philo_sleep(philo->table->time_eat);
	sem_post(philo->table->sem_forks);
	sem_post(philo->table->sem_forks);
	philo->is_eating = 0;
	philo->meal_count++;
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
ork at the same time and wait each other) - so if even 
philo_id -> start sligthly later
3. init the first time to die for a philo as he haven't eaten yet
4. while the simulation condition are correct, the philosopher executes his
eat / sleep / think rountines
*/
void	philo_process(t_philo *philo)
{

	while (get_time_ms() < philo->table->start_time)
		philo_sleep(50);
	if (philo->id % 2 == 0)
		philo_sleep(philo->table->time_eat / 10);
	philo->death_time = philo->table->start_time + philo->table->time_die;
	pthread_create(&philo->thread, NULL, life_thread, (void *) philo);
	pthread_detach(philo->thread);
	while (!simulation_stop(philo))
	{
		philo_lifecycle(philo);
		if (philo->meal_count == philo->table->meal_max && philo->table->is_bounded == 1)
		{
			message(philo, FINISHED);
			sem_close(philo->lock);
			sem_unlink(SEM_LOCK);
			exit(MEAL_CODE);
		}
	}
	exit(DEATH_CODE);
}

int	simulation_stop(t_philo *philo)
{
	int	local_state;

	sem_wait(philo->lock);
	local_state = philo->is_dead;
	sem_post(philo->lock);
	return (local_state);
}

void	*life_thread(void *input)
{
	t_philo *philo;

	philo = (t_philo *) input;
	while (1)
	{
		sem_wait(philo->lock);
		if(get_time_ms() >= philo->death_time && !philo->is_eating)
		{
			message(philo, DIED);
			philo->is_dead = 1;
			exit(DEATH_CODE);
		}
		sem_post(philo->lock);
		usleep(100);
	}
	return (NULL);
}