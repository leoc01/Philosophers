/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:26:54 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/26 11:28:13 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo			*philos;
	unsigned int	i;

	philos = f_init_philos(argc, argv);
	if (!philos)
		return (1);
	i = 0;
	while (i < philos->times.n_of_philos)
	{
		if (pthread_create(
				&philos[i].philo_t,
				NULL,
				&routine,
				(void *)&philos[i]) != 0)
			return (3);
		i++;
	}
	i = 0;
	while (i < philos->times.n_of_philos)
	{
		if (pthread_join(philos[i].philo_t, NULL) != 0)
			return (4);
		i++;
	}
}
