/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:26:54 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/26 11:28:13 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*life(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	pthread_mutex_lock(&philo->net->last_whisper);
	printf("I am dead\n");
	philo->net->someone_died = 1;
	pthread_mutex_unlock(&philo->net->last_whisper);
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
		pthread_detach(philos[i].thread);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_net	net;
	int		all_alive;

	all_alive = 1;
	net.someone_died = 0;
	pthread_mutex_init(&net.life_feed, NULL);
	pthread_mutex_init(&net.last_whisper, NULL);
	philos = f_init_philos(argc, argv, &net);
	if (!philos)
		return (1);
	if (spawn_philos(philos) == -1)
	{
		free(philos);
		return (2);
	}
	while (all_alive)
	{
		pthread_mutex_lock(&net.last_whisper);
		if (net.someone_died)
			all_alive = 0;
		pthread_mutex_unlock(&net.last_whisper);
	}
}
