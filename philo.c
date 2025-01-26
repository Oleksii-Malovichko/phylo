/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:18:13 by alex              #+#    #+#             */
/*   Updated: 2025/01/21 20:38:29 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	clean_all(s_philo **philo, int num_of_philo)
{
	int	i;

	i = 0;
	while (i < num_of_philo)
	{
		pthread_mutex_destroy(philo[i]->left_fork);
		pthread_mutex_destroy(philo[i]->right_fork);
		pthread_mutex_destroy(philo[i]->print_lock);
		pthread_mutex_destroy(philo[i]->exit_lock);
		free(philo[i]->exit_lock);
		free(philo[i]->left_fork);
		free(philo[i]->right_fork);
		free(philo[i]->print_lock);
		free(philo[i]);
		i++;
	}
	free(philo);
}

int	fill_philo(int n, char **args, s_philo **philo, int num_of_philo)
{
	int	i;

	i = 0;
	while (i < num_of_philo)
	{
		philo[i] = (s_philo *)malloc(sizeof(s_philo));
		if (!philo[i])
			return 1;
		philo[i]->id = i + 1;
		philo[i]->time_to_die = atol(args[2]);
		philo[i]->time_to_eat = atol(args[3]);
		philo[i]->time_to_sleep = atol(args[4]);
		philo[i]->left_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        philo[i]->right_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        philo[i]->print_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		philo[i]->exit_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (!philo[i]->exit_lock)
			return (1);
		philo[i]->program_running = 1;
		pthread_mutex_init(philo[i]->exit_lock, NULL);
        if (!philo[i]->left_fork || !philo[i]->right_fork || !philo[i]->print_lock)
            return (1);
        pthread_mutex_init(philo[i]->left_fork, NULL);
        pthread_mutex_init(philo[i]->right_fork, NULL);
        pthread_mutex_init(philo[i]->print_lock, NULL);
        philo[i]->last_meal_time = get_time_ms();
		if (n == 6)
		{
			philo[i]->number_to_eat = atoi(args[5]);
			if (philo[i]->number_to_eat <= 0)
				return (1);
		}
		else
			philo[i]->number_to_eat = 1;
		i++;
	}
	return (0);
}

int	sleeping(s_philo *philo)
{
	long	curr_time = get_time_ms();

	if (curr_time - philo->last_meal_time >= philo->time_to_die)
	{
		printf("%ld Philo %d is died\n", curr_time - philo->last_meal_time, philo->id);
		return (1);
	}
	pthread_mutex_lock(philo->print_lock);
	printf("%ld Philo %d is sleeping\n", curr_time - philo->last_meal_time, philo->id);
	pthread_mutex_unlock(philo->print_lock);
	usleep(philo->time_to_sleep * 1000);
	return (0);
}

int	thinking(s_philo *philo)
{
	long	curr_time = get_time_ms();

	if (curr_time - philo->last_meal_time >= philo->time_to_die)
	{
		printf("%ld Philo %d is died\n", curr_time - philo->last_meal_time, philo->id);
		return (1);
	}
	pthread_mutex_lock(philo->print_lock);
	printf("%ld Philo %d is thinking\n", curr_time - philo->last_meal_time, philo->id);
	pthread_mutex_unlock(philo->print_lock);
	usleep(philo->time_to_sleep * 1000);
	return (0);
}

int	eating(s_philo *philo)
{
	long	curr_time = get_time_ms();
	
	if (curr_time - philo->last_meal_time >= philo->time_to_die)
	{
		printf("%ld Philo %d is died\n", curr_time - philo->last_meal_time, philo->id);
		return (1);
	}
	pthread_mutex_lock(philo->print_lock);
	printf("%ld Philo %d is eating\n", curr_time - philo->last_meal_time, philo->id);
	pthread_mutex_unlock(philo->print_lock);
	usleep(philo->time_to_sleep * 1000);
	philo->last_meal_time = get_time_ms();
	return (0);
}

void	*philo_routine(void *arg)
{
	s_philo	*philo = (s_philo *)arg;

	while (philo->program_running)
	{
		if (sleeping(philo) == 1)
			break;
		if (thinking(philo) == 1)
			break;
		if (eating(philo) == 1)
			break;
		usleep(500 * 1000);
	}
	return (NULL);
}

int	main(int n, char **args)
{
	s_philo 	**philo;
	int			num_of_philo;
	pthread_t	monitor_thread;
	int			i;

	if (n < 5 || n > 6)
		return (1);
	num_of_philo = atoi(args[1]);
	if (num_of_philo <= 0)
		return (1);
	philo = (s_philo **)malloc(sizeof(s_philo *) * num_of_philo);
	if (!philo)
		return (1);
	if (fill_philo(n, args, philo, num_of_philo) == 1)
		return (clean_all(philo, num_of_philo), 1);
	i = 0;
	while (i < num_of_philo)
	{
		if (pthread_create(&philo[i]->thread, NULL, philo_routine, (void *)philo[i]) != 0)
		{
			printf("Error creating thread for philosopher %d\n", i + 1);
			return (clean_all(philo, num_of_philo), 1);
		}
		i++;
	}
	i = 0;
	while (i < num_of_philo)
	{
		if (pthread_join(philo[i]->thread, NULL) != 0)
		{
			printf("Error joining thread for philosopher %d\n", i + 1);
			return (clean_all(philo, num_of_philo), 1);
		}
		i++;
	}
	clean_all(philo, num_of_philo);
}