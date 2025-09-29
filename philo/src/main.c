/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:26:54 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/29 18:07:57 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*life(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	usleep(1000000);
	pthread_mutex_lock(&philo->net->last_whisper);
	if (philo->net->retire)
	{
		pthread_mutex_unlock(&philo->net->last_whisper);
	}
	else
	{
		philo->net->someone_died = philo->num;
		pthread_mutex_unlock(&philo->net->last_whisper);
	}
	return (NULL);
}

int	spawn_philos(t_philo *philos)
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

void	check_obituary(t_net *net, long long start)
{
	while (1)
	{
		pthread_mutex_lock(&net->last_whisper);
		if (net->someone_died)
		{
			printf("%lld %d died\n", now() - start, net->someone_died);
			net->retire = 1;
			pthread_mutex_unlock(&net->last_whisper);
			return ;
		}
		pthread_mutex_unlock(&net->last_whisper);
		usleep(100);
	}
}

void	retire_philosophers(t_philo *philos)
{
	unsigned int	i;
	
	i = 0;
	while (i < philos->code.n_of_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_net	net;
	long long		start;

	net.someone_died = 0;
	net.retire = 0;
	pthread_mutex_init(&net.life_feed, NULL);
	pthread_mutex_init(&net.last_whisper, NULL);
	philos = f_init_philos(argc, argv, &net);
	if (!philos)
		return (1);
	start = now();
	if (spawn_philos(philos) == -1)
	{
		free(philos);
		return (2);
	}
	check_obituary(&net, start);
	retire_philosophers(philos);
	pthread_mutex_destroy(&net.last_whisper);
	pthread_mutex_destroy(&net.life_feed);
	free(philos);
	return (0);
}
