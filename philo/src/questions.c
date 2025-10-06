/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:14:00 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/29 16:19:15 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	now(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

int	is_alive(long long next_meal)
{
	return (now() < next_meal);
}

int	should_retire(t_net *net)
{
	int	order;

	pthread_mutex_lock(&net->life_feed);
	order = net->retire;
	pthread_mutex_unlock(&net->life_feed);
	if (order)
		return (1);
	return (0);
}

int	hold(t_net *net, long long wait_until, long long next_meal)
{
	while (is_alive(next_meal) && !should_retire(net) && now() < wait_until)
		usleep(100);
	if (now() >= wait_until)
		return (1);
	return (0);
}

void	wait_turn(t_philo *philo, long long started_at, long long next_meal)
{
	if (philo->code.n_of_philos == 1)
		return ;
	else if (philo->code.n_of_philos % 2 == 0)
	{
		if (philo->num % 2 != 0)
		{
			message("is thinking", philo, started_at);
			hold(philo->net, started_at + philo->code.tt_eat, next_meal);
		}
	}
	else
	{
		if (philo->num == philo->code.n_of_philos - 1)
		{
			message("is thinking", philo, started_at);
			hold(philo->net, started_at + philo->code.tt_eat * 2, next_meal);
		}
		else if (philo->num % 2 != 0)
		{
			message("is thinking", philo, started_at);
			hold(philo->net, started_at + philo->code.tt_eat, next_meal);
		}
	}
}
