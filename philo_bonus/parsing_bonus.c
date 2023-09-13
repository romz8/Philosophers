/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 18:31:35 by rjobert           #+#    #+#             */
/*   Updated: 2023/09/06 18:31:37 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
This check that all arguments numbers and positive ones
*/
int	check_arguments(int argc, char **argv, int i, int j)
{
	if (argc <= 4 || argc > 6)
	{
		printf("Wrong number of arguments. \n Usage is ./philo N_philosophers \
		time_to_die time_to_eat time_to_sleep [max_nb_meal optional]\n");
		return (1);
	}
	i = 0;
	while (++i < argc)
	{
		j = -1;
		while (argv[i][++j])
		{
			if (!ft_isdigit(argv[i][j]))
			{
				printf("Argument %i wrong - negative or contain letter\n", i);
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
