/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmasetti <lmasetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:46:48 by lmasetti          #+#    #+#             */
/*   Updated: 2023/03/29 11:49:00 by lmasetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	philo_init(t_struct *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->mut_arr[i], NULL))
			return (0);
		data->philos[i] = (t_philo *) malloc(sizeof(t_philo));
		if (!data->philos[i])
			return (0);
		data->philos[i]->phi_id = i;
		data->philos[i]->data = data;
		data->philos[i]->eat_count = 0;
		data->philos[i]->start_eat = 0;
		data->philos[i]->left = i;
		data->philos[i]->right = (i + 1) % data->philo_num;
		i++;
	}
	return (1);
}

/*
la funzione inizializza la struttura data con i parametri input
e successivamente i singoli filosofi. Exit viene usata come flag
per vedere se muore qualcuno, cycles il numero di cicli completi
effettuati, times il numero di volte cui va eseguito il ciclo.
*/

int	data_init(t_struct *data, int ac, char **av)
{
	data->ac = ac;
	if (data->ac == 6)
		data->times = ft_atoi(av[5]);
	data->philo_num = ft_atoi(av[1]);
	data->die = ft_atoi(av[2]);
	data->eat = ft_atoi(av[3]);
	data->sleep = ft_atoi(av[4]);
	data->philos = (t_philo **) malloc(sizeof(t_philo *) * data->philo_num);
	if (!data->philos)
		return (0);
	data->mut_arr = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->mut_arr)
		return (0);
	data->init_time = get_time();
	data->exit = 0;
	data->cycles = 0;
	if (philo_init(data) == 0)
		return (0);
	if (pthread_mutex_init(&data->write, NULL))
		return (0);
	if (pthread_mutex_init(&data->eating, NULL))
		return (0);
	return (1);
}
