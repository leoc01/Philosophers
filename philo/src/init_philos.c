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

static void			f_ground_philo(t_philo *phi, t_net *net, const char **arg);
static unsigned int	f_atoui(const char *nptr);

int	init_fork(t_philo *philos, int i, int n_of_philos)
{
	if (n_of_philos > 1)
	{
		if (i == 0)
		{
			philos[i].his_fork = &philos[n_of_philos - 1].my_fork;
			philos[i].fork_l = &philos[n_of_philos - 1].fork_r;
		}
		else
		{
			philos[i].his_fork = &philos[i - 1].my_fork;
			philos[i].fork_l = &philos[i - 1].fork_r;
		}
	}
	else
	{
		philos[i].his_fork = NULL;
		philos[i].fork_l = NULL;
	}
	if (pthread_mutex_init(&philos[i].fork_r, NULL) == -1)
	{
		free(philos);
		return (0);
	}
	return (1);
}

t_philo	*f_init_philos(char **argv, t_net *net)
{
	t_philo			*philos;
	unsigned int	i;
	unsigned int	n_of_philos;

	n_of_philos = f_atoui(argv[1]);
	philos = malloc(n_of_philos * sizeof(t_philo));
	if (!philos)
		return (NULL);
	i = -1;
	while (++i < n_of_philos)
	{
		philos[i].num = i;
		f_ground_philo(&philos[i], net, (const char **)&argv[1]);
		if (!init_fork(philos, i, n_of_philos))
			return (NULL);
	}
	return (philos);
}

static void	f_ground_philo(t_philo *phi, t_net *net, const char **arg)
{
	static unsigned int	n_of_philos;
	static unsigned int	tt_die;
	static unsigned int	tt_eat;
	static unsigned int	tt_sleep;
	static unsigned int	meals;

	if (n_of_philos == 0)
	{
		n_of_philos = f_atoui(arg[0]);
		tt_die = f_atoui(arg[1]);
		tt_eat = f_atoui(arg[2]);
		tt_sleep = f_atoui(arg[3]);
		if (arg[4])
			meals = f_atoui(arg[4]);
	}
	phi->code.n_of_philos = n_of_philos;
	phi->code.tt_die = tt_die;
	phi->code.tt_eat = tt_eat;
	phi->code.tt_sleep = tt_sleep;
	phi->code.meals = meals;
	phi->remaining_meal = 1;
	if (meals)
		phi->remaining_meal = meals;
	phi->net = net;
	phi->my_fork = 1;
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
