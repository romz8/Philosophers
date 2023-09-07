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
1. We initiate a table and philo data structre then check arguments are correct
2. we fill the table data from arguments, special case if user entered a 
max nber of meal
3. we init the mutex of the table (that will be use when a thread modify a table
data like deat_count and start time)
4. we init the forks data struct (an array of N forks, all mutexes)
5. we init philosopers (see below)
6. destroy the Mutexes and free Data struct
*/
int main(int argc, char **argv)
{
	t_table table;
	pthread_mutex_t *forks;

	if (check_arguments(argc, argv))
		return (1);
	table.total = ft_atoi(argv[1]);
	table.death_count = 0;
	table.time_die = ft_atoi(argv[2]);
	table.time_eat = ft_atoi(argv[3]);
	table.time_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		table.is_bounded = 1;
		table.meal_max= ft_atoi(argv[5]);
	}
	else
		table.is_bounded = 0;
	table.start_time = 0;
	table.is_over = 0;
	forks = NULL;
	pthread_mutex_init(&table.time_lock, NULL);
	pthread_mutex_init(&table.death_lock, NULL);
	pthread_mutex_init(&table.write_lock, NULL);
	init_forks(forks, &table);
	table.philos = init_philosophers(&table);
	thread_dinner(table.philos, &table);
	simulation_thread(&table);
	// while (simulation_continue(&table))
	// 	usleep(50);
	stop_simulation(&table);
	//printf("the death count is %i\n", table.death_count);
	//free(forks); - do a proper function carefull
	//pthread_mutex_destroy(&table.mutex_table);
	//clean_forks(forks, table.total);
	return (0);
}

/*
MALLOC - THIS NEEDS TO BE FREED - CHECKED ?
1. we allocate N memory space of size N as nbr forks = nbr phillo
2. malloc protected
3. we init all the mutexes in the mallocated mutex array
4. we make the forks pointer in table DS point toward our array
*/
void	init_forks(pthread_mutex_t *forks, t_table *table)
{
	int i;
	
	forks = malloc(sizeof(pthread_mutex_t) * table->total);
	if (!forks)
	{	
		free(table);
		return ;
	}
	i = 0;
	while (i < table->total)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	table->forks = forks;
}

/*
we init our threads based on our nber of philosophers
1. receive table DS and from there take the nber of philo input
to allocated an array of philo struct - malloc protected and if 
issue free the table object.
2. fill the struct in the array for all philo : it's id, init nber of
meals, if it has a cap, and fill in the forks and table data.
3. the forks are filled as follwo : forks array goes from 0 to N -1 and 
philo id go from 1 to N : philo 1 has fork[0] o his left and fork[1] on 
his right, philo 2 has fork[1] and fork[2], ..., philo n has fork[n - 1] and
fork [0]. so we fill the philo of id [i + 1] with left_mutex pointer has fork[i]
and the left fork[i + 1] and make it modulo nber of philo so that philo [n] left
fork is [n % n] = 0.
4. we return the philo array
*/
t_philo	*init_philosophers(t_table *table)
{
	int		i;
	t_philo	*philo;

	philo = malloc(sizeof(t_philo) * table->total);
	if (!philo)
	{
		//clean_forks(table->forks, table->total);
		free(table);
		return (NULL);
	}
	i = -1;	
	while (++i < table->total)
	{
		philo[i].id = i + 1;
		philo[i].meal_count = 0; 
		philo[i].left_fork = &table->forks[i];
		philo[i].right_fork = &table->forks[(i + 1) % table->total];
		philo[i].table = table;
		pthread_mutex_init(&philo[i].philo_lock, NULL);
	}
	return (philo);
}

/*
we "activate" the thread in each philo object and start its philo routine
1. we pass the object itself as an argument
2. we iniate the start time in the table object using a mutex to be sure.
you can see in the philo_routine taht no trhead starts while the start_time is
set up.
3. we join all the threads to wait for their execution.
*/
void	thread_dinner(t_philo *philo, t_table *table)
{
	int i;

	i = 0;
	table->start_time = get_time_ms() + table->total * 20;
	while (i < table->total)
	{
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		i++;
	}
	pthread_mutex_lock(&philo->table->time_lock);
	table->start_time = get_time_ms(); //to make sure time doesn't start while everybody is seated
	pthread_mutex_unlock(&philo->table->time_lock);
}

int	simulation_continue(t_table *table)
{
	int	i;
	long long death_time;
	
	i = 0;
// 	while (get_time_ms() < table->start_time || !table->start_time)
// 		usleep(100);
	while (i < table->total)
	{
		pthread_mutex_lock(&table->philos[i].philo_lock);
		death_time = table->philos[i].death_time;
		pthread_mutex_unlock(&table->philos[i].philo_lock);
		if (get_time_ms() >= death_time && death_time)
		{
			message(&table->philos[i], DIED);
			return (0);
		}
		i++;
	}
	return (1);
}

void	*simulation_routine(void *input)
{
	t_table *table;

	table = (t_table *) input;

	while(1)
	{
		if (!simulation_continue(table))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}

void	simulation_thread(t_table *table)
{
	pthread_create(&table->simul_thread, NULL, simulation_routine, (void *) table);
}
void	stop_simulation(t_table *table)
{
	int		i;
	t_philo *philos;

	philos = table->philos;
	i = 0;
	while (i < table->total)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(table->simul_thread, NULL);
	return ;
}