/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 17:35:03 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/09/27 19:56:30 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	f_is_digit(int c);

int	f_is_invalid_args(const int argc, const char **arg)
{
	unsigned int	w;
	unsigned int	c;
	int				is_invalid;

	is_invalid = FALSE;
	if (argc != 5 && argc != 6)
		is_invalid = TRUE;
	w = 0;
	while (arg[w] != NULL)
	{
		c = 0;
		while (arg[w][c] != '\0')
		{
			if (!f_is_digit(arg[w][c]))
				is_invalid = TRUE;
			c++;
		}
		w++;
	}
	if (is_invalid)
		printf(
			"usage: ./philo <np> <td> <te> <ts> <nm>(optional)\n"
			"np - number of philosophers and forks\ntd - time to die\n"
			"te - time to eat\nts - time to sleep\nnm - number of meals\n");
	return (is_invalid);
}

static int	f_is_digit(int c)
{
	if (c >= 48 && c <= 57)
		return (TRUE);
	return (FALSE);
}
