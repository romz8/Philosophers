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
run eat + sleep + think consecutives routines
1. decrement sempahore to encode concurrency on forks : if availabel takes -1 
forks from the available pool of N total, otherwise wait for available forks 
if zero 
2. Eating message and is_eating = 1 too avoid killing a eating philo
3. update philosopher next death_time as per time_die in the table object
4. give back forks to the pool of available forks (increment semaphore)
5. put philo to sleep as per sleep-time in table object and with ms adjsutment
6. philo can wake up and think
*/
void	philo_lifecycle(t_philo *philo)
{
	sem_wait(philo->table->sem_forks);
	message(philo, FORK);
	sem_wait(philo->table->sem_forks);
	message(philo, FORK);
	message(philo, EATING);
	philo->is_eating = 1;
	sem_wait(philo->lock);
	philo->death_time = get_time_ms() + philo->table->time_die;
	sem_post(philo->lock);
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
take a the philo instance struct and run it as a process
1. create randomness to avoid deadlock (all philo reach their left 
ork at the same time and wait each other) - so if even 
philo_id -> start sligthly later
3. create a thread to monitor the death of the philo if starves (see below)
4. detach as we don't want to wait for it to execute the follwing instructions
4. while the simulation continue (linked to thread monitoring) we execute the
philo lifecycle 
5. if reach the number of meal from input and simulation is bounded -> exit with
special exit code
*/
void	philo_process(t_philo *philo)
{
	if (philo->id % 2 == 0)
		philo_sleep(philo->table->time_eat / 10);
	philo->death_time = philo->table->start_time + philo->table->time_die;
	pthread_create(&philo->thread, NULL, life_thread, (void *) philo);
	pthread_detach(philo->thread);
	while (!simulation_stop(philo))
	{
		philo_lifecycle(philo);
		if (philo->meal_count == philo->table->meal_max \
		&& philo->table->is_bounded == 1)
		{
			message(philo, FINISHED);
			sem_close(philo->lock);
			sem_unlink(SEM_LOCK);
			exit(MEAL_CODE);
		}
	}
	exit(DEATH_CODE);
}

/*
we lock the code by decrementing the semaphore, 
locally import the is_dead variable and return 0
or 1
the purpose is to continue the process while the 
philo is not dead
*/
int	simulation_stop(t_philo *philo)
{
	int	local_state;

	sem_wait(philo->lock);
	local_state = philo->is_dead;
	sem_post(philo->lock);
	return (local_state);
}

/*
this monitoring the philosopher (local to the process)
1. lock the philosopher code by decrementing the semaphore
of value 1, check if we are above his
expected death_time and he/she is not eating :
1.a if so we flaged the is_dead variable (to stop the
process loop) and exit with the appropirate 
1.b if not, we release the semaphore and wait 100 microsec
*/
void	*life_thread(void *input)
{
	t_philo	*philo;

	philo = (t_philo *) input;
	while (1)
	{
		sem_wait(philo->lock);
		if (get_time_ms() >= philo->death_time && !philo->is_eating)
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
