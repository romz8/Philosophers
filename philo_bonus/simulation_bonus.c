/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:33:00 by rjobert           #+#    #+#             */
/*   Updated: 2023/09/11 11:33:03 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
/*
we "activate" the thread in each philo object and start its philo routine
1. we pass the object itself as an argument
2. we iniate the start time in the table object using a mutex to be sure.
you can see in the philo_routine taht no trhead starts while the start_time is
set up.
3. we join all the threads to wait for their execution.
*/
void	start_simulation(t_table *table)
{
	int		i;
	pid_t	pid;
	int		status;

	i = 0;
	table->start_time = get_time_ms();
	while (i < table->total)
	{
		pid = fork();
		if (pid == -1)
			ft_exit(table, EXIT_FAILURE);
		if (pid == 0)
		{
			table->philos[i].lock = safe_sem_init(SEM_LOCK, 1);
			philo_process(&table->philos[i]);
		}
		else
			table->philo_pids[i] = pid;
		i++;
	}
	waitpid(-1, &status, 0);
	// IF RECEIVEE SIIGNAL TO END -> KILL ALL PROCESS
	return ;
}
/*
monitoring if the simuluation should stop and close all the thread
accordingly if so
*/
void	simulation_monitor(t_table *table)
{
	pthread_t	monitor;
	
	pthread_create(&monitor, NULL, meal_check, (void *) table);
	pthread_detach(monitor);
	terminate_processes(table);
	return ;
}

void	*meal_check(void *input)
{
	t_table *table;
	int		total_finished;
	int		i;

	table = (t_table *) input;
	total_finished = 0;
	while (total_finished < table->total)
	{
		i = 0;
		while (i < table->total)
		{
			if (sem_wait(table->sem_meals))
				i++;
			usleep(50);
		}
		total_finished += 1;
	}
	sem_wait(table->sem_write);
	return (NULL);
}

void terminate_processes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->total)
	{
		if(kill(table->philo_pids[i], SIGTERM) == -1)
			ft_exit(table, EXIT_FAILURE);
		i++;
	}
}