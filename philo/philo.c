/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 14:50:15 by rjobert           #+#    #+#             */
/*   Updated: 2023/08/30 14:50:18 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 
1. We declare a table and philo data structure then check arguments are 
correct
2. we fill the table data from arguments, special case if user entered a 
max nber of meal
3. we init the mutex of the table object(that will be use when a thread modify
 a table
data like deat_count and start time)
4. we init the forks data struct (an array of N forks, all mutexes)
5. we init philosopers (see below)
6. destroy the Mutexes and free Data struct
*/
int	main(int argc, char **argv)
{
	t_table			table;
	pthread_mutex_t	*forks;

	if (check_arguments(argc, argv, 0, 0))
		return (1);
	if (init_table(&table, argv))
		return (exit_bad(&table, EXIT_FAILURE));
	forks = NULL;
	if (init_forks(forks, &table))
		return (exit_bad(&table, EXIT_FAILURE));
	table.philos = init_philosophers(&table);
	if (!table.philos)
		return (exit_bad(&table, EXIT_FAILURE));
	if (start_simulation(table.philos, &table))
		return (exit_free_all(&table, EXIT_FAILURE));
	if (simulation_monitor(&table))
		return (exit_free_all(&table, EXIT_FAILURE));
	return (exit_free_all(&table, EXIT_SUCCESS));
}
