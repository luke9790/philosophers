/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmasetti <lmasetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 12:04:39 by lmasetti          #+#    #+#             */
/*   Updated: 2023/03/29 12:00:59 by lmasetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_struct	t_struct;

typedef struct s_philo {
	int			phi_id;
	int			eat_count;
	int			left;
	int			right;
	time_t		start_eat;
	pthread_t	thr_arr;
	t_struct	*data;
}	t_philo;

typedef struct s_struct {
	int				philo_num;
	int				exit;
	int				cycles;
	int				times;
	int				ac;
	time_t			init_time;
	time_t			die;
	time_t			eat;
	time_t			sleep;
	pthread_mutex_t	*mut_arr;
	pthread_mutex_t	write;
	pthread_mutex_t	eating;
	t_philo			**philos;

}	t_struct;

void	free_for_all(t_struct *data);
void	*my_malloc(int size);
time_t	get_time(void);
void	append_time(time_t time);
int		ft_atoi(const char *s);
void	write_all(char *str, t_philo *philo);
int		data_init(t_struct *data, int ac, char **av);
void	check_and_exit(t_struct *data);
int		fork_a(int left, int right);
int		fork_b(int right, int left);
int		check_1(t_philo *philo);
int		check_loop(t_philo *philo);
int		create_thread(t_struct *data, int i);

#endif
