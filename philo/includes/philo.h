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
# define START 1
# define STOP 0

typedef long long	t_ms;

typedef struct s_code
{
	unsigned int	n_of_philos;
	unsigned int	tt_die;
	unsigned int	tt_eat;
	unsigned int	tt_sleep;
	t_ms			first_meal;
	unsigned int	meals;
}	t_code;

typedef struct s_net
{
	pthread_mutex_t	life_feed;
	pthread_mutex_t	go;
	pthread_mutex_t	last_whisper;
	pthread_mutex_t	read;
	int				retire;
	int				obituary;
	t_ms			started_at;
}	t_net;

typedef struct s_philo
{
	unsigned int	num;
	pthread_t		thread;
	pthread_mutex_t	*fork_l;
	pthread_mutex_t	fork_r;
	int				*his_fork;
	int				my_fork;
	int				remaining_meal;
	t_code			code;
	t_net			*net;
}	t_philo;

int				is_invalid_args(int argc, const char **arg);
t_philo			*init_philos(char **argv, t_net *net);
int				handle_net(int action, t_net *net);
t_ms			start(t_net *net);

void			*life(void *p);

t_ms			get_forks(t_philo *philo, t_ms start, t_ms next_meal);
void			drop_forks(t_philo *philo);
t_ms			message(const char *msg, t_philo *philo, t_ms start);

t_ms			now(void);
int				is_alive(t_ms next_meal);
int				should_retire(t_net *net);
int				hold(t_net *net, t_ms wait_until, t_ms next_meal);
void			wait_turn(t_philo *philo, t_ms start, t_ms next_meal);

int				f_is_digit(int c);
int				is_invalid_args(const int argc, const char **arg);
unsigned int	atoui(const char *nptr);

#endif
