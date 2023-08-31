/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 15:10:37 by rjobert           #+#    #+#             */
/*   Updated: 2023/08/30 15:10:42 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r'))
		i++;
	if (str[i] && (str[i] == '-' || str[i] == '+'))
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		nbr = (str[i] - 48) + nbr * 10;
		i++;
	}
	return (sign * nbr);
}

long long timestamp_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((long long) tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	clean_forks(pthread_mutex_t *forks, int n)
{
	int i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
}

int	check_arguments(int argc, char **argv)
{
	int i;
	int	j;

	if (argc < 5)
		return (1);
	i = 0;
	while (++i < argc - 1)
	{
		j = -1;
		while (argv[i][++j])
		{
			if (!ft_isdigit(argv[i][j]))
			{
			printf("Argument %i wrong - contain letter\n", i);
			return (1);
			}
		}
		if (ft_atoi(argv[i]) <= 0)
		{
			printf("Argument %i wrong - (negative number or zero)\n", i);
			return (1);
		}
	}
	return (0);
}
