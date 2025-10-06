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

static void	ground_philo(t_philo *phi, t_net *net, const char **arg);
static int	init_fork(t_philo *philos, int i, int n_of_philos);

t_philo	*init_philos(char **argv, t_net *net)
{
	t_philo			*philos;
	unsigned int	i;
	unsigned int	n_of_philos;

	n_of_philos = atoui(argv[1]);
	philos = malloc(n_of_philos * sizeof(t_philo));
	if (!philos)
		return (NULL);
	i = -1;
	while (++i < n_of_philos)
	{
		philos[i].num = i;
		ground_philo(&philos[i], net, (const char **)&argv[1]);
		if (!init_fork(philos, i, n_of_philos))
			return (NULL);
	}
	return (philos);
}

int	handle_net(int action, t_net *net)
{
	if (action == START)
	{
		net->obituary = 0;
		net->retire = 0;
		pthread_mutex_init(&net->life_feed, NULL);
		pthread_mutex_init(&net->last_whisper, NULL);
		pthread_mutex_init(&net->read, NULL);
		pthread_mutex_init(&net->go, NULL);
		pthread_mutex_lock(&net->go);
		return (1);
	}
	else
	{
		pthread_mutex_destroy(&net->last_whisper);
		pthread_mutex_destroy(&net->life_feed);
		pthread_mutex_destroy(&net->read);
		pthread_mutex_destroy(&net->go);
	}
	return (0);
}

long long	start(t_net *net)
{
	long long	started_at;

	pthread_mutex_lock(&net->go);
	pthread_mutex_unlock(&net->go);
	pthread_mutex_lock(&net->read);
	started_at = net->started_at;
	pthread_mutex_unlock(&net->read);
	return (started_at);
}

static void	ground_philo(t_philo *phi, t_net *net, const char **arg)
{
	static unsigned int	n_of_philos;
	static unsigned int	tt_die;
	static unsigned int	tt_eat;
	static unsigned int	tt_sleep;
	static unsigned int	meals;

	if (n_of_philos == 0)
	{
		n_of_philos = atoui(arg[0]);
		tt_die = atoui(arg[1]);
		tt_eat = atoui(arg[2]);
		tt_sleep = atoui(arg[3]);
		if (arg[4])
			meals = atoui(arg[4]);
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

static int	init_fork(t_philo *philos, int i, int n_of_philos)
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
