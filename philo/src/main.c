/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:26:54 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/10/06 18:20:49 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	spawn_philos(t_philo *philos);
static void	check_obituary(t_net *net);
static void	retire_philosophers(t_philo *philos);

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_net	net;

	if (is_invalid_args(argc, (const char **)&argv[1]))
		return (0);
	handle_net(START, &net);
	philos = init_philos(argv, &net);
	if (!philos)
		return (1);
	if (spawn_philos(philos) == -1)
	{
		free(philos);
		return (2);
	}
	usleep(1000);
	net.started_at = now();
	pthread_mutex_unlock(&net.go);
	check_obituary(&net);
	retire_philosophers(philos);
	handle_net(STOP, &net);
	free(philos);
	return (0);
}

static int	spawn_philos(t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos->code.n_of_philos)
	{
		if
		(
			pthread_create(&philos[i].thread, NULL, &life, (void *)&philos[i])
				!= 0
		)
			return (-1);
		i++;
	}
	return (0);
}

static void	check_obituary(t_net *net)
{
	while (1)
	{
		pthread_mutex_lock(&net->last_whisper);
		if (net->obituary > 0)
		{
			pthread_mutex_lock(&net->life_feed);
			net->retire = 1;
			printf("%lld %d died\n", now() - net->started_at, net->obituary);
			pthread_mutex_unlock(&net->life_feed);
			break ;
		}
		else if (net->obituary == -1)
		{
			break ;
		}
		pthread_mutex_unlock(&net->last_whisper);
		usleep(5000);
	}
	pthread_mutex_unlock(&net->last_whisper);
}

static void	retire_philosophers(t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos->code.n_of_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}
