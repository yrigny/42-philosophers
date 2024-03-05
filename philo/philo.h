/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:43:55 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/22 11:45:26 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define DIE 5

typedef struct s_philo
{
	struct timeval	start;
	struct timeval	current;
	pthread_t		tid;
	int				id;
	int				nb_philo;
	int				time_eat;
	int				time_sleep;
	int				must_eat_option_on;
	int				meal_to_eat;
	int				fork_l;
	int				fork_r;
	long			last_meal;
	void			*env;
}					t_philo;

typedef struct s_set
{
	struct timeval	start;
	struct timeval	current;
	long			all_alive;
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				must_eat;
	int				must_eat_option_on;
	pthread_mutex_t	mutex_printf;
	pthread_mutex_t	mutex_all_alive;
	pthread_mutex_t	*mutex_fork;
	pthread_mutex_t	*mutex_meal_to_eat;
	pthread_mutex_t	*mutex_last_meal;
	t_philo			*philo;
}					t_set;

/*parsing*/
int		parse_arg(int ac, char **av, t_set *env);
int		av_toi(int i, char *arg, t_set *env);
int		init_all(t_set *env);
void	init_philo(t_set *env);

/*simulating*/
void	*routine(void *philo);
void	philo_eat(t_set *env, t_philo *phl);
void	monitor(t_set *env);
void	check_famine(t_set *env, int *philo_all_done, t_philo *phl, long time);
void	printmsg(t_set *env, int id, int status);

/*utils*/
long	get_ts_in_ms(struct timeval current, struct timeval start);
void	meal_to_eat_minus(t_set *env, t_philo *phl);
int		meal_left_to_eat(t_set *env, t_philo *phl);
int		philo_all_alive(t_set *env);
void	free_all(t_set *env);

#endif
