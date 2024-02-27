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
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define DIE 5

typedef struct s_philo
{
    pthread_t       tid;
    int             id;
    int             meal_to_eat;
    struct timeval  time;
    long            last_meal;
    void            *env;
}                   t_philo;

typedef struct s_set
{
    struct timeval	start;
	struct timeval	current;
    int             all_alive;
    int             nb_philo;
    int             time_die;
    int             time_eat;
    int             time_sleep;
    int             must_eat;
    int             must_eat_on;
    pthread_mutex_t printex;
    pthread_mutex_t *mutex; // array of fork mutex
    t_philo         *philo; // array of philo
}                   t_set;

#endif
