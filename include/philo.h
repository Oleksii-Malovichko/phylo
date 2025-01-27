/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:18:19 by alex              #+#    #+#             */
/*   Updated: 2025/01/21 20:34:12 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>

typedef struct t_philo
{
	int				id;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				number_to_eat;
	pthread_t		thread;
	long long		last_meal_time;
	int 			number_of_philos;
	pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t *print_lock;
	int				exit_flag;
	int				program_running;
	pthread_mutex_t	*exit_lock;
}	s_philo;

#endif