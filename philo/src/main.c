/* *****************q********************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:26:54 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/24 12:54:12 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	main(int argc, char **argv)
{
	t_times	times;
	int		i;

	if (f_invalid_args(argc, (const char **)&argv[1]))
		return (1);
	f_save_times(&times, (const char **)&argv[1]);
	i = 0;
	printf("%d\n", times.philos);
	printf("%d\n", times.ttd);
	printf("%d\n", times.tte);
	printf("%d\n", times.tts);
	printf("%d\n", times.meals);
}

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
	while (*nptr >= '0' && *nptr <= '9' && *nptr != '\0' )
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

	if (argc != 5 && argc != 6)
		return (TRUE);
	w = 0;
	while (arg[w] != NULL)
	{
		c = 0;
		while (arg[w][c] != '\0')
		{
			if (!f_is_digit(arg[w][c]))
				return (TRUE);
			c++;
		}
		w++;
	}
	return (FALSE);
}

int	f_is_digit(int c)
{
	if (c >= 48 && c <= 57)
		return (TRUE);
	return (FALSE);
}
