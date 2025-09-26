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

#include <philo.h>

int	main(int argc, char **argv)
{
	t_times	times;

	if (f_invalid_args(argc, (const char **)&argv[1]))
		return (1);
	f_save_times(&times, (const char **)&argv[1]);
	printf("%d\n", times.philos);
	printf("%d\n", times.ttd);
	printf("%d\n", times.tte);
	printf("%d\n", times.tts);
	printf("%d\n", times.meals);
}
