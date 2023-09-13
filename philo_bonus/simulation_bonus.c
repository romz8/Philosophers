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

	table->start_time = get_time_ms();
	i = 0;
	while (i < table->total)
	{
		pid = fork();
		if (pid == -1)
			ft_exit(table, EXIT_FAILURE);
		if (pid == 0)
			philo_process(&table->philos[i]);
		else
			table->philo_pids[i] = pid;
		i++;
	}
	return ;
}
/*
monitoring if the simuluation should stop and close all the thread
accordingly if so
*/
void	simulation_monitor(t_table *table)
{
	int 	status;
	int		total_finished;
	pid_t	pid;

	total_finished = 0;
	while(1)
	{
		pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			if(WEXITSTATUS(status) == DEATH_CODE)
			{
				sem_post(table->sem_stop);
				terminate_processes(table);
				break;
			}
			else if (WEXITSTATUS(status) == MEAL_CODE)
				total_finished++;
			if (total_finished == table->total)
			{
				sem_post(table->sem_stop);
				break;
			}
		}
		usleep(100);
	}
}

void terminate_processes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->total)
	{
		free(table->philos[i].sem_name);
		if(kill(table->philo_pids[i], SIGTERM) == -1)
			ft_exit(table, EXIT_FAILURE);
		i++;
	}
	printf("killed all processed");
}