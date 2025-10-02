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
# include <sys/time.h>

# define TRUE 1
# define FALSE 0

typedef struct s_code
{
	unsigned int	n_of_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	long long		first_meal;
	unsigned int	meals;
}	t_code;

typedef struct s_net
{
	pthread_mutex_t	life_feed;
	pthread_mutex_t	start;
	pthread_mutex_t	last_whisper;
	pthread_mutex_t	read;
	int				retire;
	int				obituary;
	long long		beginning;
}	t_net;

typedef struct s_philo
{
	unsigned int	num;
	pthread_t		thread;
	pthread_mutex_t	*fork_l;
	pthread_mutex_t	fork_r;
	int				*his_fork;
	int				my_fork;
	t_code			code;
	t_net			*net;
}	t_philo;

int			f_is_invalid_args(int argc, const char **arg);
t_philo		*f_init_philos(int argc, char **argv, t_net *net);
long long	now(void);

#endif
