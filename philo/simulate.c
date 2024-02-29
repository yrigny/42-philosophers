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

void	philo_eat(t_set *env, t_philo *phl)
{
	int	fork_l;
	int	fork_r;

	if (env->nb_philo == 1)
	{
		printmsg(env, phl->id, FORK);
		phl->last_meal = 0;
		return ;
	}
	fork_l = phl->id - 1;
	fork_r = phl->id % env->nb_philo;
	pthread_mutex_lock(&env->mutex[fork_l]);
	pthread_mutex_lock(&env->mutex[fork_r]);
	if (env->all_alive)
	{
		printmsg(env, phl->id, FORK);
		printmsg(env, phl->id, FORK);
		printmsg(env, phl->id, EAT);
		usleep(1000 * env->time_eat);
		gettimeofday(&phl->time, NULL);
		phl->last_meal = get_ts_in_ms(phl->time, env->start);
	}
	pthread_mutex_unlock(&env->mutex[fork_r]);
	pthread_mutex_unlock(&env->mutex[fork_l]);
}

void	*routine(void *philo)
{
	t_philo	*phl;
	t_set	*env;

	phl = (t_philo *)philo;
	env = (t_set *)phl->env;
	if (phl->id % 2 == 0)
		usleep(500 * env->time_eat);
	while (env->all_alive)
	{
		philo_eat(env, phl);
		if (env->nb_philo == 1)
			break ;
		if (env->must_eat_on)
			phl->meal_to_eat -= 1;
		if (env->must_eat_on && phl->meal_to_eat == 0)
			break ;
		if (env->all_alive)
		{
			printmsg(env, phl->id, SLEEP);
			usleep(1000 * env->time_sleep);
		}
		if (env->all_alive)
			printmsg(env, phl->id, THINK);
	}
	return (NULL);
}

void	check_famine(t_set *env, int *ended_philo, t_philo phl, long time)
{
	long	philo_hungry;

	if (env->must_eat_on && phl.meal_to_eat == 0)
		*ended_philo += 1;
	else
	{
		philo_hungry = time - phl.last_meal;
		if (philo_hungry >= env->time_die)
		{
			env->all_alive = 0;
			printmsg(env, phl.id, DIE);
		}
	}
}

void	monitor(t_set *env)
{
	t_philo	*phl;
	long	time;
	int		i;
	int		ended_philo;

	phl = env->philo;
	while (env->all_alive)
	{
		ended_philo = 0;
		gettimeofday(&env->current, NULL);
		time = get_ts_in_ms(env->current, env->start);
		i = -1;
		while (env->all_alive && ++i < env->nb_philo)
			check_famine(env, &ended_philo, phl[i], time);
		if (env->must_eat_on && ended_philo == env->nb_philo)
			break ;
		if (env->all_alive == 0)
			break ;
		usleep(50);
	}
}
