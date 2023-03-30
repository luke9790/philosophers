/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmasetti <lmasetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:04:54 by lmasetti          #+#    #+#             */
/*   Updated: 2023/03/29 12:00:59 by lmasetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	fork_a(int left, int right)
{
	if (left < right)
		return (left);
	return (right);
}

int	fork_b(int right, int left)
{
	if (right < left)
		return (left);
	return (right);
}

int	check_1(t_philo *philo_num)
{
	printf("%d\t%d\t%s", 0, 1, "has taken a fork\n");
	usleep(philo_num->data->eat);
	return (1);
}

int	check_loop(t_philo *philo_num)
{
	int		loop;

	pthread_mutex_lock(&philo_num->data->eating);
	loop = philo_num->data->exit;
	pthread_mutex_unlock(&philo_num->data->eating);
	return (loop);
}
