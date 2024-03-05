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

void	init_philo(t_set *env)
{
	int	i;

	i = -1;
	while (++i < env->nb_philo)
	{
		(env->philo)[i].start = env->start;
		(env->philo)[i].id = i + 1;
		(env->philo)[i].nb_philo = env->nb_philo;
		(env->philo)[i].time_eat = env->time_eat;
		(env->philo)[i].time_sleep = env->time_sleep;
		(env->philo)[i].must_eat_option_on = env->must_eat_option_on;
		(env->philo)[i].meal_to_eat = env->must_eat;
		(env->philo)[i].fork_l = i;
		(env->philo)[i].fork_r = (i + 1) % env->nb_philo;
		(env->philo)[i].last_meal = 0;
		(env->philo)[i].env = (void *)env;
	}
	i = env->nb_philo - 1;
	(env->philo)[i].fork_l = (i + 1) % env->nb_philo;
	(env->philo)[i].fork_r = i;
}

int	init_all(t_set *env)
{
	int	i;

	env->all_alive = 1;
	pthread_mutex_init(&env->mutex_printf, NULL);
	pthread_mutex_init(&env->mutex_all_alive, NULL);
	env->philo = malloc(sizeof(t_philo) * env->nb_philo);
	env->mutex_fork = malloc(sizeof(pthread_mutex_t) * env->nb_philo);
	env->mutex_last_meal = malloc(sizeof(pthread_mutex_t) * env->nb_philo);
	env->mutex_meal_to_eat = malloc(sizeof(pthread_mutex_t) * env->nb_philo);
	if (!env->philo || !env->mutex_fork || !env->mutex_last_meal
		|| !env->mutex_meal_to_eat)
		return (0);
	i = -1;
	while (++i < env->nb_philo)
	{
		pthread_mutex_init(&env->mutex_fork[i], NULL);
		pthread_mutex_init(&env->mutex_last_meal[i], NULL);
		pthread_mutex_init(&env->mutex_meal_to_eat[i], NULL);
	}
	init_philo(env);
	return (1);
}

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

int	parse_arg(int ac, char **av, t_set *env)
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
		env->must_eat_option_on = 0;
	}
	else if (valid_args && ac == 6)
		env->must_eat_option_on = 1;
	return (valid_args);
}

int	main(int ac, char **av)
{
	t_set	env;
	int		i;

	if (!parse_arg(ac, av, &env))
	{
		printf("Error: Bad arguments.\n"
			"Usage: ./philo number_of_philosophers\n"
			"\ttime_to_die (in milliseconds)\n"
			"\ttime_to_eat (in milliseconds)\n"
			"\ttime_to_sleep (in milliseconds)\n"
			"\tnumber_of_times_each_philosopher_must_eat (optional)\n"
			"\tPlease enter positive integer for each argument.\n");
		return (1);
	}
	if (!init_all(&env))
		return (1);
	i = -1;
	while (++i < env.nb_philo)
		pthread_create(&env.philo[i].tid, NULL, &routine, &env.philo[i]);
	monitor(&env);
	i = -1;
	while (++i < env.nb_philo)
		pthread_join(env.philo[i].tid, NULL);
	free_all(&env);
	return (0);
}
