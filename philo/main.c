#include "philo.h"

void	printmsg(t_set *env, int id, int status)
{
	long	ts;
	char	*msg;

	gettimeofday(&env->current, NULL);
	ts = get_ts_in_ms(env->current, env->start);
	if (status == FORK)
		msg = "is taking a fork\n";
	else if (status == EAT)
		msg = "is eating\n";
	else if (status == SLEEP)
		msg = "is sleeping\n";
	else if (status == THINK)
		msg = "is thinking\n";
	else if (status == DIE)
		"died\n";
	pthread_mutex_lock(&env->printex);
	printf("%4ld %d %s\n", ts, id, msg);
	pthread_mutex_unlock(&env->printex);
}

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
	if (ac < 5 || ac > 6)
		valid_args = 0;
	i = 0;
	while (valid_args && av[++i])
	{
		if (!av_toi(i, av[i], env))
			valid_args = 0;
	}
	if (valid_args && env->nb_philo && env->time_die
			&& env->time_eat && env->time_sleep)
		valid_args = 1;
	if (valid_args && ac == 5)
	{
		env->must_eat = 0;
		env->must_eat_on = 0;
	}
	else if (valid_args && ac == 6)
		env->must_eat_on = 1;
	env->all_alive = 1;
	return (valid_args);
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
		(env->philo)[i].eating = 0;
		(env->philo)[i].life = env->time_die;
		(env->philo)[i].meal_to_eat = env->must_eat;
		(env->philo)[i].env = (void *)env;
	}
	return (1);
}

void	*routine(void *philo)
{
	t_philo	*phl;
	t_set	*env;
	int		fork_l;
	int		fork_r;

	phl = (t_philo *)philo;
	env = (t_set *)phl->env;
	fork_l = phl->id - 1;
	fork_r = phl->id % 4;
	while (env->all_alive)
	{
		pthread_mutex_lock(&env->mutex[fork_l]);
		pthread_mutex_lock(&env->mutex[fork_r]);
		if (env->all_alive)
		{
			printmsg(env, phl->id, FORK);
			printmsg(env, phl->id, FORK);
			printmsg(env, phl->id, EAT);
			phl->eating = 1;
			usleep(1000 * env->time_eat);
			phl->life = env->time_die;
			phl->eating = 0;
		}
		pthread_mutex_unlock(&env->mutex[fork_l]);
		pthread_mutex_unlock(&env->mutex[fork_r]);
		if (env->must_eat_on && (phl->meal_to_eat)-- == 0)
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

void	monitor(t_set *env)
{

}

int	main(int ac, char **av)
{
	t_set	env;

	if (!parse_init(ac, av, &env))
		return (err_arg());
	if (!philo_mutex_init(&env))
		return (1);
	int i = -1;
	while (++i < env.nb_philo)
		pthread_create(&env.philo[i].tid, NULL, &routine, &env.philo[i]);
	// main routine function
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
	return 0;
}
