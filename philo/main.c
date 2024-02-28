/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:26:32 by yrigny            #+#    #+#             */
/*   Updated: 2024/02/28 16:26:34 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	err_arg(void)
{
	printf("Error: Bad arguments.\n"
		"Usage: ./philo <number_of_philosophers>\n"
		"\t<time_to_die> (in milliseconds)\n"
		"\t<time_to_eat> (in milliseconds)\n"
		"\t<time_to_sleep> (in milliseconds)\n"
		"\t<number_of_times_each_philosopher_must_eat> (optional)\n");
	return (0);
}

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

int	main(int ac, char **av)
{
	t_set	env;
	int		i;

	if (!parse_init(ac, av, &env))
		return (err_arg());
	if (!philo_mutex_init(&env))
		return (1);
	env.all_alive = 1;
	i = -1;
	while (++i < env.nb_philo)
		pthread_create(&env.philo[i].tid, NULL, &routine, &env.philo[i]);
	monitor(&env);
	i = -1;
	while (++i < env.nb_philo)
		pthread_join(env.philo[i].tid, NULL);
	i = -1;
	while (++i < env.nb_philo)
		pthread_mutex_destroy(&env.mutex[i]);
	pthread_mutex_destroy(&env.printex);
	free(env.mutex);
	free(env.philo);
	return (0);
}
