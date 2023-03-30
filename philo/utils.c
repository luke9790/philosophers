/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmasetti <lmasetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:04:49 by lmasetti          #+#    #+#             */
/*   Updated: 2023/03/29 11:49:01 by lmasetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_for_all(t_struct *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
		free(data->philos[i++]);
	free(data->philos);
	free(data->mut_arr);
}

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	append_time(time_t time)
{
	time_t	tmp;

	tmp = get_time() + time;
	while (get_time() < tmp)
		usleep(50);
}

int	ft_atoi(const char *s)
{
	int	i;
	int	nbr;

	i = 0;
	nbr = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
		i++;
	if (s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		nbr = nbr * 10 + s[i] - 48;
		i++;
	}
	return (nbr);
}
