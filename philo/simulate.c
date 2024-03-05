/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:26:53 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/28 16:26:55 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	printmsg(t_set *env, int id, int status)
{
	long	ts;
	char	*msg;

	gettimeofday(&env->current, NULL);
	ts = get_ts_in_ms(env->current, env->start);
	msg = NULL;
	if (status == FORK)
		msg = "has taken a fork";
	else if (status == EAT)
		msg = "is eating";
	else if (status == SLEEP)
		msg = "is sleeping";
	else if (status == THINK)
		msg = "is thinking";
	else if (status == DIE)
		msg = "died";
	if (status == DIE)
		usleep(100);
	pthread_mutex_lock(&env->mutex_printf);
	printf("%ld %d %s\n", ts, id, msg);
	pthread_mutex_unlock(&env->mutex_printf);
}

void	philo_eat(t_set *env, t_philo *phl)
{
	if (phl->nb_philo == 1)
	{
		printmsg(env, phl->id, FORK);
		return ;
	}
	pthread_mutex_lock(&env->mutex_fork[phl->fork_l]);
	pthread_mutex_lock(&env->mutex_fork[phl->fork_r]);
	if (philo_all_alive(env))
	{
		printmsg(env, phl->id, FORK);
		printmsg(env, phl->id, FORK);
		printmsg(env, phl->id, EAT);
		usleep(1000 * phl->time_eat);
		gettimeofday(&phl->current, NULL);
		pthread_mutex_lock(&env->mutex_last_meal[phl->id - 1]);
		phl->last_meal = get_ts_in_ms(phl->current, phl->start);
		pthread_mutex_unlock(&env->mutex_last_meal[phl->id - 1]);
	}
	pthread_mutex_unlock(&env->mutex_fork[phl->fork_r]);
	pthread_mutex_unlock(&env->mutex_fork[phl->fork_l]);
}

void	*routine(void *philo)
{
	t_philo	*phl;
	t_set	*env;

	phl = (t_philo *)philo;
	env = (t_set *)phl->env;
	if (phl->id % 2 == 0)
		usleep(500 * phl->time_eat);
	while (philo_all_alive(env))
	{
		philo_eat(env, phl);
		if (phl->nb_philo == 1)
			break ;
		if (phl->must_eat_option_on)
			meal_to_eat_minus(env, phl);
		if (phl->must_eat_option_on && !meal_left_to_eat(env, phl))
			break ;
		if (philo_all_alive(env))
		{
			printmsg(env, phl->id, SLEEP);
			usleep(1000 * phl->time_sleep);
		}
		if (philo_all_alive(env))
			printmsg(env, phl->id, THINK);
	}
	return (NULL);
}

void	check_famine(t_set *env, int *philo_all_done, t_philo *phl, long time)
{
	long	philo_hungry;

	if (env->must_eat_option_on && !meal_left_to_eat(env, phl))
		*philo_all_done += 1;
	else
	{
		pthread_mutex_lock(&env->mutex_last_meal[phl->id - 1]);
		philo_hungry = time - phl->last_meal;
		pthread_mutex_unlock(&env->mutex_last_meal[phl->id - 1]);
		if (philo_hungry >= env->time_die)
		{
			pthread_mutex_lock(&env->mutex_all_alive);
			env->all_alive = 0;
			pthread_mutex_unlock(&env->mutex_all_alive);
			printmsg(env, phl->id, DIE);
		}
	}
}

void	monitor(t_set *env)
{
	long	time;
	int		i;
	int		philo_all_done;

	while (philo_all_alive(env))
	{
		philo_all_done = 0;
		gettimeofday(&env->current, NULL);
		time = get_ts_in_ms(env->current, env->start);
		i = -1;
		while (philo_all_alive(env) && ++i < env->nb_philo)
			check_famine(env, &philo_all_done, &env->philo[i], time);
		if (env->must_eat_option_on && philo_all_done == env->nb_philo)
			break ;
		if (!philo_all_alive(env))
			break ;
		usleep(50);
	}
}
