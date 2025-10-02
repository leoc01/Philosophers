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
	if (n_of_philos == 1)
		return ;
	else if (n_of_philos % 2 == 0)
	{
		if (me % 2 == 0)
			return ;
		else
			usleep(time_to_eat * 1000 - 10000);
	}
	else
	{
		if (me == n_of_philos - 1)
			usleep(time_to_eat * 1000 * 2 - 10000);
		else if (me % 2 == 0)
			return ;
		else
			usleep(time_to_eat * 1000 - 10000);
	}
}

int		retire(t_net *net)
{
	int	order;

	pthread_mutex_lock(&net->read);
	order = net->retire;
	pthread_mutex_unlock(&net->read);
	if (order)
		return (1);
	return (0);
}

long long	message(const char *msg, t_philo *philo, long long beginning)
{
	long long	msg_time;

	if (retire(philo->net))
		return (0);
	pthread_mutex_lock(&philo->net->life_feed);
	msg_time = now();
	printf("%lld %u %s\n", msg_time - beginning, philo->num + 1, msg);
	pthread_mutex_unlock(&philo->net->life_feed);
	return (msg_time);
}

long long	get_forks(t_philo *philo, long long beginning, long long next_meal)
{
	int	fork1;
	int	fork2;

	fork1 = 0;
	fork2 = 0;
	while (fork1 != 1)
	{
		if (now() > next_meal)
			return (0);
		pthread_mutex_lock(&philo->fork_r);
		if (philo->my_fork == 1)
		{
			philo->my_fork = 0;
			fork1 = 1;
		}
		pthread_mutex_unlock(&philo->fork_r);
		if (fork1)
			message("has taken a fork", philo, beginning);
	}
	while (fork2 != 1)
	{
		if (now() > next_meal)
			return (0);
		if (!philo->fork_l)
			continue ;
		pthread_mutex_lock(philo->fork_l);
		if (*philo->his_fork == 1)
		{
			*philo->his_fork = 0;
			fork2 = 1;
		}
		pthread_mutex_unlock(philo->fork_l);
		if (fork2)
			message("has taken a fork", philo, beginning);
	}
	return (now());
}

void	drop_forks(t_philo *philo)
{
		pthread_mutex_lock(philo->fork_l);
		*philo->his_fork = 1;
		pthread_mutex_unlock(philo->fork_l);
		pthread_mutex_lock(&philo->fork_r);
		philo->my_fork = 1;
		pthread_mutex_unlock(&philo->fork_r);
}

void	*life(void *p)
{
	t_philo		*philo;
	long long	next_meal;
	long long	beginning;
	long long	start_eating;

	philo = (t_philo *)p;
	beginning = wait_start(philo->net);
	wait_turn(philo->code.n_of_philos, philo->num, philo->code.time_to_eat);
	next_meal = beginning + philo->code.time_to_die;
	while (now() < next_meal)
	{
		if (!get_forks(philo, beginning, next_meal))
			break ;
		start_eating = message("is eating", philo, beginning);
		if (!start_eating)
		{
			drop_forks(philo);
			break ;
		}
		next_meal = start_eating + philo->code.time_to_die;
		usleep(philo->code.time_to_eat * 1000);
		drop_forks(philo);
		if (philo->code.meals != 0 && !--philo->remaining_meal)
			return (NULL);
		if (!message("is sleeping", philo, beginning))
			break ;
		usleep(philo->code.time_to_sleep * 1000);
		if (!message("is thinking", philo, beginning))
			break ;
	}
	if (retire(philo->net) == 0)
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
		usleep(6000);
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
