/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmasetti <lmasetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:30:13 by lmasetti          #+#    #+#             */
/*   Updated: 2023/03/29 12:02:07 by lmasetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Con check 1 abbiamo il caso del singolo filosofo. Per il resto blocchiamo
i mutex delle forchette, che vengono prese e usate per mangiare.
Si prende il tempo, aggiorna il conteggio dei pasti e aggiorna il tempo
una volta trascorso quello necessario per mangiare(append time).
*/

static int	lock_forks_and_eat(t_philo *philo)
{
	if (philo->left == philo->right)
		return (check_1(philo));
	pthread_mutex_lock
		(&philo->data->mut_arr[fork_a(philo->left, philo->right)]);
	write_all("has taken a fork\n", philo);
	pthread_mutex_lock
		(&philo->data->mut_arr[fork_b(philo->right, philo->left)]);
	write_all("has taken a fork\n", philo);
	write_all("is eating\n", philo);
	pthread_mutex_lock(&philo->data->eating);
	philo->start_eat = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->eating);
	append_time(philo->data->eat);
	return (0);
}

/*
la funzione prende il filosofo giusto e controlla il loop (se non e' morto).
Se va tutto bene usa la funzione sopra per prendere le forchette e mangiare.
(con lock forks and eat).
Se non ci sono errori sblocca i due mutex e libera le forchette a e b. 
Se temp e' uguale al numero di cicli interrompe la routine, altrimenti passa
a farlo dormire e poi pensare, aggiornando i tempi.

check loop controlla se lo status exit e' sempre a zero (cioe sono tutti vivi)
*/

static void	*is_eating(void *arg)
{
	t_philo	*philo;
	int		temp;

	philo = (t_philo *)arg;
	if (philo->phi_id % 2)
		usleep(15000);
	while (!(check_loop(philo)))
	{
		if ((lock_forks_and_eat(philo)) == 1)
			break ;
		pthread_mutex_unlock
			(&philo->data->mut_arr[fork_b(philo->left, philo->right)]);
		pthread_mutex_unlock
			(&philo->data->mut_arr[fork_a(philo->right, philo->left)]);
		pthread_mutex_lock(&philo->data->eating);
		temp = philo->data->cycles;
		pthread_mutex_unlock(&philo->data->eating);
		if (temp)
			break ;
		write_all("is sleeping\n", philo);
		append_time(philo->data->sleep);
		write_all("is thinking\n", philo);
	}
	return (NULL);
}

/*
con la funzione assegniamo la routine(is eating) a ogni thread, la routine
fa prendere le forchette, mangiare, pensare e dormire e si interrompe solo
se si arriva al numero di cicli stabilito.
se fallisce joiniamo e liberiamo tutto, altrimenti blocchiamo il mutex,
che mangia, e poi sblocchiamo il mutex per permettere l'uso agli altri filosofi
*/

int	create_thread(t_struct *data, int i)
{
	if (pthread_create(&data->philos[i]->thr_arr, NULL,
			&is_eating, data->philos[i]))
	{
		while (i--)
			pthread_join (data->philos[i]->thr_arr, NULL);
		free_for_all(data);
		free (data);
		return (0);
	}
	pthread_mutex_lock(&data->eating);
	data->philos[i]->start_eat = get_time();
	pthread_mutex_unlock(&data->eating);
	return (1);
}
