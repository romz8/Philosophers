/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 14:49:45 by rjobert           #+#    #+#             */
/*   Updated: 2023/08/30 14:49:48 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_H
# define PHILO_H

# define BLUE "\x1B[34m"
# define YELLOW "\x1B[33m"
# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define RESET "\x1B[0m"

# define DIED 0
# define EATING 1
# define THINKING 2
# define FORK 3
# define SLEEPING 4

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table
{
	int				total;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				meal_max;
	int				is_bounded;
	int				death_count;
	int				is_over;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	time_lock;
	pthread_t		simul_thread;
	struct s_philo 	*philos;
}	t_table;

typedef struct s_philo
{
	int				id;
	int				meal_count;
	int				meal_max;
	long long		death_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	philo_lock;
	t_table			*table;
}	t_philo;

int	ft_atoi(const char *s);
int	ft_isdigit(int c);
long long get_time_ms(void);
void	*philo_routine(void *data);
void   philo_think(t_philo *philo);
void	init_forks(pthread_mutex_t *forks, t_table *table);
void	clean_forks(pthread_mutex_t *forks, int n);
t_philo	*init_philosophers(t_table *table);
void	thread_dinner(t_philo *philo, t_table *table);
int		check_arguments(int argc, char **argv);
int		simulation_conditions(t_table *table);
void	stop_simulation(t_table *table);
int		simulation_continue(t_table *table);
void	message(t_philo *philo, int type);
void	philo_sleep(int time_ms);
void	simulation_thread(t_table *table);
void	*simulation_routine(void *input);

#endif