/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:26:40 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/28 16:26:45 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	av_toi(int i, char *arg, t_set *env)
{
	int	nbr;
	int	idx;

	nbr = 0;
	idx = 0;
	while (arg[idx] && arg[idx] >= '0' && arg[idx] <= '9')
		idx++;
	if (arg[idx] != '\0')
		return (0);
	idx = -1;
	while (arg[++idx])
		nbr = nbr * 10 + arg[idx] - '0';
	if (i == 1)
		env->nb_philo = nbr;
	else if (i == 2)
		env->time_die = nbr;
	else if (i == 3)
		env->time_eat = nbr;
	else if (i == 4)
		env->time_sleep = nbr;
	else if (i == 5)
		env->must_eat = nbr;
	return (nbr > 0);
}

int	philo_mutex_init(t_set *env)
{
	int	i;

	pthread_mutex_init(&env->printex, NULL);
	env->philo = malloc(sizeof(t_philo) * env->nb_philo);
	env->mutex = malloc(sizeof(pthread_mutex_t) * env->nb_philo);
	if (env->philo == NULL || env->mutex == NULL)
		return (0);
	i = -1;
	while (++i < env->nb_philo)
		pthread_mutex_init(&env->mutex[i], NULL);
	i = -1;
	while (++i < env->nb_philo)
	{
		(env->philo)[i].id = i + 1;
		(env->philo)[i].meal_to_eat = env->must_eat;
		(env->philo)[i].last_meal = 0;
		(env->philo)[i].env = (void *)env;
	}
	return (1);
}

int	parse_init(int ac, char **av, t_set *env)
{
	int	valid_args;
	int	i;

	valid_args = 1;
	gettimeofday(&env->start, NULL);
	if (ac < 5 || ac > 6)
		valid_args = 0;
	i = 0;
	while (valid_args && av[++i])
	{
		if (!av_toi(i, av[i], env))
			valid_args = 0;
	}
	if (valid_args && ac == 5)
	{
		env->must_eat = 0;
		env->must_eat_on = 0;
	}
	else if (valid_args && ac == 6)
		env->must_eat_on = 1;
	return (valid_args);
}
