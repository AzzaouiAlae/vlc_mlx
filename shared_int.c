/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:15 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 18:57:38 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

void	init_mutex(t_shared_int *sint)
{
	pthread_mutexattr_t	attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&sint->mutex, &attr);
	pthread_mutexattr_destroy(&attr);
	sint->value = 0;
}

int	shared_int_access(t_shared_int *shared, int set, int value)
{
	int	val;

	pthread_mutex_lock(&shared->mutex);
	if (set)
		shared->value = value;
	val = shared->value;
	pthread_mutex_unlock(&shared->mutex);
	return (val);
}

bool	should_clean_vlc(void)
{
	return (shared_int_access(&(vars()->shared_flags)->should_clean, 0, 0));
}

bool	should_play_video(void)
{
	return (shared_int_access(&(vars()->shared_flags)->should_play, 0, 0));
}
