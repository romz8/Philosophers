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
	volatile long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex_table;
}	t_table;

typedef struct s_philo
{
	int				id;
	int				meal_count;
	int				meal_max;
	int				is_bounded;
	long			start;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_table			*table;
}	t_philo;

/*struct timeval 
{
	time_t tv_sec;
	suseconds_t tv_usec;
}	s_timeval; */

int	ft_atoi(const char *s);
long long timestamp_ms(void);
void	*philo_routine(void *data);
void	philo_death_risk(long long last_meal, t_philo *philo);
long	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void   philo_think(t_philo *philo);
void	init_forks(pthread_mutex_t *forks, t_table *table);
void	clean_forks(pthread_mutex_t *forks, int n);
t_philo	*init_philosophers(t_table *table);
void	thread_dinner(t_philo *philo, t_table *table);
int		check_arguments(int argc, char **argv);

#endif