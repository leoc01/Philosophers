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

int	retire(t_net *net)
{
	int	order;

	pthread_mutex_lock(&net->life_feed);
	order = net->retire;
	pthread_mutex_unlock(&net->life_feed);
	if (order)
		return (1);
	return (0);
}

int	is_alive(long long next_meal)
{
	return (now() < next_meal);
}

long long	wait_start(t_net *net)
{
	long long	start;

	pthread_mutex_lock(&net->go);
	pthread_mutex_unlock(&net->go);
	pthread_mutex_lock(&net->read);
	start = net->start;
	pthread_mutex_unlock(&net->read);
	return (start);
}

int	hold(t_net *net, long long wait_until, long long next_meal)
{
	while (is_alive(next_meal) && !retire(net) && now() < wait_until)
		usleep(100);
	if (now() >= wait_until)
		return (1);
	return (0);
}

long long	message(const char *msg, t_philo *philo, long long start)
{
	pthread_mutex_lock(&philo->net->life_feed);
	if (philo->net->retire)
	{
		pthread_mutex_unlock(&philo->net->life_feed);
		return (0);
	}
	printf("%lld %u %s\n", now() - start, philo->num + 1, msg);
	pthread_mutex_unlock(&philo->net->life_feed);
	return (1);
}

void	wait_turn(t_philo *philo, long long start, long long next_meal)
{
	if (philo->code.n_of_philos == 1)
		return ;
	else if (philo->code.n_of_philos % 2 == 0)
	{
		if (philo->num % 2 != 0)
		{
			message("is thinking", philo, start);
			hold(philo->net, start + philo->code.tt_eat, next_meal);
		}
	}
	else
	{
		if (philo->num == philo->code.n_of_philos - 1)
		{
			message("is thinking", philo, start);
			hold(philo->net, start + philo->code.tt_eat * 2, next_meal);
		}
		else if (philo->num % 2 != 0)
		{
			message("is thinking", philo, start);
			hold(philo->net, start + philo->code.tt_eat, next_meal);
		}
	}
}

long long	get_forks(t_philo *philo, long long start, long long next_meal)
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
		{
			message("has taken a fork", philo, start);
			break ;
		}
		usleep(100);
	}
	if (!philo->fork_l)
	{
		while (now() > next_meal)
			usleep(100);
	}
	while (fork2 != 1)
	{
		if (now() > next_meal)
		{
			return (0);
		}
		pthread_mutex_lock(philo->fork_l);
		if (*philo->his_fork == 1)
		{
			*philo->his_fork = 0;
			fork2 = 1;
		}
		pthread_mutex_unlock(philo->fork_l);
		if (fork2)
		{
			message("has taken a fork", philo, start);
			break ;
		}
		usleep(100);
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

long long	philo_eat(t_philo *philo, long long start, long long next_meal)
{
	long long	last_meal;

	last_meal = get_forks(philo, start, next_meal);
	if (!last_meal)
		return (0);
	if (!message("is eating", philo, start))
	{
		drop_forks(philo);
		return (0);
	}
	next_meal = last_meal + philo->code.tt_die;
	if (!hold(philo->net, last_meal + philo->code.tt_eat, next_meal))
		return (0);
	drop_forks(philo);
	if (philo->code.meals != 0)
	{
		philo->remaining_meal -= 1;
		if (philo->remaining_meal == 0)
			return (0);
	}
	return (next_meal);
}

long long	philo_sleep(t_philo *philo, long long start, long long next_meal)
{
	long long	sleep_time;

	sleep_time = now();
	if (!message("is sleeping", philo, start))
		return (0);
	if (!hold(philo->net, sleep_time + philo->code.tt_sleep, next_meal))
		return (0);
	return (sleep_time);
}

void	*life(void *p)
{
	t_philo		*philo;
	long long	next_meal;
	long long	start;
	long long	sleep_at;

	philo = (t_philo *)p;
	start = wait_start(philo->net);
	next_meal = start + philo->code.tt_die;
	wait_turn(philo, start, next_meal);
	while (now() < next_meal)
	{
		next_meal = philo_eat(philo, start, next_meal);
		if (!next_meal)
			break ;
		sleep_at = philo_sleep(philo, start, next_meal);
		if (!sleep_at)
			break ;
		if (!message("is thinking", philo, start))
			break ;
		while (now() < next_meal - 10)
			usleep(100);
	}
	if (retire(philo->net) == 0 && philo->remaining_meal > 0)
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
			pthread_mutex_lock(&net->life_feed);
			net->retire = 1;
			printf("%lld %d died\n", now() - net->start, net->obituary);
			pthread_mutex_unlock(&net->life_feed);
			pthread_mutex_unlock(&net->last_whisper);
			return ;
		}
		pthread_mutex_unlock(&net->last_whisper);
		usleep(5000);
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

int	philo_net(int action, t_net *net)
{
	if (action == 1)
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

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_net	net;

	if (f_is_invalid_args(argc, (const char **)&argv[1]))
		return (0);
	philo_net(1, &net);
	philos = f_init_philos(argv, &net);
	if (!philos)
		return (1);
	if (spawn_philos(philos) == -1)
	{
		free(philos);
		return (2);
	}
	usleep(1000);
	net.start = now();
	pthread_mutex_unlock(&net.go);
	check_obituary(&net);
	retire_philosophers(philos);
	philo_net(0, &net);
	free(philos);
	return (0);
}
