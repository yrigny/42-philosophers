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
# define EAT 1
# define SLEEP 2
# define THINK 3

typedef struct s_set
{
    struct timeval	start;
	struct timeval	current;
    int             nb_philo;
    int             time_die;
    int             time_eat;
    int             time_sleep;
    int             must_eat;
    int             must_eat_on;
    t_philo         *philos;
}                   t_set;

typedef struct s_philo
{
    pthread_t       tid;
    int             id;
    int             status;
    int             fork1;
    int             mutex1;
    int             fork2;
    int             mutex2;
}                   t_philo;


#endif
