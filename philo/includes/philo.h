/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:29:30 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/26 11:24:22 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>

# define TRUE 1
# define FALSE 0

typedef struct s_args
{
	unsigned int	n_of_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	meals;
}	t_args;

typedef struct s_philo
{
	unsigned int	num;
	pthread_t		philo_t;
	pthread_mutex_t	*fork_l;
	pthread_mutex_t	fork_r;
	t_args			args;
}	t_philo;

int		f_is_invalid_args(int argc, const char **arg);
t_philo	*f_init_philos(int argc, char **argv);

#endif
