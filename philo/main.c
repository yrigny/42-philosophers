#include "philo.h"

int	err_arg(void)
{
	printf("Error: Bad arguments.\n"
		"Usage: ./philo <number_of_philosophers>\n"
		"<time_to_die> (in milliseconds)\n"
		"<time_to_eat> (in milliseconds)\n"
		"<time_to_sleep> (in milliseconds)\n"
		"<number_of_times_each_philosopher_must_eat> (optional)\n");
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

int	av_toi(int i, char *arg, t_set *env)
{
	int	nbr;
	int	idx;

	nbr = 0;
	if (arg[0] == '0' && arg[1] != '\0')
		return (0);
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
	return (1);
}

int	parse_init(int ac, char **av, t_set	*env)
{
	int	valid_args;
	int	i;

	valid_args = 1;
	gettimeofday(&env->start, NULL);
	gettimeofday(&env->current, NULL);
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

int	philo_init(t_philo **philo, int nb_philo)
{
	int	i;

	*philo = malloc(sizeof(t_philo) * nb_philo);
	if (*philo == NULL)
		return (0);
	i = -1;
	while (++i < nb_philo)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].status = THINK;
	}
}

int	main(int ac, char **av)
{
	t_set	env;
	t_philo *philo;
	pthread_mutex_t	mutex;

	if (!parse_init(ac, av, &env))
		return (err_arg());
	if (!philo_init(&philo, env.nb_philo))
		return (1);
	pthread_mutex_init(&mutex, NULL);
	// threads doing their routine
	pthread_mutex_destroy(&mutex);
	// {
	// 	printf("start time: %ld\n", env.start.tv_sec);
	// 	printf("current time: %ld\n", env.current.tv_sec);
	// 	printf("nb_philo: %d\n", env.nb_philo);
	// 	printf("time_die: %d\n", env.time_die);
	// 	printf("time_eat: %d\n", env.time_eat);
	// 	printf("time_sleep: %d\n", env.time_sleep);
	// 	printf("must_eat: %d\n", env.must_eat);
	// 	printf("must_eat_on: %d\n", env.must_eat_on);
	// }
	return 0;
}
