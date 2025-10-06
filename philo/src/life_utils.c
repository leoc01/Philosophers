/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:20:58 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/10/06 18:21:02 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	grab_fork(pthread_mutex_t *fork_m, int *fork_on_table);

t_ms	message(const char *msg, t_philo *philo, t_ms started_at)
{
	pthread_mutex_lock(&philo->net->life_feed);
	if (philo->net->retire)
	{
		pthread_mutex_unlock(&philo->net->life_feed);
		return (0);
	}
	printf("%lld %u %s\n", now() - started_at, philo->num + 1, msg);
	pthread_mutex_unlock(&philo->net->life_feed);
	return (1);
}

t_ms	get_forks(t_philo *philo, t_ms started_at, t_ms next_meal)
{
	int	fork;

	fork = 0;
	while (is_alive(next_meal) && !fork)
	{
		fork = grab_fork(&philo->fork_r, &philo->my_fork);
		usleep(100);
	}
	if (fork)
		message("has taken a fork", philo, started_at);
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
		message("has taken a fork", philo, started_at);
	else
		return (0);
	return (now());
}

static int	grab_fork(pthread_mutex_t *fork_m, int *fork_on_table)
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

void	drop_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_l);
	*philo->his_fork = 1;
	pthread_mutex_unlock(philo->fork_l);
	pthread_mutex_lock(&philo->fork_r);
	philo->my_fork = 1;
	pthread_mutex_unlock(&philo->fork_r);
}
