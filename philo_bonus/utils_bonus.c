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

#include "philo_bonus.h"

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
	long long	timing;

	timing = get_time_ms() - philo->table->start_time;
	sem_wait(philo->table->sem_write);
	if (type == DIED)
		printf(RED "%lld %i died\n" RESET, timing, philo->id);
	else if (type == FINISHED)
		printf(PINK "%lld %i FINISHED\n" RESET, timing, philo->id);
	else if (type == FORK)
		printf("%lld %i has taken a fork\n", timing, philo->id);
	else if (type == EATING)
		printf(GREEN "%lld %i is eating\n" RESET, timing, philo->id);
	else if (type == SLEEPING)
		printf(BLUE "%lld %i is sleeping\n" RESET, timing, philo->id);
	else if (type == THINKING)
		printf(YELLOW "%lld %i is thinking\n" RESET, timing, philo->id);
	sem_post(philo->table->sem_write);
	return ;
}

int	ft_exit(t_table *table, int exit_code)
{
	if (table->philo_pids)
		free(table->philo_pids);
	if (table->philos)
		free(table->philos);
	sem_close(table->sem_death);
	sem_close(table->sem_write);
	sem_close(table->sem_meals);
	exit(exit_code);
}

sem_t *safe_sem_init(char *sem_name, int value)
{
	sem_unlink(sem_name);
	return (sem_open(sem_name, O_CREAT | O_EXCL, 0666, value));
}

int clear_programme(t_table *table)
{
	sem_close(table->sem_death);
	sem_close(table->sem_write);
	sem_close(table->sem_meals);
	sem_close(table->sem_forks);
	sem_unlink(SEM_DEATH);
	sem_unlink(SEM_WRITE);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_FORKS);
	return(ft_exit(table, EXIT_SUCCESS));
}