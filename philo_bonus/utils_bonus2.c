/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 20:41:39 by rjobert           #+#    #+#             */
/*   Updated: 2023/09/13 20:41:41 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_strlen(char const *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

/*
1. unlinking the semaphore we are creating (to start on a clean slate)
2. creating it with the value and name passed on
3. checking there is no issue while opening and clean if necessary
*/
sem_t	*safe_sem_init(char *sem_name, int value)
{
	sem_unlink(sem_name);
	return (sem_open(sem_name, O_CREAT | O_EXCL, 0666, value));
}
