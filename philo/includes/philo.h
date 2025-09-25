/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:29:30 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/24 12:53:52 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>

# define TRUE 1
# define FALSE 0

typedef struct s_times
{
	unsigned int	philos;
	unsigned int	ttd;
	unsigned int	tte;
	unsigned int	tts;
	unsigned int	meals;
}	t_times;

int				f_invalid_args(int argc, const char **arg);
int				f_is_digit(int c);
void			f_save_times(t_times *times, const char **arg);
unsigned int	f_atoui(const char *nptr);

#endif
