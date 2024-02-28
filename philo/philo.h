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
	pthread_t		tid;
	int				id;
	int				meal_to_eat;
	struct timeval	time;
	long			last_meal;
	void			*env;
}					t_philo;

typedef struct s_set
{
	struct timeval	start;
	struct timeval	current;
	int				all_alive;
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				must_eat;
	int				must_eat_on;
	pthread_mutex_t	printex;
	pthread_mutex_t	*mutex;
	t_philo			*philo;
}					t_set;

/*parsing*/
int		err_arg(void);
int		av_toi(int i, char *arg, t_set *env);
int		parse_init(int ac, char **av, t_set *env);
int		philo_mutex_init(t_set *env);

/*simulating*/
void	monitor(t_set *env);
void	check_famine(t_set *env, int *ended_philo, t_philo phl, long time);
void	*routine(void *philo);
void	philo_eat(t_set *env, t_philo *phl);

/*utils*/
void	printmsg(t_set *env, int id, int status);
long	get_ts_in_ms(struct timeval current, struct timeval start);

#endif
