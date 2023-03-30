/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmasetti <lmasetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:04:25 by lmasetti          #+#    #+#             */
/*   Updated: 2023/03/29 11:48:58 by lmasetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_av(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (i < ac)
	{
		if (av[i][0] == '\0')
			return (1);
		if (av[i][0] == '+' && av[i][1] >= '0' && av[i][1] <= '9')
			j++;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (1);
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}

static int	first_check(int ac, char **av)
{
	if (ac < 5 || ac > 6 || check_av(ac, av) == 1)
	{
		write(2, "Invalid arguments\n", 18);
		return (0);
	}
	return (1);
}

/*
nel main controlliamo l'input passato,se valido inizializziamo
le strutture e successivamente usiamo create thread per fare
partire i singoli filosofi con la loro routine 
(mangiare, pensare, dormire) che si interrompe solo se terminano
i cicli(!times!) passati come input. Nel caso non siano specificati usiamo
check and exit per controllare lo stato del loop.
*/

int	main(int ac, char **av)
{
	t_struct	*data;
	int			i;

	if (first_check (ac, av) == 0)
		return (-1);
	data = (t_struct *) malloc(sizeof(t_struct));
	if (!data)
		return (-1);
	i = 0;
	if (data_init(data, ac, av) == 0)
		return (-1);
	while (i < data->philo_num)
	{
		if (create_thread(data, i) == 0)
			return (-1);
		i++;
	}
	check_and_exit(data);
	return (0);
}
