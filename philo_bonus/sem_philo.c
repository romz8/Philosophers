/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 13:36:43 by rjobert           #+#    #+#             */
/*   Updated: 2023/09/13 13:36:45 by rjobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
/*
we need to open a unique semaphore for our philosopher based on his/her id
1. convert his id from integer to char *s
2. concatenate "sem_philo_" with this "id" and then safely init the semaphore
*/

static int	number_bsize(int n)
{
	int	i;

	i = 0;
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*malloc_dynam(long nb, int size, int neg)
{
	char	*nbr;

	if (nb != 0)
		nbr = malloc(size + 1);
	else
		return ("0");
	if (!nbr)
		return (NULL);
	nbr[size] = '\0';
	while (--size)
	{
		nbr[size] = nb % 10 + '0';
		nb /= 10;
	}
	if (neg == 1)
		nbr[0] = '-';
	else
		nbr[0] = nb % 10 + '0';
	return (nbr);
}

char	*ft_itoa(int n)
{
	char	*nbr;
	long	nb;
	int		neg;
	int		size;

	nb = n;
	neg = 0;
	if (n < 0)
	{
		size = number_bsize(n) + 1;
		neg = 1;
		nb *= -1;
	}
	else
		size = number_bsize(n);
	nbr = malloc_dynam(nb, size, neg);
	return (nbr);
}

int	ft_strlen(char const *s)
{
	int	i;
	i = 0;

	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin_modif(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t i;
	size_t	n;
	char	*concat;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	concat = malloc(s1_len + s2_len + 1);
	if (!concat)
		return (NULL);
	i = -1;
	while (++i < s1_len)
		concat[i] = s1[i];
	n = i;
	i = -1;
	while (++i < s2_len)
		concat[n++] = s2[i];
	concat[s1_len + s2_len] = '\0';
	return (concat);
}

char	*custom_sem_philo(int id)
{
	char *sem_name;
	char *philo_char_id;

	philo_char_id = ft_itoa(id);
	sem_name = ft_strjoin_modif("/sem_philo_", philo_char_id);
	return (sem_name);
}