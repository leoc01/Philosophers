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
	unsigned int	tt_die;
	unsigned int	tt_eat;
	unsigned int	tt_sleep;
	long long		first_meal;
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
	long long		start;
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

long long	message(const char *msg, t_philo *philo, long long start);
int	philo_net(int action, t_net *net);

void	*life(void *p);
void	wait_turn(t_philo *philo, long long start, long long next_meal);
long long	wait_start(t_net *net);
int	hold(t_net *net, long long wait_until, long long next_meal);
long long	now(void);

long long	philo_eat(t_philo *philo, long long start, long long next_meal);
long long	philo_sleep(t_philo *philo, long long start, long long next_meal);
int	is_alive(long long next_meal);
int	retire(t_net *net);
void	end_cycle(t_philo *philo);

long long	get_forks(t_philo *philo, long long start, long long next_meal);
int	grab_fork(pthread_mutex_t *fork_m, int *fork_on_table);
void	drop_forks(t_philo *philo);

int	spawn_philos(t_philo *philos);
int			f_is_invalid_args(int argc, const char **arg);
t_philo		*f_init_philos(char **argv, t_net *net);
void	check_obituary(t_net *net);
void	retire_philosophers(t_philo *philos);



#endif
