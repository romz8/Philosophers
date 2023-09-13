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

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

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
# define DEATH_CODE 43
# define MEAL_CODE 42
# define SEM_FORKS "/sem_forks"
# define SEM_WRITE "/sem_write"
# define SEM_STOP "/sem_stop"
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
	char			**sem_names;
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_stop;
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

/* Functions for Parsing and input check*/
int			ft_atoi(const char *s);
int			ft_isdigit(int c);
int			check_arguments(int argc, char **argv, int i, int j);

/* Functions to init Data Strcutures*/
int			init_table(t_table *table, char **argv);
int			init_philosophers(t_table *table);
char		**process_sem(t_table *table);
char		*custom_sem_philo(int id);

/* Function for process used to encode a philosopher life cycle*/
void		philo_lifecycle(t_philo *philo);
void		philo_sleep(int time_ms);
void		philo_process(t_philo *philo);
void		*life_thread(void *input);

/*Functions used to launch / montior / stop the simulation*/
void		start_simulation(t_table *table);
void		simulation_monitor(t_table *table);
int			simulation_stop(t_philo *philo);
void		terminate_processes(t_table *table);

/* utils : write to std_out, get_time, exit and clean memory*/
void		message(t_philo *philo, int type);
long long	get_time_ms(void);
int			ft_exit(t_table *table, int exit_code);
int			ft_strlen(const char *s);
sem_t		*safe_sem_init(char *sem_name, int value);
int			clear_programme(t_table *table);

#endif