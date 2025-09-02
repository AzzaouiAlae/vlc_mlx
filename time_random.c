/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_random.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:18 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/09/02 14:34:59 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

size_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((size_t)(tv.tv_sec * 1000 * 1000 + tv.tv_usec));
}

char	*generate_random_shm_name(void)
{
	const char	charset[]
		= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	char		*name;
	size_t		charset_size;
	size_t		rnd;
	int			i;

	i = 0;
	name = calloc(SHM_NAME_LEN + 1, 1);
	name[0] = '/';
	charset_size = sizeof(charset) - 1;
	rnd = get_time_ms();
	while (++i < SHM_NAME_LEN)
	{
		name[i] = charset[rnd % charset_size];
		rnd /= charset_size;
	}
	name[SHM_NAME_LEN] = '\0';
	return (name);
}
