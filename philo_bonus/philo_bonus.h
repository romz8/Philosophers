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
# define PINK "\x1B[95m"
# define DIED 0
# define EATING 1
# define THINKING 2
# define FORK 3
# define SLEEPING 4
# define FINISHED 5
# define SEM_FORKS "/sem_forks"
# define SEM_WRITE "/sem_write"
# define SEM_MEAL "/sem_meal"
# define SEM_DEATH "/sem_death"
# define SEM_LOCK "/sem_lock"

# include <stdio.h>
# include <stdlib.h>
# include <semaphore.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>

typedef struct s_table
{
	int				total;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				meal_max;
	int				is_bounded;
	long long		start_time;
	struct s_philo	*philos;
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_meals;
	sem_t			*sem_death;
	pid_t			*philo_pids;
}	t_table;

typedef struct s_philo
{
	int				id;
	int				meal_count;
	int				is_eating;
	int				is_dead;
	long long		death_time;
	pthread_t		thread;
	sem_t			*lock;
	t_table			*table;
}	t_philo;

int			ft_atoi(const char *s);
int			ft_isdigit(int c);
int			check_arguments(int argc, char **argv, int i, int j);

int			init_table(t_table *table, char **argv);
int			init_philosophers(t_table *table);

void		philo_lifecycle(t_philo *philo);
void		philo_sleep(int time_ms);
void		philo_process(t_philo *philo);
void		*life_thread(void *input);

void		start_simulation(t_table *table);
void		*meal_check(void *input);
void		simulation_monitor(t_table *table);
int			simulation_stop(t_philo *philo);
void		terminate_processes(t_table *table);

void		message(t_philo *philo, int type);
long long	get_time_ms(void);
int			ft_exit(t_table *table, int exit_code);
sem_t 		*safe_sem_init(char *sem_name, int value);
int			clear_programme(t_table *table);

#endif