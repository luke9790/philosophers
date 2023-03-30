/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmasetti <lmasetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:46:37 by lmasetti          #+#    #+#             */
/*   Updated: 2023/03/29 11:48:57 by lmasetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
funzione per scrivere i tempi delle azioni dei filosofi e il relativo 
messaggio se lo status exit e' zero (cioe' sono tutti vivi).
Il printf stampa il tempo, l'id del filosofo e la relativa azione che sta
eseguendo.
*/

void	write_all(char *str, t_philo *philo)
{
	int	temp;

	pthread_mutex_lock(&philo->data->write);
	pthread_mutex_lock(&philo->data->eating);
	temp = philo->data->exit;
	pthread_mutex_unlock(&philo->data->eating);
	if (temp == 0)
		printf("%ld\t%d\t%s", get_time() - philo->data->init_time,
			philo->phi_id + 1, str);
	pthread_mutex_unlock(&philo->data->write);
}

/*
la funzione controlla il ciclo dei pasti dei filosofi.
Il numero dei pasti e' uguale al conteggio dei singoli pasti
di ogni filosofo. Se count e' diverso dal numero dei filosofi
significa che qualcuno non ha mangiato e riporta l'errore.

eat count e' il totale dei pasti effettuati dal singolo filosofo
una volta che tutti i filosofi hanno fatto times pasti, aumentiamo
il counter(una volta a filosofo). Se il count e' uguale al numero
dei filosofi significa che tutti hanno mangiato times volte, quindi
possiamo fermare tutto.
*/

static int	check_eat(t_struct *data)
{
	int			count;
	int			i;
	int			meal_count;

	count = 0;
	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_lock(&data->eating);
		meal_count = data->philos[i]->eat_count;
		pthread_mutex_unlock(&data->eating);
		if (meal_count >= data->times)
			count++;
		i++;
	}
	if (count == data->philo_num)
		return (0);
	return (1);
}

/*
funzione per vedere se il filosofo mangia nei tempi prestabiliti
altrimenti muore e cambia lo stato exit in 1 (che impedisce di 
continuare la routine oltre la morte di un filosofo).
*/

static void	check_die2(t_struct *data, int i)
{
	time_t	temp;

	pthread_mutex_lock(&data->eating);
	temp = (get_time() - data->philos[i]->start_eat);
	pthread_mutex_unlock(&data->eating);
	if (temp > data->die)
	{
		write_all("died\n", data->philos[i]);
		pthread_mutex_lock(&data->eating);
		data->exit = 1;
		pthread_mutex_unlock(&data->eating);
	}
	usleep(100);
}

/*
funzione per controllare lo stato dei filosofi a ciclo infinito.
Con check2 valutiamo se il filosofo ha fatto in tempo a mangiare.
Se lo stato exit e' cambiato si interrompe, se specificato il
sesto arg mettiamo i cicli a 1.
Con check eat controlliamo che tutti abbiano mangiato il numero
di volte prestabilito e se va bene interrompiamo il while.
*/

static void	check_die(t_struct	*data)
{
	int			i;

	while (data->cycles == 0)
	{
		i = -1;
		while (++i < data->philo_num && data->exit == 0)
			check_die2(data, i);
		if (data->exit == 1)
			break ;
		if (data->ac == 6)
		{
			if (check_eat(data) == 0)
			{
				pthread_mutex_lock(&data->eating);
				data->cycles = 1;
				pthread_mutex_unlock(&data->eating);
			}
		}
	}
}

/*
la funzione serve per fare i vari controlli di stato e chiudere il 
programmasenza leak. Prima si controlla se sono morti dei filosofi 
(check die) e se ci sono problemi passiamo in anticipo o meno alla 
distruzione dei thread e dei mutex.Joiniamo i vari thread e 
distruggiamo i mutex, liberiamo poi cio' che resta.
*/

void	check_and_exit(t_struct *data)
{
	int	i;

	i = -1;
	check_die(data);
	while (++i < data->philo_num)
		pthread_join(data->philos[i]->thr_arr, NULL);
	i = -1;
	while (++i < data->philo_num)
		pthread_mutex_destroy(&data->mut_arr[i]);
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->eating);
	free_for_all(data);
	free(data);
}
