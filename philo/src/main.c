/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:26:54 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/29 18:50:29 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	wait_start(t_net *net)
{
	long long	beginning;

	pthread_mutex_lock(&net->start);
	pthread_mutex_unlock(&net->start);
	pthread_mutex_lock(&net->read);
	beginning = net->beginning;
	pthread_mutex_unlock(&net->read);
	return (beginning);
}

void	wait_turn(unsigned int n_of_philos, unsigned int me, unsigned int time_to_eat)
{
	if (n_of_philos % 2 == 0)
	{
		if (me % 2 == 0)
			return ;
		else
			usleep(time_to_eat - 100);
	}
	else
	{
		if (me == n_of_philos - 1)
			return;
		else if (me % 2 == 0)
			usleep(time_to_eat - 100);
		else
			usleep(time_to_eat * 2 - 100);
	}
}

void	*life(void *p)
{
	t_philo		*philo;
	long long	next_meal;
	int			order;
	long long	beginning;

	philo = (t_philo *)p;
	beginning = wait_start(philo->net);
	wait_turn(philo->code.n_of_philos, philo->num, philo->code.time_to_eat);
	next_meal = beginning + philo->code.time_to_die;
	while (now() < next_meal)
	{
		pthread_mutex_lock(&philo->net->read);
		order = philo->net->retire;
		pthread_mutex_unlock(&philo->net->read);
		if (order)
			break ;
		pthread_mutex_lock(&philo->fork_r);
		pthread_mutex_lock(philo->fork_l);
		next_meal = now() + philo->code.time_to_die;
		pthread_mutex_lock(&philo->net->life_feed);
		printf("%lld %u is eating\n", now() - beginning, philo->num + 1);
		pthread_mutex_unlock(&philo->net->life_feed);
		usleep(philo->code.time_to_eat * 1000);
		pthread_mutex_unlock(&philo->fork_r);
		pthread_mutex_unlock(philo->fork_l);
		pthread_mutex_lock(&philo->net->read);
		order = philo->net->retire;
		pthread_mutex_unlock(&philo->net->read);
		if (order)
			break ;
		pthread_mutex_lock(&philo->net->life_feed);
		printf("%lld %u is sleeping\n", now() - beginning, philo->num + 1);
		pthread_mutex_unlock(&philo->net->life_feed);
		usleep(philo->code.time_to_sleep * 1000);
		pthread_mutex_lock(&philo->net->read);
		order = philo->net->retire;
		pthread_mutex_unlock(&philo->net->read);
		if (order)
			break ;
		pthread_mutex_lock(&philo->net->life_feed);
		printf("%lld %u is thinking\n", now() - beginning, philo->num + 1);
		pthread_mutex_unlock(&philo->net->life_feed);
		pthread_mutex_lock(&philo->net->read);
		order = philo->net->retire;
		pthread_mutex_unlock(&philo->net->read);
		if (order)
			break ;
	}
	if (order == 0)
	{
		pthread_mutex_lock(&philo->net->last_whisper);
		philo->net->obituary = philo->num + 1;
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

void	check_obituary(t_net *net)
{
	while (1)
	{
		pthread_mutex_lock(&net->last_whisper);
		if (net->obituary)
		{
			pthread_mutex_lock(&net->read);
			net->retire = 1;
			printf("%lld %d died\n", now() - net->beginning, net->obituary);
			pthread_mutex_unlock(&net->read);
			pthread_mutex_unlock(&net->last_whisper);
			return ;
		}
		pthread_mutex_unlock(&net->last_whisper);
		usleep(1000);
	}
	pthread_mutex_unlock(&net->last_whisper);
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

	net.obituary = 0;
	net.retire = 0;
	pthread_mutex_init(&net.life_feed, NULL);
	pthread_mutex_init(&net.last_whisper, NULL);
	pthread_mutex_init(&net.read, NULL);
	pthread_mutex_init(&net.start, NULL);
	pthread_mutex_lock(&net.start);
	philos = f_init_philos(argc, argv, &net);
	if (!philos)
		return (1);
	if (spawn_philos(philos) == -1)
	{
		free(philos);
		return (2);
	}
	net.beginning = now();
	pthread_mutex_unlock(&net.start);
	check_obituary(&net);
	retire_philosophers(philos);
	pthread_mutex_destroy(&net.last_whisper);
	pthread_mutex_destroy(&net.life_feed);
	pthread_mutex_destroy(&net.read);
	pthread_mutex_destroy(&net.start);
	free(philos);
	return (0);
}
