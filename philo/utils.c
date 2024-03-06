/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:30:25 by yrigny            #+#    #+#             */
/*   Updated: 2024/03/05 15:30:28 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_ts_in_ms(struct timeval current, struct timeval start)
{
	long	ts_s;
	long	ts_us;

	if (current.tv_usec < start.tv_usec)
		ts_s = current.tv_sec - 1 - start.tv_sec;
	else
		ts_s = current.tv_sec - start.tv_sec;
	if (current.tv_usec < start.tv_usec)
		ts_us = current.tv_usec + 1000000 - start.tv_usec;
	else
		ts_us = current.tv_usec - start.tv_usec;
	return (ts_s * 1000 + ts_us / 1000);
}

void	left_to_eat_minus(t_set *env, t_philo *phl)
{
	pthread_mutex_lock(&env->mutex_left_to_eat[phl->id - 1]);
	phl->left_to_eat -= 1;
	pthread_mutex_unlock(&env->mutex_left_to_eat[phl->id - 1]);
}

int	left_to_eat(t_set *env, t_philo *phl)
{
	int	meal_left_to_eat;

	pthread_mutex_lock(&env->mutex_left_to_eat[phl->id - 1]);
	meal_left_to_eat = phl->left_to_eat;
	pthread_mutex_unlock(&env->mutex_left_to_eat[phl->id - 1]);
	return (meal_left_to_eat);
}

int	philo_all_alive(t_set *env)
{
	int	philo_all_alive;

	pthread_mutex_lock(&env->mutex_all_alive);
	philo_all_alive = env->all_alive;
	pthread_mutex_unlock(&env->mutex_all_alive);
	return (philo_all_alive);
}

void	free_all(t_set *env)
{
	int	i;

	i = -1;
	while (++i < env->nb_philo)
	{
		pthread_mutex_destroy(&env->mutex_fork[i]);
		pthread_mutex_destroy(&env->mutex_last_meal[i]);
		pthread_mutex_destroy(&env->mutex_left_to_eat[i]);
	}
	pthread_mutex_destroy(&env->mutex_printf);
	pthread_mutex_destroy(&env->mutex_all_alive);
	free(env->mutex_fork);
	free(env->mutex_last_meal);
	free(env->mutex_left_to_eat);
	free(env->philo);
}
