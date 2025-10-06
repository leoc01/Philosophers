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

int	grab_fork(pthread_mutex_t *fork_m, int *fork_on_table)
{
	int	fork_with_philo;

	fork_with_philo = 0;
	pthread_mutex_lock(fork_m);
	if (*fork_on_table)
	{
		*fork_on_table = 0;
		fork_with_philo = 1;
	}
	pthread_mutex_unlock(fork_m);
	return (fork_with_philo);
}

long long	get_forks(t_philo *philo, long long start, long long next_meal)
{
	int	fork;

	fork = 0;
	while (is_alive(next_meal) && !fork)
	{
		fork = grab_fork(&philo->fork_r, &philo->my_fork);
		usleep(100);
	}
	if (fork)
		message("has taken a fork", philo, start);
	else
		return (0);
	while (!philo->fork_l && is_alive(next_meal))
		usleep(100);
	fork = 0;
	while (is_alive(next_meal) && !fork)
	{
		fork = grab_fork(philo->fork_l, philo->his_fork);
		usleep(100);
	}
	if (fork)
		message("has taken a fork", philo, start);
	else
		return (0);
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

void	end_cycle(t_philo *philo)
{
	if (philo->remaining_meal > 0 && !retire(philo->net))
	{
		pthread_mutex_lock(&philo->net->last_whisper);
		philo->net->obituary = philo->num + 1;
		pthread_mutex_unlock(&philo->net->last_whisper);
	}
	else
	{
		pthread_mutex_lock(&philo->net->last_whisper);
		philo->net->obituary = -1;
		pthread_mutex_unlock(&philo->net->last_whisper);
	}
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
		while (now() < next_meal - 2)
			usleep(100);
	}
	end_cycle(philo);
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
		if (net->obituary > 0)
		{
			pthread_mutex_lock(&net->life_feed);
			net->retire = 1;
			printf("%lld %d died\n", now() - net->start, net->obituary);
			pthread_mutex_unlock(&net->life_feed);
			//pthread_mutex_unlock(&net->last_whisper);
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
