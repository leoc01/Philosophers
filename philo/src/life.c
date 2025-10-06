/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:21:25 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/10/06 18:29:02 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	end_cycle(t_philo *philo);
static t_ms	philo_eat(t_philo *philo, t_ms started_at, t_ms next_meal);
static t_ms	philo_sleep(t_philo *philo, t_ms started_at, t_ms next_meal);

void	*life(void *p)
{
	t_philo	*philo;
	t_ms	next_meal;
	t_ms	started_at;
	t_ms	sleep_at;

	philo = (t_philo *)p;
	started_at = start(philo->net);
	next_meal = started_at + philo->code.tt_die;
	wait_turn(philo, started_at, next_meal);
	while (now() < next_meal)
	{
		next_meal = philo_eat(philo, started_at, next_meal);
		if (!next_meal)
			break ;
		sleep_at = philo_sleep(philo, started_at, next_meal);
		if (!sleep_at)
			break ;
		if (!message("is thinking", philo, started_at))
			break ;
		while (now() < next_meal - 2)
			usleep(100);
	}
	end_cycle(philo);
	return (NULL);
}

static t_ms	philo_eat(t_philo *philo, t_ms started_at, t_ms next_meal)
{
	t_ms	last_meal;

	last_meal = get_forks(philo, started_at, next_meal);
	if (!last_meal)
		return (0);
	if (!message("is eating", philo, started_at))
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

static t_ms	philo_sleep(t_philo *philo, t_ms started_at, t_ms next_meal)
{
	t_ms	sleep_time;

	sleep_time = now();
	if (!message("is sleeping", philo, started_at))
		return (0);
	if (!hold(philo->net, sleep_time + philo->code.tt_sleep, next_meal))
		return (0);
	return (sleep_time);
}

static void	end_cycle(t_philo *philo)
{
	if (philo->remaining_meal > 0 && !should_retire(philo->net))
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
