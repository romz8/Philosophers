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
1. We check that all arguments are correct (min 5, max 6) all positive nbers
2. We init a table, forks (mutex) and philo data structure 
3. we link them all together via poiters to each other to ensure the 
simulation never miss a local event (at the philo scale) and vice versa
3. we launch the simulation
5. we launch the monitoring the conditions
6. free and exit (destroy the Mutexes and free Data struct - then quit)
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
