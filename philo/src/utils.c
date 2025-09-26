/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:31:03 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/24 12:36:42 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	f_save_times(t_times *times, const char **arg)
{
	times->meals = 0;
	times->philos = f_atoui(arg[0]);
	times->ttd = f_atoui(arg[1]);
	times->tte = f_atoui(arg[2]);
	times->tts = f_atoui(arg[3]);
	if (arg[4])
		times->meals = f_atoui(arg[4]);
}

unsigned int	f_atoui(const char *nptr)
{
	unsigned long int	result;

	result = 0;
	while (*nptr >= '0' && *nptr <= '9' && *nptr != '\0')
	{
		result = result * 10 + (*nptr) - '0';
		nptr++;
	}
	if (result > UINT_MAX)
		return (0);
	return ((unsigned int)result);
}

int	f_invalid_args(const int argc, const char **arg)
{
	unsigned int	w;
	unsigned int	c;
	int				is_invalid;

	is_invalid = FALSE;
	if (argc != 5 && argc != 6)
		is_invalid = TRUE;
	w = 0;
	while (arg[w] != NULL)
	{
		c = 0;
		while (arg[w][c] != '\0')
		{
			if (!f_is_digit(arg[w][c]))
				is_invalid = TRUE;
			c++;
		}
		w++;
	}
	if (is_invalid)
		printf(
			"usage: ./philo <np> <td> <te> <ts> <nm>(optional)\n"
			"np - number of philosophers and forks\ntd - time to die\n"
			"te - time to eat\nts - time to sleep\nnm - number of meals\n");
	return (is_invalid);
}

int	f_is_digit(int c)
{
	if (c >= 48 && c <= 57)
		return (TRUE);
	return (FALSE);
}
