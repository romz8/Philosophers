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
Launch the simulation where all philos are child processes
1. run a loop of N iterations for N philos
2. fork() and retrieve the pid, if -1 error handing
3. if we are in the child (pid == 0) we are in the code for the child process :
execute the philo_routine based and pass the corresponding philo struct pntr
3. otherwise in main (parent) process and store the pids to later on kill and
avoid orphan or zombie process
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
monitoring if the simuluation should stop based on case
1. we run a loop and use waitpid with -1 (wait for any child process
to return) : it doesn't do anything as long as no child return so no 
CPU usage in this loop
2. as soon as first child exit with a code, we check the status with
WIFEXITSATUS macro ; if death_code (43) or error code -> we terminate
all the process and unlock the semaphore sem_stop to clear the simulation
3. if it is one philo with exit MEAL_CODE (42) we increment the nbr 
of philo who finished and continue un reach total of philo 
*/
void	simulation_monitor(t_table *table)
{
	int		status;
	int		total_finished;
	pid_t	pid;

	total_finished = 0;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == DEATH_CODE)
		{
			sem_post(table->sem_stop);
			terminate_processes(table);
			break ;
		}
		else if (WIFEXITED(status) && WEXITSTATUS(status) == MEAL_CODE)
			total_finished++;
		if (total_finished == table->total)
		{
			sem_post(table->sem_stop);
			break ;
		}
		usleep(100);
	}
}

/*
Terminate all child processes to avoid any orphan process
*/
void	terminate_processes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->total)
	{
		if (kill(table->philo_pids[i], SIGTERM) == -1)
			ft_exit(table, EXIT_FAILURE);
		i++;
	}
}
