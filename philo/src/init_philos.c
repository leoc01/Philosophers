/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:31:03 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/27 19:56:47 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void			f_save_args(t_args *args, const char **arg);
static unsigned int	f_atoui(const char *nptr);

t_philo	*f_init_philos(int argc, char **argv)
{
	t_philo			*philos;
	t_args			args;
	unsigned int	i;

	if (f_is_invalid_args(argc, (const char **)&argv[1]))
		return (NULL);
	philos = malloc(args.n_of_philos * sizeof(t_philo));
	if (!philos)
		return (NULL);
	i = -1;
	while (++i < args.n_of_philos)
	{
		philos[i].num = i;
		f_save_args(&philos[i].args, (const char **)&argv[1]);
		if (i == 0)
			philos[i].fork_l = &philos[args.n_of_philos].fork_r;
		else
			philos[i].fork_l = &philos[i - 1].fork_r;
		if (pthread_mutex_init(&philos[i].fork_r, NULL) == -1)
		{
			free(philos);
			return (NULL);
		}
	}
	return (philos);
}

static void	f_save_args(t_args *args, const char **arg)
{
	static unsigned int	n_of_philos;
	static unsigned int ttd;
	static unsigned int tte;
	static unsigned int tts;
	static unsigned int	meals;

	if (n_of_philos ==0)
	{
		n_of_philos = f_atoui(arg[0]);
		ttd = f_atoui(arg[1]);
		tte = f_atoui(arg[2]);
		tts = f_atoui(arg[3]);
		if (arg[4])
			meals = f_atoui(arg[4]);
	}
	args->n_of_philos = n_of_philos;
	args->ttd = ttd;
	args->tte = tte;
	args->tts = tts;
	args->meals = meals;
}

static unsigned int	f_atoui(const char *nptr)
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
